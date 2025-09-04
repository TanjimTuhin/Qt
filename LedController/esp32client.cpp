#include "esp32client.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

ESP32Client::ESP32Client(const QString &host, int port, const QString &authPassword, QObject *parent)
    : QObject(parent)
    , socket(new QTcpSocket(this))
    , keepAliveTimer(new QTimer(this))
    , sequenceTimer(new QTimer(this))
    , host(host)
    , port(port)
    , authPassword(authPassword)
    , authenticated(false)
    , sequenceRunning(false)
    , sequenceStep(0)
{
    // Setup socket connections
    connect(socket, &QTcpSocket::connected, this, &ESP32Client::onSocketConnected);
    connect(socket, &QTcpSocket::disconnected, this, &ESP32Client::onSocketDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &ESP32Client::onSocketError);
    connect(socket, &QTcpSocket::readyRead, this, &ESP32Client::onDataReceived);

    // Setup keep alive timer (25 seconds)
    keepAliveTimer->setInterval(25000);
    keepAliveTimer->setSingleShot(false);
    connect(keepAliveTimer, &QTimer::timeout, this, &ESP32Client::sendKeepAlive);

    // Setup sequence timer
    sequenceTimer->setSingleShot(true);
    connect(sequenceTimer, &QTimer::timeout, this, &ESP32Client::performSequenceStep);
}

ESP32Client::~ESP32Client()
{
    disconnect();
}

void ESP32Client::connectToHost()
{
    if (socket->state() != QAbstractSocket::UnconnectedState) {
        socket->disconnectFromHost();
        if (socket->state() != QAbstractSocket::UnconnectedState) {
            socket->waitForDisconnected(3000);
        }
    }

    authenticated = false;
    messageBuffer.clear();
    socket->connectToHost(host, port);
}

void ESP32Client::disconnect()
{
    keepAliveTimer->stop();
    sequenceTimer->stop();
    sequenceRunning = false;

    if (socket->state() != QAbstractSocket::UnconnectedState) {
        socket->disconnectFromHost();
        if (socket->state() != QAbstractSocket::UnconnectedState) {
            socket->waitForDisconnected(3000);
        }
    }

    authenticated = false;
}

bool ESP32Client::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState && authenticated;
}

void ESP32Client::controlLed(int ledNumber, bool state)
{
    if (!isConnected()) return;

    QJsonObject message;
    message["command"] = "set_led";
    message["led"] = ledNumber;
    message["state"] = state;

    sendMessage(message);

    // Request immediate status update
    QTimer::singleShot(100, this, &ESP32Client::requestStatus);
}

void ESP32Client::controlAllLeds(bool state)
{
    if (!isConnected()) return;

    QJsonObject message;
    message["command"] = "set_all_leds";
    message["state"] = state;

    sendMessage(message);

    // Request immediate status update
    QTimer::singleShot(100, this, &ESP32Client::requestStatus);
}

void ESP32Client::toggleLedSequence()
{
    if (!isConnected() || sequenceRunning) return;

    startSequence();
}

void ESP32Client::requestStatus()
{
    if (!isConnected()) return;

    QJsonObject message;
    message["command"] = "get_status";
    sendMessage(message);
}

void ESP32Client::onSocketConnected()
{
    qDebug() << "Connected to ESP32 at" << host << ":" << port;
    // Wait for authentication challenge, then authenticate
    QTimer::singleShot(500, this, &ESP32Client::authenticate);
}

void ESP32Client::onSocketDisconnected()
{
    qDebug() << "Disconnected from ESP32";
    keepAliveTimer->stop();
    sequenceTimer->stop();
    sequenceRunning = false;
    authenticated = false;
    emit connectionStateChanged(false);
}

void ESP32Client::onSocketError(QAbstractSocket::SocketError error)
{
    QString errorString;
    switch (error) {
    case QAbstractSocket::HostNotFoundError:
        errorString = "Host not found. Please check the IP address.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        errorString = "Connection refused. Make sure the ESP32 is running and accessible.";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errorString = "ESP32 closed the connection.";
        break;
    case QAbstractSocket::NetworkError:
        errorString = "Network error occurred.";
        break;
    default:
        errorString = socket->errorString();
        break;
    }

    qDebug() << "Socket error:" << errorString;
    emit errorOccurred(errorString);
}

void ESP32Client::onDataReceived()
{
    QByteArray data = socket->readAll();
    messageBuffer += QString::fromUtf8(data);

    // Process complete lines
    while (messageBuffer.contains('\n')) {
        int newlinePos = messageBuffer.indexOf('\n');
        QString line = messageBuffer.left(newlinePos).trimmed();
        messageBuffer.remove(0, newlinePos + 1);

        if (line.isEmpty()) continue;

        // Parse JSON
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &error);

        if (error.error == QJsonParseError::NoError && doc.isObject()) {
            processMessage(doc.object());
        } else {
            qDebug() << "Failed to parse JSON:" << line << "Error:" << error.errorString();
        }
    }
}

void ESP32Client::sendKeepAlive()
{
    if (!isConnected()) return;

    QJsonObject message;
    message["command"] = "ping";
    sendMessage(message);
}

void ESP32Client::performSequenceStep()
{
    if (!sequenceRunning || !isConnected()) return;

    if (sequenceStep < 5) {
        // Turn LEDs on one by one (steps 0-4)
        controlLed(sequenceStep + 1, true);
        sequenceStep++;
        sequenceTimer->start(200); // 200ms delay
    } else if (sequenceStep == 5) {
        // Hold all LEDs on (step 5)
        sequenceStep++;
        sequenceTimer->start(1000); // 1 second hold
    } else if (sequenceStep < 11) {
        // Turn LEDs off one by one (steps 6-10)
        int ledToTurnOff = sequenceStep - 5;
        controlLed(ledToTurnOff, false);
        sequenceStep++;
        if (sequenceStep < 11) {
            sequenceTimer->start(200); // 200ms delay
        } else {
            sequenceRunning = false; // Sequence complete
        }
    }
}

void ESP32Client::sendMessage(const QJsonObject &message)
{
    if (socket->state() != QAbstractSocket::ConnectedState) return;

    QJsonDocument doc(message);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
    socket->write(data);
}

void ESP32Client::processMessage(const QJsonObject &message)
{
    QString type = message["type"].toString();

    if (type == "status") {
        emit statusReceived(message);
    } else if (message.contains("status")) {
        QString status = message["status"].toString();
        if (status == "success" && !authenticated) {
            authenticated = true;
            keepAliveTimer->start();
            emit connectionStateChanged(true);
            qDebug() << "Authentication successful";

            // Request initial status
            requestStatus();
        } else if (status == "error") {
            QString errorMsg = message["message"].toString();
            emit errorOccurred("ESP32 Error: " + errorMsg);
            if (!authenticated) {
                socket->disconnectFromHost();
            }
        }
    } else {
        qDebug() << "Received message:" << message;
    }
}

void ESP32Client::authenticate()
{
    if (socket->state() != QAbstractSocket::ConnectedState) return;

    QJsonObject authMessage;
    authMessage["command"] = "auth";
    authMessage["password"] = authPassword;

    sendMessage(authMessage);
}

void ESP32Client::startSequence()
{
    sequenceRunning = true;
    sequenceStep = 0;
    performSequenceStep();
}
