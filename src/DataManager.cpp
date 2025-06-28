// DataManager.cpp (Parte de la implementación)
#include "DataManager.h"
#include <QDebug>

// Constructor ya definido en .h si es solo un initializer list

void DataManager::addSerie(const QString &name, const QColor &initialColor, bool initialVisible) {
    if (!m_seriesInfo.contains(name)) {
        SerieInfo info;
        info.color = initialColor;
        info.visible = initialVisible;
        m_seriesInfo.insert(name, info);
        emit serieAdded(name);
        qDebug() << "Serie added:" << name;
    } else {
        qDebug() << "Serie already exists:" << name;
    }
}

void DataManager::removeSerie(const QString &name) {
    if (m_seriesInfo.remove(name)) {
        emit serieRemoved(name);
        qDebug() << "Serie removed:" << name;
    }
}
    
/*
void DataManager::addDataTimeToSerie(const QString& name, double time, double value)
{

}
*/
void DataManager::addDataToSerie(const QString& name, double x, double y){
    // Esto asume que DataPoint ya tiene un 'sourceId' que corresponde al 'name' de la serie.
    // De lo contrario, necesitarías una lógica para mapear el DataPoint a una serie.
    const QString& serieName = name; // Usamos sourceId del DataPoint como nombre de serie

    if (!m_seriesInfo.contains(serieName)) {
        qWarning() << "Serie not found:" << serieName;
        return;
    }

    SerieInfo& info = m_seriesInfo[serieName]; // Obtener referencia a la SerieInfo

    info.xData.append(x); // Agregar valor al eje X
    info.yData.append(y); // Agregar valor al eje Y

    emit serieDataChanged(serieName);
}

void DataManager::setSerieColor(const QString &name, const QColor &color) {
    if (m_seriesInfo.contains(name)) {
        m_seriesInfo[name].color = color;
        emit serieColorChanged(name, color);
    }
}

void DataManager::setSerieVisibility(const QString &name, bool visible) {
    if (m_seriesInfo.contains(name)) {
        m_seriesInfo[name].visible = visible;
        emit serieVisibilityChanged(name, visible);
    }
}

void DataManager::clearSerie(const QString &name) {
    if (m_seriesInfo.contains(name)) {
        m_seriesInfo[name].xData.clear();
        m_seriesInfo[name].yData.clear();
        emit serieCleared(name);
    }
}

void DataManager::clearAllSeries() {
    for (const QString& name : m_seriesInfo.keys()) {
        m_seriesInfo[name].xData.clear();
        m_seriesInfo[name].yData.clear();
    }
    emit allSeriesCleared(); // Señal para limpiar todo
}