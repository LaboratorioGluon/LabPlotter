#ifndef DATAMANAGER_H__
#define DATAMANAGER_H__

#include <QObject>
#include <QVector>
#include <QColor>
#include <QHash>
#include <QString>
#include "dataInterface/DataPoint.h"

struct SerieInfo{
    QColor color;
    QVector<double> xData; // Datos del eje X
    QVector<double> yData; // Datos del eje Y
    bool visible; // Indica si la serie es visible en el plot
};

class DataManager: public QObject
{
    Q_OBJECT
    
public:
    explicit DataManager(QObject *parent = nullptr) : QObject(parent) {}
    ~DataManager() override = default;

    void addSerie(const QString &name, const QColor &initialColor, bool initialVisible);
    void removeSerie(const QString& name);    

    void addDataToSerie(const QString& name, double x, double y);
    //void addDataPoint(const DataPoint& dataPoint);
    //void addDataTimeToSerie(const QString& name, double time, double value);
    

    void setSerieColor(const QString& name, const QColor& color);
    void setSerieVisibility(const QString& name, bool visible);
    void clearSerie(const QString& name);
    void clearAllSeries();

    QColor getSerieColor(const QString& name) const { return m_seriesInfo.value(name).color; };
    bool isSerieVisible(const QString& name) const { return m_seriesInfo.value(name).visible; };
    QVector<double> getSerieXData(const QString& name) const { return m_seriesInfo.value(name).xData; };
    QVector<double> getSerieYData(const QString& name) const { return m_seriesInfo.value(name).yData; };

signals:
    void serieAdded(const QString& name);
    void serieRemoved(const QString& name);

    void serieDataChanged(const QString& name);

    void serieColorChanged(const QString& name, const QColor& color);
    void serieVisibilityChanged(const QString& name, bool visible);
    void serieCleared(const QString& name);
    void allSeriesCleared();


private:
    QHash<QString,SerieInfo> m_seriesInfo; // Almacena información de las series para el plot
    double m_timestamp = 0.0;
};

#endif //DATAMANAGER_H__