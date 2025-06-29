#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SourceManager.h"
#include "DataManager.h"
#include "dataInterface/DataPoint.h"
#include <QTreeWidget>
#include <QMap>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
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
    void newData(const DataPoint &data, const QString &sourceId);

    void refreshPlot();

    void signalListItemChanged(QTreeWidgetItem *item, int column);

    void mousePress(QMouseEvent *event);
    void mouseWheel(QWheelEvent *event);

private:
    Ui::MainWindow *ui;

    SourceManager *m_sourceManager;
    DataManager *m_dataManager;
    QTimer *m_plotRefreshTimer;

    QVector<DataPoint> m_dataPoints; // Almacena los puntos de datos para el plot

    struct SerieToUI
    {
        int graphIndex;
        QTreeWidgetItem *treeItem;
        QCPAxis *axis;
    };

    QMap<int, SerieToUI> m_seriesToUI;

    // plot data
    int datacount;
    int xAxisNumElements = 500;
    bool isPlotLive = true;

    QColor m_plotColors[14] =
        {
            QColor("#fffb4934"),
            QColor("#ffb8bb26"),
            QColor("#fffabd2f"),
            QColor("#ff83a598"),
            QColor("#ffd3869b"),
            QColor("#ff8ec07c"),
            QColor("#fffe8019"),
            QColor("#ffcc241d"),
            QColor("#ff98971a"),
            QColor("#ffd79921"),
            QColor("#ff458588"),
            QColor("#ffb16286"),
            QColor("#ff689d6a"),
            QColor("#ffd65d0e"),
        };
};
#endif // MAINWINDOW_H