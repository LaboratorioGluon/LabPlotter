#include "FileSource.h"
#include <QDebug>

FileSource::FileSource(const QString& id, const QString& filePath, QObject *parent)
    : IDataSource(id, parent),m_file( new QFile(parent)), m_filePath(filePath)
{
    m_file->setFileName(m_filePath);
}

FileSource::~FileSource()
{
    stop();
}

bool FileSource::start()
{
    bool res = m_file->open(QIODevice::ReadOnly | QIODevice::Text);
    qDebug() << res;

    char buffer[256];
    int readbytes = 0;
    while( !m_file->atEnd())
    {
        QByteArray line = m_file->readLine();
        emit dataReceived(line, id()); // Emite la señal con los datos RAW tal cual se leen
    }

    return res;
}

void FileSource::stop()
{

}

void FileSource::readData()
{
    
}