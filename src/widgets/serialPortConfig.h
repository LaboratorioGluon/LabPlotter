#ifndef SERALPORTCONFIG_H__
#define SERALPORTCONFIG_H__

#include <QWidget>

namespace Ui{
    class SerialPortConfig;
}

class SerialPortConfig : public QWidget {
    Q_OBJECT
public:
    explicit SerialPortConfig(QWidget *parent = nullptr);
    ~SerialPortConfig() override;   

    void setupUi(QWidget *serialPortPanel);

    void setConnectionEnabled(bool enabled);

signals:
    void serialPortConnectChange(bool connect, const QString &portName, int baudRate);

private:
    Ui::SerialPortConfig *ui;

    bool isConnected = false;
};

#endif // SERALPORTCONFIG_H__