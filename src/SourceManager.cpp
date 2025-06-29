// SourceManager.cpp
#include "SourceManager.h"
#include <QDebug>

SourceManager::SourceManager(QObject *parent) : QObject(parent)
{
}

SourceManager::~SourceManager()
{
    qDeleteAll(m_dataSources);
    m_dataSources.clear();

    qDeleteAll(m_parsers);
    m_parsers.clear();
}

void SourceManager::addSource(IDataSource* source)
{
    if (!source) return;

    if (m_dataSources.contains(source)) {
        qWarning() << "Data source already added:" << source->id() << ". Ignoring.";
        return;
    }

    connect(source, &IDataSource::dataReceived, this, &SourceManager::onSourceDataReceived);
    m_dataSources.append(source);
    qDebug() << "Added data source:" << source->id();
}

void SourceManager::removeSource(const QString& sourceId)
{

    IDataSource* source = nullptr;
    for (IDataSource* s : m_dataSources) {
        if (s->id() == sourceId) {
            source = s;
            break;
        }
    }   

    if (!source) {
        qWarning() << "No data source found with ID:" << sourceId;
        return;
    }

    disconnect(source, &IDataSource::dataReceived, this, &SourceManager::onSourceDataReceived);
    m_dataSources.removeAll(source);
    delete source;
    qDebug() << "Removed data source:" << source->id();
}

void SourceManager::registerParser(const QString& sourceIdOrFormatType, IDataParser* parser)
{
    if (m_parsers.contains(sourceIdOrFormatType)) {
        delete m_parsers.value(sourceIdOrFormatType);
        m_parsers.remove(sourceIdOrFormatType);

    }

    if (parser)
    {
        m_parsers.insert(sourceIdOrFormatType, parser);
        qDebug() << "Registered parser for" << sourceIdOrFormatType << ":" << parser->formatType();
    }
}

bool SourceManager::startAllSources()
{
    bool allStarted = true;
    for (IDataSource* source : m_dataSources) {
        if( !source->start() )
        {
            qWarning() << "Failed to start data source:" << source->id();
            allStarted = false;
        }

        // Llamar a reset() en el parser asociado a esta fuente al iniciar
        IDataParser* parser = m_parsers.value(source->id());
        if (parser) {
            parser->reset();
        }
    }

    return allStarted;
}

void SourceManager::stopAllSources()
{
    for (IDataSource* source : m_dataSources) {
        source->stop();
    }
}

void SourceManager::onSourceDataReceived(const QByteArray& rawData, const QString& sourceId)
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

bool SourceManager::startSource(const QString& sourceId)
{
    IDataSource* source = nullptr;
    for (IDataSource* s : m_dataSources) {
        if (s->id() == sourceId) {
            source = s;
            break;
        }
    }

    if (!source) {
        qWarning() << "No data source found with ID:" << sourceId;
        return false;
    }

    return source->start();
}

void SourceManager::stopSource(const QString& sourceId)
{
    IDataSource* source = nullptr;
    for (IDataSource* s : m_dataSources) {
        if (s->id() == sourceId) {
            source = s;
            break;
        }
    }

    if (!source) {
        qWarning() << "No data source found with ID:" << sourceId;
        return;
    }

    source->stop();
}