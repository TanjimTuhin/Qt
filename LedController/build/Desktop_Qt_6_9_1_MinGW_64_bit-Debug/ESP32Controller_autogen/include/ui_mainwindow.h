/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *connectionGroupBox;
    QHBoxLayout *connectionLayout;
    QLabel *ipLabel;
    QLineEdit *ipLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QPushButton *connectButton;
    QLabel *statusLabel;
    QGroupBox *ledGroupBox;
    QVBoxLayout *ledVerticalLayout;
    QHBoxLayout *ledCheckboxLayout;
    QCheckBox *led1CheckBox;
    QCheckBox *led2CheckBox;
    QCheckBox *led3CheckBox;
    QCheckBox *led4CheckBox;
    QCheckBox *led5CheckBox;
    QHBoxLayout *ledButtonLayout;
    QPushButton *allOnButton;
    QPushButton *allOffButton;
    QPushButton *sequenceButton;
    QGroupBox *statusGroupBox;
    QVBoxLayout *statusLayout;
    QTextEdit *statusTextEdit;
    QPushButton *disconnectButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(507, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        connectionGroupBox = new QGroupBox(centralwidget);
        connectionGroupBox->setObjectName("connectionGroupBox");
        connectionLayout = new QHBoxLayout(connectionGroupBox);
        connectionLayout->setObjectName("connectionLayout");
        ipLabel = new QLabel(connectionGroupBox);
        ipLabel->setObjectName("ipLabel");

        connectionLayout->addWidget(ipLabel);

        ipLineEdit = new QLineEdit(connectionGroupBox);
        ipLineEdit->setObjectName("ipLineEdit");

        connectionLayout->addWidget(ipLineEdit);

        portLabel = new QLabel(connectionGroupBox);
        portLabel->setObjectName("portLabel");

        connectionLayout->addWidget(portLabel);

        portLineEdit = new QLineEdit(connectionGroupBox);
        portLineEdit->setObjectName("portLineEdit");
        portLineEdit->setMaximumSize(QSize(80, 16777215));

        connectionLayout->addWidget(portLineEdit);

        passwordLabel = new QLabel(connectionGroupBox);
        passwordLabel->setObjectName("passwordLabel");

        connectionLayout->addWidget(passwordLabel);

        passwordLineEdit = new QLineEdit(connectionGroupBox);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setMaximumSize(QSize(150, 16777215));

        connectionLayout->addWidget(passwordLineEdit);


        verticalLayout->addWidget(connectionGroupBox);

        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName("connectButton");

        verticalLayout->addWidget(connectButton, 0, Qt::AlignmentFlag::AlignLeft);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");

        verticalLayout->addWidget(statusLabel);

        ledGroupBox = new QGroupBox(centralwidget);
        ledGroupBox->setObjectName("ledGroupBox");
        ledVerticalLayout = new QVBoxLayout(ledGroupBox);
        ledVerticalLayout->setObjectName("ledVerticalLayout");
        ledCheckboxLayout = new QHBoxLayout();
        ledCheckboxLayout->setObjectName("ledCheckboxLayout");
        led1CheckBox = new QCheckBox(ledGroupBox);
        led1CheckBox->setObjectName("led1CheckBox");

        ledCheckboxLayout->addWidget(led1CheckBox);

        led2CheckBox = new QCheckBox(ledGroupBox);
        led2CheckBox->setObjectName("led2CheckBox");

        ledCheckboxLayout->addWidget(led2CheckBox);

        led3CheckBox = new QCheckBox(ledGroupBox);
        led3CheckBox->setObjectName("led3CheckBox");

        ledCheckboxLayout->addWidget(led3CheckBox);

        led4CheckBox = new QCheckBox(ledGroupBox);
        led4CheckBox->setObjectName("led4CheckBox");

        ledCheckboxLayout->addWidget(led4CheckBox);

        led5CheckBox = new QCheckBox(ledGroupBox);
        led5CheckBox->setObjectName("led5CheckBox");

        ledCheckboxLayout->addWidget(led5CheckBox);


        ledVerticalLayout->addLayout(ledCheckboxLayout);

        ledButtonLayout = new QHBoxLayout();
        ledButtonLayout->setObjectName("ledButtonLayout");
        allOnButton = new QPushButton(ledGroupBox);
        allOnButton->setObjectName("allOnButton");

        ledButtonLayout->addWidget(allOnButton);

        allOffButton = new QPushButton(ledGroupBox);
        allOffButton->setObjectName("allOffButton");

        ledButtonLayout->addWidget(allOffButton);

        sequenceButton = new QPushButton(ledGroupBox);
        sequenceButton->setObjectName("sequenceButton");

        ledButtonLayout->addWidget(sequenceButton);


        ledVerticalLayout->addLayout(ledButtonLayout);


        verticalLayout->addWidget(ledGroupBox);

        statusGroupBox = new QGroupBox(centralwidget);
        statusGroupBox->setObjectName("statusGroupBox");
        statusLayout = new QVBoxLayout(statusGroupBox);
        statusLayout->setObjectName("statusLayout");
        statusTextEdit = new QTextEdit(statusGroupBox);
        statusTextEdit->setObjectName("statusTextEdit");
        QFont font;
        font.setFamilies({QString::fromUtf8("Consolas")});
        font.setPointSize(9);
        statusTextEdit->setFont(font);

        statusLayout->addWidget(statusTextEdit);


        verticalLayout->addWidget(statusGroupBox);

        disconnectButton = new QPushButton(centralwidget);
        disconnectButton->setObjectName("disconnectButton");

        verticalLayout->addWidget(disconnectButton, 0, Qt::AlignmentFlag::AlignRight);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 507, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "ESP32 IoT Control System", nullptr));
        connectionGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Connection", nullptr));
        ipLabel->setText(QCoreApplication::translate("MainWindow", "IP:", nullptr));
        ipLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "192.168.1.100", nullptr));
        portLabel->setText(QCoreApplication::translate("MainWindow", "Port:", nullptr));
        passwordLabel->setText(QCoreApplication::translate("MainWindow", "Password:", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        statusLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: red;", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "Disconnected", nullptr));
        ledGroupBox->setTitle(QCoreApplication::translate("MainWindow", "LED Control", nullptr));
        led1CheckBox->setText(QCoreApplication::translate("MainWindow", "LED 1", nullptr));
        led2CheckBox->setText(QCoreApplication::translate("MainWindow", "LED 2", nullptr));
        led3CheckBox->setText(QCoreApplication::translate("MainWindow", "LED 3", nullptr));
        led4CheckBox->setText(QCoreApplication::translate("MainWindow", "LED 4", nullptr));
        led5CheckBox->setText(QCoreApplication::translate("MainWindow", "LED 5", nullptr));
        allOnButton->setText(QCoreApplication::translate("MainWindow", "All ON", nullptr));
        allOffButton->setText(QCoreApplication::translate("MainWindow", "All OFF", nullptr));
        sequenceButton->setText(QCoreApplication::translate("MainWindow", "Toggle Sequence", nullptr));
        statusGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Status", nullptr));
        statusTextEdit->setPlainText(QCoreApplication::translate("MainWindow", "Not connected to ESP32", nullptr));
        disconnectButton->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
