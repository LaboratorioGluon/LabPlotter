// CharSeparatedParser.h
#ifndef SPACE_SEPARATED_PARSER_H
#define SPACE_SEPARATED_PARSER_H

#include "IDataParser.h"
#include <QStringList>

class CharSeparatedParser : public IDataParser
{
    Q_OBJECT
public:
    explicit CharSeparatedParser(QObject *parent = nullptr, const char separator = ',')
        : IDataParser(parent), m_separator(separator) {};
    ~CharSeparatedParser() override = default;

    QList<DataPoint> processAndParse(const QByteArray& newData) override;
    QString formatType() const override { return "SpaceSeparated"; }


    const void setSeparator(const char separator) { m_separator = separator; }

    void reset() override;
private:

    QByteArray m_buffer; // Buffer para almacenar datos parciales
    char m_separator; // Separador de campos, por defecto es coma
};

#endif // SPACE_SEPARATED_PARSER_H

