#include "SerialPortSource.h"
#include <QDebug>

SerialPortSource::SerialPortSource(const QString& id, const QString& portName, QObject *parent)
    : IDataSource(id, parent), m_portName(portName)
{
    m_serialPort.setPortName(m_portName);
    m_serialPort.setBaudRate(QSerialPort::Baud115200);
    // ... otras configuraciones

    connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialPortSource::readData);
    connect(&m_serialPort, &QSerialPort::errorOccurred, this, [&](QSerialPort::SerialPortError error){
        if (error != QSerialPort::NoError && error != QSerialPort::ResourceError) {
            qDebug() << "Serial port error on" << m_portName << ":" << m_serialPort.errorString();
        }
    });
}

SerialPortSource::~SerialPortSource()
{
    stop();
}

bool SerialPortSource::start()
{
    if (m_serialPort.open(QIODevice::ReadOnly)) {
        qDebug() << "Serial port" << m_portName << "opened.";
        return true;
    } else {
        qDebug() << "Failed to open serial port" << m_portName << ":" << m_serialPort.errorString();
        return false;
    }
}

void SerialPortSource::stop()
{
    if (m_serialPort.isOpen()) {
        m_serialPort.close();
        qDebug() << "Serial port" << m_portName << "closed.";
    }
}

void SerialPortSource::readData()
{
    QByteArray data = m_serialPort.readAll();
    if (!data.isEmpty()) {
        
        emit dataReceived(data, id()); // Emite la señal con los datos RAW tal cual se leen
    }
}