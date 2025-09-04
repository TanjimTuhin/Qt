#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QTimer>
#include <QThread>
#include "esp32client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void connectToESP();
    void onConnectionStateChanged(bool connected);
    void onStatusReceived(const QJsonObject &status);
    void onLedCheckboxToggled(bool checked);
    void onAllOnClicked();
    void onAllOffClicked();
    void onSequenceClicked();
    void updateStatusDisplay();

private:
    void setupConnections();
    void loadLastIP();
    void saveLastIP(const QString &ip);
    void updateLedCheckboxes(const QJsonArray &leds);
    void setConnectedState(bool connected);

    Ui::MainWindow *ui;
    ESP32Client *client;
    QList<QCheckBox*> ledCheckboxes;
    QTimer *statusUpdateTimer;
    bool isConnected;
    QJsonObject lastStatus;

    static const QString CONFIG_FILE;
};

#endif // MAINWINDOW_H
