#ifndef CONNECTIONPANEL_H__
#define CONNECTIONPANEL_H__

#include <QWidget>

namespace Ui{
    class ConnectionPanel;
}

class ConnectionPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionPanel(QWidget *parent = nullptr);
    ~ConnectionPanel() override;

    void setSerialPanel(bool connected);

private slots:
    void on_serialConnectChange(bool connect, const QString &portName, int baudRate);

signals:
    void serialPortConnectChange(bool connect, const QString &portName, int baudRate);
    
private:
    void configureSerialTab();
    Ui::ConnectionPanel *ui;
};

#endif // CONNECTIONPANEL_H__