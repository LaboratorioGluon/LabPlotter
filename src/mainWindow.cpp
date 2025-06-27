#include "mainWindow.h"
#include "ui_mainWindow.h" // Se genera automáticamente por UIC

#include "dataInterface/SerialPortSource.h"
#include "dataInterface/CharSeparatedParser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dataManager(new DataManager(this)) // Inicializa el DataManager
{
    ui->setupUi(this);
    
    m_dataPoints.clear();

    setupPlot();

    connect(m_dataManager, &DataManager::newDataAvailable, this, &MainWindow::newData);

    SerialPortSource *serialSource = new SerialPortSource("Serial1", "/dev/ttyUSB0", m_dataManager);
    m_dataManager->addSource(serialSource);
    m_dataManager->registerParser("Serial1", new CharSeparatedParser(this, ','));
    m_dataManager->startAllSources();

    m_plotRefreshTimer = new QTimer(this);
    m_plotRefreshTimer->setInterval(50);
    connect(m_plotRefreshTimer, &QTimer::timeout, this, &MainWindow::refreshPlot);
    m_plotRefreshTimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPlot()
{

    QColor gui_colors[ ] = {
      /* Monochromatic for axes and ui */
      QColor (48,  47,  47,  255), /**<  0: qdark ui dark/background color */
      QColor (80,  80,  80,  255), /**<  1: qdark ui medium/grid color */
      QColor (170, 170, 170, 255), /**<  2: qdark ui light/text color */
      QColor (48,  47,  47,  200)  /**<  3: qdark ui dark/background color w/transparency */
    };

    QCustomPlot *cp = ui->customPlot;
    cp->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                        QCP::iSelectLegend | QCP::iSelectPlottables);
    cp->xAxis->setRange(-1, 1);    
    cp->setBackground (gui_colors[0]);
    cp->setNotAntialiasedElements (QCP::aeAll);

    cp->xAxis->grid()->setPen (QPen(gui_colors[2], 1, Qt::DotLine));
    cp->xAxis->grid()->setSubGridPen (QPen(gui_colors[1], 1, Qt::DotLine));
    cp->xAxis->grid()->setSubGridVisible (true);
    cp->xAxis->setBasePen (QPen (gui_colors[2]));
    cp->xAxis->setTickPen (QPen (gui_colors[2]));
    cp->xAxis->setSubTickPen (QPen (gui_colors[2]));
    cp->xAxis->setTickLabelColor (gui_colors[2]);

    cp->yAxis->grid()->setPen (QPen(gui_colors[2], 1, Qt::DotLine));
    cp->yAxis->grid()->setSubGridPen (QPen(gui_colors[1], 1, Qt::DotLine));
    cp->yAxis->grid()->setSubGridVisible (true);
    cp->yAxis->setBasePen (QPen (gui_colors[2]));
    cp->yAxis->setTickPen (QPen (gui_colors[2]));
    cp->yAxis->setSubTickPen (QPen (gui_colors[2]));
    cp->yAxis->setTickLabelColor (gui_colors[2]);

    // give the axes some labels:
    ui->customPlot->replot();   

}

void MainWindow::newData(const DataPoint& data, const QString& sourceId)
{
    static int datacount = 0;
    m_dataPoints.append(data); 
}

void MainWindow::refreshPlot()
{
    static int datacount = 0;
    QCustomPlot *cp = ui->customPlot;

    while(!m_dataPoints.isEmpty()) {
        DataPoint dp = m_dataPoints.takeFirst();
        if (dp.values.isEmpty()) {
            qWarning() << "Data point is empty, skipping.";
            continue;
        }

        //qDebug() << "Adding data point at index" << datacount << ":" << dp.values;

        for( int i = 0; i < dp.values.size(); i++ ) {

            // Ensure the graph exists for the current index
            if (cp->graphCount() <= i) {
                cp->addGraph();
                QPen pen;
                pen.setWidth(2);
                pen.setStyle(Qt::SolidLine);
                //pen.setColor(m_plotColors[i]); // set color to blue
                pen.setColor(m_plotColors[i]); // set color to blue
                cp->graph(i)->setPen(pen); // set the graph
                cp->graph(i)->setName(QString("Graph %1").arg(i + 1));
                QTreeWidgetItem *item = new QTreeWidgetItem(ui->signalList);
                item->setText(0, QString("Graph %1").arg(i + 1));
                item->setCheckState(0, Qt::Checked);
                ui->signalList->addTopLevelItem( item);
            }
            cp->graph(i)->addData(datacount, dp.values[i]);
        }
        datacount++;

    }

    cp->replot();
}