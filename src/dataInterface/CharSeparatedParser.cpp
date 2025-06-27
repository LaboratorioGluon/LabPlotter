#include "CharSeparatedParser.h"
#include <QDebug>
#include <QJsonDocument> // Puede que ya no sea necesario si solo procesas este formato


QList<DataPoint> CharSeparatedParser::processAndParse(const QByteArray& newData)
{
    m_buffer.append(newData); // Añadir los nuevos datos al buffer
    QList<DataPoint> completeDataPoints; // Lista para almacenar los DataPoints válidos
    
    int endIndex = -1;
    while ((endIndex = m_buffer.indexOf(';')) != -1) {
        // Se encontró un delimitador de fin de mensaje
        QByteArray rawMessage = m_buffer.mid(0, endIndex + 1); // Extraer el mensaje completo
        m_buffer.remove(0, endIndex + 1); // Eliminarlo del buffer

        // Ahora, parsear este mensaje completo (que ya sabemos que es un frame completo)
        DataPoint dp;
        dp.timestamp = QDateTime::currentDateTime(); // O parsear del rawMessage si está ahí

        QString dataString = QString::fromUtf8(rawMessage).trimmed();

        // Validación de formato: empieza con '$' y termina con ';'
        if (!dataString.startsWith("$") || !dataString.endsWith(";")) {
            qWarning() << "CharSeparatedParser: Invalid frame format. Missing '$' or ';'. Frame:" << dataString;
            // No se añade al resultado, se descarta este frame mal formado
            continue; // Ir al siguiente frame en el buffer si existe
        }

        dataString = dataString.mid(1, dataString.length() - 2); // Eliminar '$' y ';'
        QStringList strValues = dataString.split(m_separator, Qt::SkipEmptyParts);

        for (const QString& s : strValues) {
            bool ok;
            double val = s.toDouble(&ok);
            if (ok) {
                dp.values.append(val);
            } else {
                qWarning() << "CharSeparatedParser: Could not parse value '" << s << "'. Skipping.";
            }
        }

        if (!dp.values.isEmpty()) {
            completeDataPoints.append(dp); // Solo añadir si el DataPoint tiene valores
        } else {
            qWarning() << "CharSeparatedParser: Parsed frame resulted in empty DataPoint. Frame:" << dataString;
        }
    }
    // Si el bucle termina, m_buffer contiene solo datos incompletos (o está vacío)
    return completeDataPoints;
}

void CharSeparatedParser::reset()
{
    m_buffer.clear(); // Limpiar el buffer interno del parser
}