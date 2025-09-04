#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>

const QString MainWindow::CONFIG_FILE = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/ESP32Controller/last_ip.txt";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(nullptr)
    , isConnected(false)
{
    ui->setupUi(this);

    // Initialize LED checkboxes list
    ledCheckboxes << ui->led1CheckBox
                  << ui->led2CheckBox
                  << ui->led3CheckBox
                  << ui->led4CheckBox
                  << ui->led5CheckBox;

    // Setup connections
    setupConnections();

    // Load last used IP
    loadLastIP();

    // Set default values
    ui->portLineEdit->setText("8080");
    ui->passwordLineEdit->setText("IoTDevice2024");
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    // Initialize status update timer
    statusUpdateTimer = new QTimer(this);
    connect(statusUpdateTimer, &QTimer::timeout, this, &MainWindow::updateStatusDisplay);

    // Set initial state
    setConnectedState(false);
}

MainWindow::~MainWindow()
{
    if (client) {
        client->disconnect();
        client->deleteLater();
    }
    delete ui;
}

void MainWindow::setupConnections()
{
    // Connection button
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectToESP);

    // LED checkboxes
    for (int i = 0; i < ledCheckboxes.size(); ++i) {
        connect(ledCheckboxes[i], &QCheckBox::toggled, this, &MainWindow::onLedCheckboxToggled);
    }

    // LED control buttons
    connect(ui->allOnButton, &QPushButton::clicked, this, &MainWindow::onAllOnClicked);
    connect(ui->allOffButton, &QPushButton::clicked, this, &MainWindow::onAllOffClicked);
    connect(ui->sequenceButton, &QPushButton::clicked, this, &MainWindow::onSequenceClicked);
}

void MainWindow::connectToESP()
{
    if (isConnected) {
        // Disconnect
        if (client) {
            client->disconnect();
            client->deleteLater();
            client = nullptr;
        }
        setConnectedState(false);
        return;
    }

    // Get connection parameters
    QString ip = ui->ipLineEdit->text().trimmed();
    int port = ui->portLineEdit->text().toInt();
    QString password = ui->passwordLineEdit->text();

    if (ip.isEmpty()) {
        QMessageBox::warning(this, "Connection Error", "Please enter an IP address.");
        return;
    }

    if (port <= 0 || port > 65535) {
        QMessageBox::warning(this, "Connection Error", "Please enter a valid port number.");
        return;
    }

    // Save IP for next time
    saveLastIP(ip);

    // Create and configure client
    client = new ESP32Client(ip, port, password, this);

    connect(client, &ESP32Client::connectionStateChanged, this, &MainWindow::onConnectionStateChanged);
    connect(client, &ESP32Client::statusReceived, this, &MainWindow::onStatusReceived);
    connect(client, &ESP32Client::errorOccurred, this, [this](const QString &error) {
        QMessageBox::critical(this, "ESP32 Error", error);
    });

    // Try to connect
    ui->connectButton->setEnabled(false);
    ui->statusLabel->setText("Connecting...");
    ui->statusLabel->setStyleSheet("color: orange;");

    client->connectToHost();
}

void MainWindow::onConnectionStateChanged(bool connected)
{
    setConnectedState(connected);

    if (connected) {
        statusUpdateTimer->start(1000); // Update every second
        client->requestStatus(); // Get initial status
    } else {
        statusUpdateTimer->stop();
        ui->statusTextEdit->setPlainText("Not connected to ESP32");
    }
}

void MainWindow::onStatusReceived(const QJsonObject &status)
{
    lastStatus = status;
    updateStatusDisplay();

    // Update LED checkboxes
    if (status.contains("leds")) {
        updateLedCheckboxes(status["leds"].toArray());
    }
}

void MainWindow::onLedCheckboxToggled(bool checked)
{
    if (!isConnected || !client) return;

    QCheckBox *senderCheckbox = qobject_cast<QCheckBox*>(sender());
    if (!senderCheckbox) return;

    // Find which LED this is
    int ledNumber = ledCheckboxes.indexOf(senderCheckbox) + 1;
    if (ledNumber > 0) {
        client->controlLed(ledNumber, checked);
    }
}

void MainWindow::onAllOnClicked()
{
    if (isConnected && client) {
        client->controlAllLeds(true);
    }
}

void MainWindow::onAllOffClicked()
{
    if (isConnected && client) {
        client->controlAllLeds(false);
    }
}

void MainWindow::onSequenceClicked()
{
    if (isConnected && client) {
        client->toggleLedSequence();
    }
}

void MainWindow::updateStatusDisplay()
{
    if (lastStatus.isEmpty()) return;

    QString statusText;
    QTextStream stream(&statusText);

    // Timestamp
    if (lastStatus.contains("timestamp")) {
        stream << "Timestamp: " << lastStatus["timestamp"].toString() << "\n\n";
    }

    // LEDs
    stream << "LEDs:\n";
    if (lastStatus.contains("leds")) {
        QJsonArray leds = lastStatus["leds"].toArray();
        for (const QJsonValue &ledValue : leds) {
            QJsonObject led = ledValue.toObject();
            int id = led["id"].toInt();
            bool state = led["state"].toBool();
            stream << "  LED " << id << ": " << (state ? "ON" : "OFF") << "\n";
        }
    }

    // Buttons
    stream << "\nButtons:\n";
    if (lastStatus.contains("buttons")) {
        QJsonArray buttons = lastStatus["buttons"].toArray();
        for (const QJsonValue &btnValue : buttons) {
            QJsonObject btn = btnValue.toObject();
            int id = btn["id"].toInt();
            bool pressed = btn["pressed"].toBool();
            stream << "  Button " << id << ": " << (pressed ? "PRESSED" : "released") << "\n";
        }
    }

    // Potentiometer
    stream << "\nPotentiometer:\n";
    if (lastStatus.contains("potentiometer")) {
        QJsonObject pot = lastStatus["potentiometer"].toObject();
        stream << "  Raw: " << pot["raw"].toInt() << "\n";
        stream << "  Voltage: " << QString::number(pot["voltage"].toDouble(), 'f', 2) << " V\n";
        stream << "  Percent: " << pot["percent"].toInt() << " %\n";
    }

    ui->statusTextEdit->setPlainText(statusText);
}

void MainWindow::loadLastIP()
{
    QFile file(CONFIG_FILE);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString ip = in.readLine().trimmed();
        if (!ip.isEmpty()) {
            ui->ipLineEdit->setText(ip);
        }
    }
}

void MainWindow::saveLastIP(const QString &ip)
{
    // Ensure directory exists
    QDir dir;
    dir.mkpath(QFileInfo(CONFIG_FILE).absolutePath());

    QFile file(CONFIG_FILE);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << ip;
    }
}

void MainWindow::updateLedCheckboxes(const QJsonArray &leds)
{
    // Block signals to prevent triggering LED control
    for (QCheckBox *checkbox : ledCheckboxes) {
        checkbox->blockSignals(true);
    }

    for (const QJsonValue &ledValue : leds) {
        QJsonObject led = ledValue.toObject();
        int id = led["id"].toInt();
        bool state = led["state"].toBool();

        if (id >= 1 && id <= ledCheckboxes.size()) {
            ledCheckboxes[id - 1]->setChecked(state);
        }
    }

    // Unblock signals
    for (QCheckBox *checkbox : ledCheckboxes) {
        checkbox->blockSignals(false);
    }
}

void MainWindow::setConnectedState(bool connected)
{
    isConnected = connected;

    if (connected) {
        ui->statusLabel->setText("Connected");
        ui->statusLabel->setStyleSheet("color: green;");
        ui->connectButton->setText("Disconnect");

        // Enable LED controls
        for (QCheckBox *checkbox : ledCheckboxes) {
            checkbox->setEnabled(true);
        }
        ui->allOnButton->setEnabled(true);
        ui->allOffButton->setEnabled(true);
        ui->sequenceButton->setEnabled(true);
    } else {
        ui->statusLabel->setText("Disconnected");
        ui->statusLabel->setStyleSheet("color: red;");
        ui->connectButton->setText("Connect");

        // Disable LED controls
        for (QCheckBox *checkbox : ledCheckboxes) {
            checkbox->setEnabled(false);
            checkbox->setChecked(false);
        }
        ui->allOnButton->setEnabled(false);
        ui->allOffButton->setEnabled(false);
        ui->sequenceButton->setEnabled(false);
    }

    ui->connectButton->setEnabled(true);
}
