#include "recepcion.h"
#include "ui_recepcion.h"
#include <mainwindow.h>

Recepcion::Recepcion(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Recepcion)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setBackgroundBrush(Qt::black);

    QPen greenpen (Qt::green, 3, Qt::DashLine);

    verticalline1 = scene->addLine(0, 0, 0, 400, greenpen);
    verticalline2 = scene->addLine(100, 0, 100, 400, greenpen);
    verticalline3 = scene->addLine(-100, 0, -100, 400, greenpen);
    verticalline4 = scene->addLine(200, 0, 200, 400, greenpen);
    verticalline5 = scene->addLine(-200, 0, -200, 400, greenpen);

    //connect(MainWindow, SIGNAL(mensajito(QString&)), this, SLOT(recibemensaje(QString&)));

    statusrecep = "e";
    statusestimul = "a";


}

Recepcion::~Recepcion()
{
    delete ui;
}

void Recepcion::on_pushButton_clicked()
{
    statusrecep = "e";
    emit estimular_y_recibir2(statusestimul, statusrecep);
}

void Recepcion::on_pushButton_2_clicked()
{
    statusrecep = "a";
    emit estimular_y_recibir2(statusestimul, statusrecep);
}

void Recepcion::on_pushButton_3_clicked()
{
    ui->plainTextEdit->clear();
}

void Recepcion::recibemensaje(QString &meme)
{
    QString mensaje = meme;
    ui->plainTextEdit->insertPlainText(mensaje + "\n");
}
