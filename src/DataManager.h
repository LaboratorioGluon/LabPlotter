// DataManager.h
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include "dataInterface/IDataSource.h"
#include "dataInterface/IDataParser.h"
#include "dataInterface/DataPoint.h"

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

    void addSource(IDataSource* source);
    void registerParser(const QString& sourceIdOrFormatType, IDataParser* parser);

    void startAllSources();
    void stopAllSources();

signals:
    void newDataAvailable(const DataPoint& data, const QString& sourceId);

private slots:
    // Este slot sigue recibiendo QByteArray (raw del SerialPortSource)
    void onSourceDataReceived(const QByteArray& rawData, const QString& sourceId);

private:
    QList<IDataSource*> m_dataSources;
    QMap<QString, IDataParser*> m_parsers;
};

#endif // DATA_MANAGER_H