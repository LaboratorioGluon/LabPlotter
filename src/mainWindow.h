#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SourceManager.h"
#include "DataManager.h"
#include "dataInterface/DataPoint.h"
#include <QTreeWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**** PLOT ****/
    void setupPlot();    

private slots:
    // Slot to update the plot with new data
    void newData(const DataPoint& data, const QString& sourceId);

    void refreshPlot();

    void signalListItemChanged(QTreeWidgetItem *item, int column);


private:
    Ui::MainWindow *ui;

    SourceManager *m_sourceManager;
    DataManager *m_dataManager;
    QTimer *m_plotRefreshTimer;

    QVector<DataPoint> m_dataPoints; // Almacena los puntos de datos para el plot


    QColor m_plotColors[14] = 
    {
      QColor ("#fb4934"),
      QColor ("#b8bb26"),
      QColor ("#fabd2f"),
      QColor ("#83a598"),
      QColor ("#d3869b"),
      QColor ("#8ec07c"),
      QColor ("#fe8019"),
      QColor ("#cc241d"),
      QColor ("#98971a"),
      QColor ("#d79921"),
      QColor ("#458588"),
      QColor ("#b16286"),
      QColor ("#689d6a"),
      QColor ("#d65d0e"),
    };
};
#endif // MAINWINDOW_H