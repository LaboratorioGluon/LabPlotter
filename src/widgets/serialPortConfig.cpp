
#include "widgets/serialPortConfig.h"
#include "../ui/ui_serialPortConfig.h"

#include <QSerialPortInfo>
#include <QSerialPort>

SerialPortConfig::SerialPortConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPortConfig)
{
    ui->setupUi(this);
    ui->baudCombo->clear();
    ui->baudCombo->addItem("1200", QSerialPort::Baud1200);
    ui->baudCombo->addItem("2400", QSerialPort::Baud2400);
    ui->baudCombo->addItem("4800", QSerialPort::Baud4800);
    ui->baudCombo->addItem("9600", QSerialPort::Baud9600);
    ui->baudCombo->addItem("19200", QSerialPort::Baud19200);
    ui->baudCombo->addItem("38400", QSerialPort::Baud38400);
    ui->baudCombo->addItem("57600", QSerialPort::Baud57600);
    ui->baudCombo->addItem("115200", QSerialPort::Baud115200);

    ui->baudCombo->setCurrentIndex(ui->baudCombo->findData(QSerialPort::Baud115200));
    const auto serialPorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : serialPorts) {
        ui->portCombo->addItem(port.portName());  
    }

    connect(ui->connectButton, &QPushButton::clicked, this, [this]() {
        if (isConnected) {
            emit serialPortConnectChange(false, QString(), 0); // Emit empty signal to disconnect
            setConnectionEnabled(false);
            return;
        }
        else
        {
            const QString portName = ui->portCombo->currentText();
            const int baudRate = ui->baudCombo->currentData().toInt();
            emit serialPortConnectChange(true, portName, baudRate);
        }
    });

    connect(ui->refreshPorts, &QPushButton::clicked, this, [this]() {
        ui->portCombo->clear();
        const auto serialPorts = QSerialPortInfo::availablePorts();
        for (const QSerialPortInfo &port : serialPorts) {
            ui->portCombo->addItem(port.portName());
        }
    });
}

SerialPortConfig::~SerialPortConfig()
{
    delete ui;
}

void SerialPortConfig::setConnectionEnabled(bool enabled)
{
    isConnected = enabled;
    ui->connectButton->setText(isConnected ? "Disconnect" : "Connect");
    ui->portCombo->setEnabled(!isConnected);
    ui->baudCombo->setEnabled(!isConnected);
}
