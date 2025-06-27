#ifndef IDATASOURCE_H__
#define IDATASOURCE_H__

#include <QObject>
#include <QByteArray>
#include <QString>

class IDataSource : public QObject
{
    Q_OBJECT

public:
    explicit IDataSource(const QString &id, QObject *parent = nullptr) : QObject(parent), m_id(id) {}

    /**
     * @brief Start the connection to the data source.
     * @return true if connection is successful, false otherwise.
     */
    virtual bool start() = 0;

    /**
     * @brief Disconnects from the data source.
     */
    virtual void stop() = 0;

    QString id() const { return m_id;}

signals:
    void dataReceived(const QByteArray& rawData, const QString& sourceId);


private:
    QString m_id;
};

#endif // IDATASOURCE_H__