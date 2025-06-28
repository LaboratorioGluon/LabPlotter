#include "mainWindow.h"
#include "ui/ui_mainWindow.h" 
#include "widgets/connectionPanel.h"
#include "dataInterface/SerialPortSource.h"
#include "dataInterface/CharSeparatedParser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_sourceManager(new SourceManager(this)) // Inicializa el SourceManager
    , m_dataManager(new DataManager(this)) // Inicializa el DataManager
{
    ui->setupUi(this);
    
    m_dataPoints.clear();
    m_seriesToUI.clear();

    QPushButton*  buttonExtra = new QPushButton("Extra Button", this);
    ui->horizontalLayout_2->addWidget(buttonExtra);

    setupPlot();

    connect(m_sourceManager, &SourceManager::newDataAvailable, this, &MainWindow::newData);

    /*SerialPortSource *serialSource = new SerialPortSource("Serial1", "/dev/ttyUSB0", m_sourceManager);
    m_sourceManager->addSource(serialSource);
    m_sourceManager->registerParser("Serial1", new CharSeparatedParser(this, ','));
    m_sourceManager->startAllSources();*/

    m_plotRefreshTimer = new QTimer(this);
    m_plotRefreshTimer->setInterval(50);
    connect(m_plotRefreshTimer, &QTimer::timeout, this, &MainWindow::refreshPlot);
    m_plotRefreshTimer->start();

    connect(m_dataManager, &DataManager::serieAdded, this, [this](const int index) {
        QCustomPlot *cp = ui->customPlot;
        QString name = m_dataManager->getSerieName(index);
        int graphIndex = cp->graphCount();
        cp->addGraph();
        QPen pen;
        pen.setWidth(2);
        pen.setColor(m_dataManager->getSerieColor(index));
        cp->graph(graphIndex)->setPen(pen);
        cp->graph(graphIndex)->setName(name);
        cp->graph(graphIndex)->setVisible(m_dataManager->isSerieVisible(index));

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->signalList);
        item->setText(0, name);
        item->setForeground(0, QBrush(m_dataManager->getSerieColor(index)));
        item->setCheckState(0, m_dataManager->isSerieVisible(index) ? Qt::Checked : Qt::Unchecked);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
        connect(ui->signalList, &QTreeWidget::itemChanged, this, &MainWindow::signalListItemChanged);
        m_seriesToUI.insert(index, {graphIndex, item});
    });

    connect(m_dataManager, &DataManager::serieNameChanged, this, [this](const int index, const QString &name) {
        QCustomPlot *cp = ui->customPlot;
        if (index < cp->graphCount()) {
            cp->graph(index)->setName(name);
        }
        if (m_seriesToUI.contains(index)) {
            m_seriesToUI[index].treeItem->setText(0, name);
        }
        cp->replot();
    });

    connect(ui->connectionPanel, &ConnectionPanel::serialPortConnectChange, this, [this](bool connect, const QString &portName, int baudRate) {
        if (connect) {
            SerialPortSource *serialSource = new SerialPortSource("Serial1", portName, m_sourceManager);
            m_sourceManager->addSource(serialSource);
            m_sourceManager->registerParser("Serial1", new CharSeparatedParser(this, ','));
            if ( m_sourceManager->startSource("Serial1") )
            {
                ui->connectionPanel->setSerialPanel(true);
            }
        }
        else
        {
            // Stop the source if disconnecting
            m_sourceManager->stopSource("Serial1");
            m_sourceManager->removeSource("Serial1");
            m_sourceManager->registerParser("Serial1", nullptr);
            ui->connectionPanel->setSerialPanel(false);
        }
       
    });

    connect(ui->testButton, &QPushButton::clicked, this, [this]() {
        // Placeholder for test button functionality
        /*if (ui->tabWidget->isVisible()) {
            ui->tabWidget->hide();
            ui->testButton->setText(tr("Show Tabs"));
            ui->testButton->setStyleSheet("background-color: red; color: black;");
        } else {
            ui->tabWidget->show();
            ui->testButton->setText(tr("Hide Tabs"));
            ui->testButton->setStyleSheet("background-color: green; color: black;");
        }*/
        if( ui->stackConfiguracion->isVisible() ) {
            ui->stackConfiguracion->hide();
            ui->testButton->setChecked(false);
            //ui->testButton->setStyleSheet("background-color: green; color: black;");
        } else {
            ui->stackConfiguracion->show();
            ui->testButton->setChecked(true);
            //ui->testButton->setStyleSheet("background-color: red; color: black;");
        }
    });

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

    cp->legend->setVisible (true);
    cp->legend->setBrush (gui_colors[3]);
    cp->legend->setTextColor(gui_colors[2]);
    cp->legend->setBorderPen (gui_colors[2]);

    // give the axes some labels:
    ui->customPlot->replot();   

}

void MainWindow::newData(const DataPoint& data, const QString& sourceId)
{
    static int datacount = 0;
    m_dataPoints.append(data); 
}

void MainWindow::signalListItemChanged(QTreeWidgetItem *item, int column)
{
    //qDebug() << "Signal list item changed:" << item->text(0) << "Column:" << column;
    
    if (item)
    {
        for( int i = 0; i < m_seriesToUI.size(); ++i) {
            if (m_seriesToUI[i].treeItem == item) {
                // Update the series visibility in DataManager
                m_dataManager->setSerieVisibility(m_seriesToUI[i].graphIndex, item->checkState(0) == Qt::Checked);
                m_dataManager->setSerieName(m_seriesToUI[i].graphIndex, item->text(0));
                break;
            }
        }
        // Check if the item is checked or unchecked
        bool isChecked = (item->checkState(0) == Qt::Checked);
        QString graphName = item->text(0);

        // Find the corresponding graph in the plot
        QCustomPlot *cp = ui->customPlot;
        for (int i = 0; i < cp->graphCount(); ++i) {
            if (cp->graph(i)->name() == graphName) {
                cp->graph(i)->setVisible(isChecked);
                break;
            }
        }

        // Replot to reflect changes
        cp->replot();
    }
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
                m_dataManager->addSerie(QString("Graph %1").arg(i + 1), m_plotColors[i], true);
            }
            cp->graph(i)->addData(datacount, dp.values[i]);
        }
        datacount++;

    }

    cp->replot();
}