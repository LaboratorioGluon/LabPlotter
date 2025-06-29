#include "mainWindow.h"
#include "ui/ui_mainWindow.h" 
#include "widgets/connectionPanel.h"
#include "dataInterface/SerialPortSource.h"
#include "dataInterface/CharSeparatedParser.h"
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_sourceManager(new SourceManager(this)) // Inicializa el SourceManager
    , m_dataManager(new DataManager(this)) // Inicializa el DataManager
{
    datacount = 0; // Initialize data count
    ui->setupUi(this);

    m_dataPoints.clear();
    m_seriesToUI.clear();

    QPushButton*  buttonExtra = new QPushButton("Extra Button", this);


    setupPlot();

    connect(m_sourceManager, &SourceManager::newDataAvailable, this, &MainWindow::newData);

    m_plotRefreshTimer = new QTimer(this);
    m_plotRefreshTimer->setInterval(50);
    connect(m_plotRefreshTimer, &QTimer::timeout, this, &MainWindow::refreshPlot);
    m_plotRefreshTimer->start();

    //connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel(QWheelEvent*)));

    connect(ui->signalList, &QTreeWidget::itemChanged, this, &MainWindow::signalListItemChanged);

    connect(ui->customPlot, &QCustomPlot::selectionChangedByUser, this, [this]() {
        if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis)){
            ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());    
            ui->customPlot->axisRect()->setRangeDragAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
        }else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis)){
            ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
            ui->customPlot->axisRect()->setRangeDragAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
            highlightSeriesInAxis(ui->customPlot->yAxis);
        }else if (ui->customPlot->yAxis2->selectedParts().testAnyFlags(QCPAxis::spAxis | QCPAxis::spAxisLabel | QCPAxis::spTickLabels)){
            ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis2->orientation());
            ui->customPlot->axisRect()->setRangeDragAxes(ui->customPlot->xAxis, ui->customPlot->yAxis2);
            highlightSeriesInAxis(ui->customPlot->yAxis2);
            qDebug() << "Plot is live, dragging allowed in vertical direction only.";
        }else{
            ui->customPlot->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
            ui->customPlot->axisRect()->setRangeDragAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
            isPlotLive = false;
            ui->toolGoLive->setEnabled(true);
            highlightSeriesDisable();
            qDebug() << "Plot is not live, dragging allowed in both directions.";
        }
        
        /*// Check which axis is selected and set the range drag accordingly
        if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis)) {
            ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
            ui->customPlot->axisRect()->setRangeDragAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
        }else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis)) {
            qDebug() << "Y Axis selected for dragging.";
        } else if (ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis)) {
            qDebug() << "Y2 Axis selected for dragging.";
        } else {
            qDebug() << "No specific axis selected, allowing full range drag.";
        }
            */
    });

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

        QTreeWidgetItem *axisItem = new QTreeWidgetItem(item);
        QHBoxLayout *axisLayout = new QHBoxLayout();
        axisLayout->setContentsMargins(0, 0, 0, 0);
        axisLayout->setSpacing(5);

        QLabel *axisLabel = new QLabel("Axis:", ui->signalList);
        axisLayout->addWidget(axisLabel);

        QComboBox *axisEdit = new QComboBox(ui->signalList);
        axisEdit->addItem("Y0", QVariant::fromValue(cp->yAxis));
        axisEdit->addItem("Y1", QVariant::fromValue(cp->yAxis2));
        axisLayout->addWidget(axisEdit);
        

        QWidget *axisline = new QWidget(ui->signalList);
        //axisline->setFixedHeight(1);
        axisline->setLayout(axisLayout);

        ui->signalList->setItemWidget(axisItem, 0, axisline);

        //item->insertChild(0, new QLineEdit(new QString("Edit"), this));
        m_seriesToUI.insert(index, {graphIndex, item, cp->yAxis});

        connect(axisEdit, &QComboBox::currentIndexChanged, this, [this, index, axisEdit](const int currentIndex) {
            //m_dataManager->setSerieAxis(index, axis);
            QVariant axisVariant = axisEdit->itemData(currentIndex);
            QCustomPlot *cp = ui->customPlot;
            if (index < cp->graphCount()) {
                //cp->graph(index)->setYAxis(axis == "Y0" ? 0 : 1);
                cp->graph(index)->setValueAxis(axisVariant.value<QCPAxis*>());
                m_seriesToUI[index].axis = axisVariant.value<QCPAxis*>();
                cp->yAxis2->setVisible(true);
                cp->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
                cp->replot();
            }
        });
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
    
    connect(ui->clearDataButton, &QPushButton::clicked, this, [this]() {
        QCustomPlot *cp = ui->customPlot;
        cp->clearGraphs();
        m_dataManager->clearSeries();
        for (auto &serie : m_seriesToUI) {
            delete serie.treeItem; // Delete the tree item associated with the series
        }
        m_seriesToUI.clear();
        ui->signalList->clear();
        m_dataPoints.clear();
        datacount=0;
        cp->replot();
    });

    connect(ui->toolGoLive, &QToolButton::clicked, this, [this]() {
        isPlotLive = true;
        ui->toolGoLive->setEnabled(false);

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
    //cp->setOpenGl(true);
    //cp->setBufferDevicePixelRatio(1.0);
    cp->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                        QCP::iSelectLegend | QCP::iSelectPlottables);

    cp->xAxis->setSelectedBasePen(QPen(QColor(10,200,100), 2));
    cp->xAxis->setSelectedTickPen(QPen(QColor(10,200,100), 2));
    cp->xAxis->setSelectedSubTickPen(QPen(QColor(10,200,100), 2));
    cp->xAxis->setSelectedTickLabelColor(QColor(10,200,100));

    cp->yAxis->setSelectedBasePen(QPen(QColor(10,200,100), 2));
    cp->yAxis->setSelectedTickPen(QPen(QColor(10,200,100), 2));
    cp->yAxis->setSelectedSubTickPen(QPen(QColor(10,200,100), 2));
    cp->yAxis->setSelectedTickLabelColor(QColor(10,200,100));

    cp->yAxis2->setSelectedBasePen(QPen(QColor(10,200,100), 2));
    cp->yAxis2->setSelectedTickPen(QPen(QColor(10,200,100), 2));
    cp->yAxis2->setSelectedSubTickPen(QPen(QColor(10,200,100), 2));
    cp->yAxis2->setSelectedTickLabelColor(QColor(10,200,100));

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

    cp->yAxis2->grid()->setPen (QPen(gui_colors[2], 1, Qt::DotLine));
    cp->yAxis2->grid()->setSubGridPen (QPen(gui_colors[1], 1, Qt::DotLine));
    cp->yAxis2->grid()->setSubGridVisible (true);
    cp->yAxis2->setBasePen (QPen (gui_colors[2]));
    cp->yAxis2->setTickPen (QPen (gui_colors[2]));
    cp->yAxis2->setSubTickPen (QPen (gui_colors[2]));
    cp->yAxis2->setTickLabelColor (gui_colors[2]);
    cp->yAxis2->setSelectableParts(QCPAxis::spAxis | QCPAxis::spAxisLabel | QCPAxis::spTickLabels);


    cp->legend->setVisible (true);
    cp->legend->setBrush (gui_colors[3]);
    cp->legend->setTextColor(gui_colors[2]);
    cp->legend->setBorderPen (gui_colors[2]);

    // give the axes some labels:
    ui->customPlot->replot();   

}


void MainWindow::mousePress(QMouseEvent* event)
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis)){
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());    
    ui->customPlot->axisRect()->setRangeDragAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
  }else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis)){
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
    ui->customPlot->axisRect()->setRangeDragAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
  }else if (ui->customPlot->yAxis2->selectedParts().testFlags(QCPAxis::spAxis | QCPAxis::spAxisLabel | QCPAxis::spTickLabels)){
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis2->orientation());
    ui->customPlot->axisRect()->setRangeDragAxes(ui->customPlot->xAxis, ui->customPlot->yAxis2);
    qDebug() << "Plot is live, dragging allowed in vertical direction only.";
  }else{
    ui->customPlot->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
    ui->customPlot->axisRect()->setRangeDragAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
    isPlotLive = false;
    ui->toolGoLive->setEnabled(true);
    qDebug() << "Plot is not live, dragging allowed in both directions.";
  }
}

void MainWindow::mouseWheel(QWheelEvent* event)
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis)){
    ui->customPlot->axisRect()->setRangeZoomAxes(nullptr, nullptr);
    xAxisNumElements += 10 * event->angleDelta().y() / 120;
  }else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis)){
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
    ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
  }else if (ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis)){
    ui->customPlot->axisRect()->setRangeZoom(Qt::Vertical);
    ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis, ui->customPlot->yAxis2);
  }else{
    ui->customPlot->axisRect()->setRangeZoom( Qt::Vertical);
    ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
  }

}

void MainWindow::newData(const DataPoint& data, const QString& sourceId)
{
    //qDebug() << "New data received from source:" << sourceId << "Data: " << data.values;
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
                qDebug() << "Adding new graph for index" << i;
                m_dataManager->addSerie(QString("Graph %1").arg(i + 1), m_plotColors[i], true);
            }
            cp->graph(i)->addData(datacount, dp.values[i]);
        }
        datacount++;

    }
    if( isPlotLive ) {
        cp->xAxis->setRange(datacount - xAxisNumElements, datacount + 10);
    }
    cp->replot();
}


void MainWindow::highlightSeriesInAxis(const QCPAxis *axis)
{
    QCustomPlot *cp = ui->customPlot;

    for(auto &s: m_seriesToUI) {
        QColor c = m_dataManager->getSerieColor(s.graphIndex);
        if (s.axis != axis) {
            c.setAlpha(100);
        }
        cp->graph(s.graphIndex)->setPen(QPen(c,2));
    }

}


void MainWindow::highlightSeriesDisable()
{
    QCustomPlot *cp = ui->customPlot;
    for(auto &s: m_seriesToUI)
    {
        QColor c = m_dataManager->getSerieColor(s.graphIndex);
        cp->graph(s.graphIndex)->setPen(QPen(c, 2));
    }
}