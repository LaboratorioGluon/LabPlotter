#ifndef FILE_SOURCE_H__
#define FILE_SOURCE_H__

#include "IDataSource.h"
#include <QFile>

class FileSource : public IDataSource
{
    Q_OBJECT
public:
    explicit FileSource(const QString& id, const QString& portName, QObject *parent = nullptr);
    ~FileSource();

    bool start() override;
    void stop() override;

private slots:
    void readData();

private:
    QString m_filePath;
    QFile *m_file;
    // ¡m_readBuffer y m_frameDelimiter ya NO son necesarios aquí!
};

#endif // FILE_SOURCE_H__

