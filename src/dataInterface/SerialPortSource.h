#ifndef SERIAL_PORT_SOURCE_H
#define SERIAL_PORT_SOURCE_H

#include "IDataSource.h"
#include <QSerialPort>

class SerialPortSource : public IDataSource
{
    Q_OBJECT
public:
    explicit SerialPortSource(const QString& id, const QString& portName, QObject *parent = nullptr);
    ~SerialPortSource();

    void start() override;
    void stop() override;

private slots:
    void readData();

private:
    QSerialPort m_serialPort;
    QString m_portName;
};

#endif // SERIAL_PORT_SOURCE_H

