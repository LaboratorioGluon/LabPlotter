#include "mainWindow.h"
#include "ui_mainWindow.h" // Se genera automáticamente por UIC

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Tu código personalizado aquí

    setupPlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPlot()
{

    QCustomPlot *cp = ui->customPlot;
    cp->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                        QCP::iSelectLegend | QCP::iSelectPlottables);
    cp->xAxis->setRange(-1, 1);

    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
    x[i] = i/50.0 - 1; // x goes from -1 to 1
    y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    QPen pen;
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(40, 110, 255)); // set color to blue
    ui->customPlot->graph(0)->setPen(pen); // set the graph
    ui->customPlot->graph(0)->setName("y = x^2");   
    
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(-1, 1);
    ui->customPlot->yAxis->setRange(0, 1);
    ui->customPlot->replot();

}