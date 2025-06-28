#include "connectionPanel.h"
#include "../ui/ui_connectionPanel.h"
#include "serialPortConfig.h"

ConnectionPanel::ConnectionPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionPanel)
{
    ui->setupUi(this);

    connect(ui->widget, &SerialPortConfig::serialPortConnectChange, this, &ConnectionPanel::on_serialConnectChange);

    
}

ConnectionPanel::~ConnectionPanel()
{
    delete ui;
}

void ConnectionPanel::setSerialPanel(bool connected)
{
    ui->widget->setConnectionEnabled(connected);
    

}

void ConnectionPanel::configureSerialTab()
{
    ui->tabWidget->setTabText(0, tr("Serial"));    
}

void ConnectionPanel::on_serialConnectChange(bool connect, const QString &portName, int baudRate)
{
    emit serialPortConnectChange(connect, portName, baudRate); 
}