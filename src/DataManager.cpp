// DataManager.cpp (Parte de la implementación)
#include "DataManager.h"
#include <QDebug>

// Constructor ya definido en .h si es solo un initializer list

int DataManager::addSerie(const QString &name, const QColor &initialColor, bool initialVisible)
{

    SerieInfo info;
    info.color = initialColor;
    info.name = name;
    info.visible = initialVisible;
    m_seriesInfo.append(info); // Agregar la nueva serie a la lista
    int index = m_seriesInfo.size() - 1; // Obtener el índice de la nueva serie
    emit serieAdded(index);
    qDebug() << "Serie added:" << name << "at index:" << index;
    
    return index;
}

/*
void DataManager::removeSerie(const int index)
{
   m_seriesInfo.removeAt(index);
   emit serieRemoved(index);
   qDebug() << "Serie removed at index:" << index;
}*/

void DataManager::addDataToSerie(const int index, double x, double y)
{
    if (index < 0 || index >= m_seriesInfo.size())
    {
        qWarning() << "Index out of range:" << index;
        return;
    }

    SerieInfo &info = m_seriesInfo[index]; // Obtener referencia a la SerieInfo

    info.xData.append(x); // Agregar valor al eje X
    info.yData.append(y); // Agregar valor al eje Y

    emit serieDataChanged(index);
}

void DataManager::setSerieColor(const int index, const QColor &color)
{
    if (index < 0 || index >= m_seriesInfo.size())
    {
        qWarning() << "Index out of range:" << index;
        return;
    }
    m_seriesInfo[index].color = color;
    emit serieColorChanged(index, color);
}

void DataManager::setSerieVisibility(const int index, bool visible)
{
    if (index < 0 || index >= m_seriesInfo.size())
    {
        qWarning() << "Index out of range:" << index;
        return;
    }
    m_seriesInfo[index].visible = visible;
    emit serieVisibilityChanged(index, visible);
}

void DataManager::setSerieName(const int index, const QString &name)
{
    if (index < 0 || index >= m_seriesInfo.size())
    {
        qWarning() << "Index out of range:" << index;
        return;
    }
    m_seriesInfo[index].name = name;
    emit serieNameChanged(index, name);
}