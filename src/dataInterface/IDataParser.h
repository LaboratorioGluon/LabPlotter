// IDataParser.h
#ifndef I_DATA_PARSER_H
#define I_DATA_PARSER_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include "DataPoint.h" // Incluye la estructura de datos normalizada

// Interfaz para cualquier parser de datos
class IDataParser : public QObject
{
    Q_OBJECT
public:
    explicit IDataParser(QObject *parent = nullptr);
    virtual ~IDataParser();

    virtual QList<DataPoint> processAndParse(const QByteArray& newData) = 0;

    virtual QString formatType() const = 0;

    virtual void reset() = 0;
};

#endif // I_DATA_PARSER_H