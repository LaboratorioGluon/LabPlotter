// SourceManager.h
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include "dataInterface/IDataSource.h"
#include "dataInterface/IDataParser.h"
#include "dataInterface/DataPoint.h"

class SourceManager : public QObject
{
    Q_OBJECT
public:
    explicit SourceManager(QObject *parent = nullptr);
    ~SourceManager();

    void addSource(IDataSource* source);
    void removeSource(const QString& sourceId);
    
    void registerParser(const QString& sourceIdOrFormatType, IDataParser* parser);

    bool startSource(const QString& sourceId);
    void stopSource(const QString& sourceId);

    bool startAllSources();
    void stopAllSources();

signals:
    void newDataAvailable(const DataPoint& data, const QString& sourceId);

private slots:
    void onSourceDataReceived(const QByteArray& rawData, const QString& sourceId);

private:
    QList<IDataSource*> m_dataSources;
    QMap<QString, IDataParser*> m_parsers;
};

#endif // DATA_MANAGER_H