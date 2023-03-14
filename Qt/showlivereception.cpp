#include "showlivereception.h"
#include "ui_showlivereception.h"
#include "qcustomplot.h"
#include <mainwindow.h>

ShowLiveReception::ShowLiveReception(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShowLiveReception)
{
    ui->setupUi(this);

    datatiempo = new QTimer(this);
    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(Qt::blue));
    ui->widget->graph(0)->setBrush(QBrush(Qt::NoBrush));
    ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::blue), 9));

    ui->widget->addGraph();
    ui->widget->graph(1)->setPen(QPen(Qt::blue));
    ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->widget->yAxis->setRange(-1, 1050);

    ui->widget->axisRect()->setupFullAxesBox();
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->yAxis2, SLOT(setRange(QCPRange)));
    connect(datatiempo, SIGNAL(timeout()), this, SLOT(pantallazo()));
    sample = 0;
    tiempo = 0;
    datatiempo->start(16);

}

ShowLiveReception::~ShowLiveReception()
{
    delete ui;
}

void ShowLiveReception::mostrandotiemporeal(int &numero)
{
    sample++;
    ui->widget->graph(0)->addData(sample, numero);
    ui->widget->graph(1)->clearData();
    ui->widget->graph(1)->addData(sample, numero);
    //ui->widget->graph(0)->removeDataBefore(sample -8);
    //ui->widget->graph(0)->rescaleValueAxis();
    ui->widget->xAxis->setRange(sample + 2, 8, Qt::AlignRight);
    if(tiempo == 1)
    {
        ui->widget->replot();
        tiempo = 0;
    }
}

void ShowLiveReception::mostrandotiemporeal2(QVector<double> &datillos, QVector<double> &datillos2)
{
    sample++;
    ui->widget->graph(0)->addData(datillos, datillos2);
    ui->widget->graph(1)->clearData();
    ui->widget->graph(1)->addData(datillos.last(), datillos2.last());

    ui->widget->xAxis->setRange(datillos.last() + 2, 8, Qt::AlignRight);
    if(tiempo == 1)
    {
        ui->widget->replot();
        tiempo = 0;
    }
}


void ShowLiveReception::pantallazo()
{
   tiempo = 1;
}

void ShowLiveReception::on_stop_pushButton_clicked()
{
    statusrecep = "a";
    emit estimular_y_recibir3(statusestimul, statusrecep);
}

void ShowLiveReception::on_start_pushButton_clicked()
{
    statusrecep = "e";
    emit estimular_y_recibir3(statusestimul, statusrecep);
}

void ShowLiveReception::on_pushButton_clicked()
{
    ui->widget->graph(0)->clearData();
    ui->widget->graph(1)->clearData();
    ui->widget->replot();
    ui->widget->yAxis->setRange(-1, 1050);
    emit la_senal_mas_nula();
}
