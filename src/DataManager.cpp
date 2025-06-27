// DataManager.cpp
#include "DataManager.h"
#include <QDebug>

DataManager::DataManager(QObject *parent) : QObject(parent)
{
}

DataManager::~DataManager()
{
    qDeleteAll(m_dataSources);
    m_dataSources.clear();

    qDeleteAll(m_parsers);
    m_parsers.clear();
}

void DataManager::addSource(IDataSource* source)
{
    if (!source) return;
    connect(source, &IDataSource::dataReceived, this, &DataManager::onSourceDataReceived);
    m_dataSources.append(source);
    qDebug() << "Added data source:" << source->id();
}

void DataManager::registerParser(const QString& sourceIdOrFormatType, IDataParser* parser)
{
    if (!parser) return;
    if (m_parsers.contains(sourceIdOrFormatType)) {
        qWarning() << "Parser already registered for" << sourceIdOrFormatType << ". Overwriting.";
        delete m_parsers.value(sourceIdOrFormatType);
    }
    m_parsers.insert(sourceIdOrFormatType, parser);
    qDebug() << "Registered parser for" << sourceIdOrFormatType << ":" << parser->formatType();
}

void DataManager::startAllSources()
{
    for (IDataSource* source : m_dataSources) {
        source->start();
        // Llamar a reset() en el parser asociado a esta fuente al iniciar
        IDataParser* parser = m_parsers.value(source->id());
        if (parser) {
            parser->reset();
        }
    }
}

void DataManager::stopAllSources()
{
    for (IDataSource* source : m_dataSources) {
        source->stop();
    }
}

void DataManager::onSourceDataReceived(const QByteArray& rawData, const QString& sourceId)
{
    
    IDataParser* parser = m_parsers.value(sourceId);
    if (!parser) {
        qWarning() << "No parser registered for source ID:" << sourceId << ". Data will not be parsed.";
        return;
    }

    // El parser ahora procesa el chunk y devuelve una lista de DataPoints completos que haya encontrado
    QList<DataPoint> normalizedDataPoints = parser->processAndParse(rawData);

    for (const DataPoint& dp : normalizedDataPoints) {
        if (!dp.values.isEmpty()) {
            emit newDataAvailable(dp, sourceId);
        } else {
            qWarning() << "Parsed frame resulted in empty DataPoint for source" << sourceId << ". Skipping.";
        }
    }
}