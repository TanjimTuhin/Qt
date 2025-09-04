#ifndef ESP32CLIENT_H
#define ESP32CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class ESP32Client : public QObject
{
    Q_OBJECT

public:
    explicit ESP32Client(const QString &host, int port = 8080,
                         const QString &authPassword = "IoTDevice2024",
                         QObject *parent = nullptr);
    ~ESP32Client();

    void connectToHost();
    void disconnect();
    bool isConnected() const;

    void controlLed(int ledNumber, bool state);
    void controlAllLeds(bool state);
    void toggleLedSequence();
    void requestStatus();

signals:
    void connectionStateChanged(bool connected);
    void statusReceived(const QJsonObject &status);
    void errorOccurred(const QString &error);

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);
    void onDataReceived();
    void sendKeepAlive();
    void performSequenceStep();

private:
    void sendMessage(const QJsonObject &message);
    void processMessage(const QJsonObject &message);
    void authenticate();
    void startSequence();

    QTcpSocket *socket;
    QTimer *keepAliveTimer;
    QTimer *sequenceTimer;
    QString host;
    int port;
    QString authPassword;
    bool authenticated;
    QString messageBuffer;

    // Sequence control
    bool sequenceRunning;
    int sequenceStep;
    static const int SEQUENCE_STEPS = 11; // 5 on + 1 wait + 5 off
};

#endif // ESP32CLIENT_H
