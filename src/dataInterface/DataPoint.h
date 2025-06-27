#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <QVector>
#include <QDateTime>
#include <QString>

struct DataPoint {
    QDateTime timestamp;
    QString tag;
    QVector<double> values; // Eje X (tiempo, índice, etc.)
};

#endif // DATA_STRUCT_H