#ifndef DATAMANAGER_H__
#define DATAMANAGER_H__

#include <QObject>
#include <QVector>
#include <QColor>
#include <QHash>
#include <QString>
#include "dataInterface/DataPoint.h"

struct SerieInfo{
    QString name;
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

    int addSerie(const QString &name, const QColor &initialColor, bool initialVisible);
    //int getSerieIndex(const QString& name) const;
    //void removeSerie(const QString& name);    
    void removeSerie(const int index) { m_seriesInfo.removeAt(index); }    

    void addDataToSerie(const int index, double x, double y);
    //void addDataPoint(const DataPoint& dataPoint);
    //void addDataTimeToSerie(const int index, double time, double value);
    
    void clearSeries() { m_seriesInfo.clear(); emit serieRemoved(-1); } // Clear all series and emit signal

    void setSerieColor(const int index, const QColor& color);
    void setSerieVisibility(const int index, bool visible);
    void setSerieName(const int index, const QString& name);


    QColor getSerieColor(const int index) const { return m_seriesInfo[index].color; };
    bool isSerieVisible(const int index) const { return m_seriesInfo[index].visible; };
    QString getSerieName(const int index) const { return m_seriesInfo[index].name; };
    QVector<double> getSerieXData(const int index) const { return m_seriesInfo[index].xData; };
    QVector<double> getSerieYData(const int index) const { return m_seriesInfo[index].yData; };

signals:
    void serieAdded(const int index);
    void serieRemoved(const int index);

    void serieDataChanged(const int index);
    void serieNameChanged(const int index, const QString& name);

    void serieColorChanged(const int index, const QColor& color);
    void serieVisibilityChanged(const int index, bool visible);



private:
    QVector<SerieInfo> m_seriesInfo; // Almacena información de las series para el plot
};

#endif //DATAMANAGER_H__