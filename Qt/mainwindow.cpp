#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dialog.h>
#include <recepcion.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <showlivereception.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ventanapuerto = new Dialog(this);
    recepcion = new Recepcion(this);
    serial = new QSerialPort(this);
    livereception = new ShowLiveReception(this);
    inicioparapintar();
    connections();


    p1 = ui->line_train_1->palette();
    p2 = ui->line_singlepulse_1->palette();

    no_permitir_uso();

    QFile arch;
    QTextStream io;
    QString nombreArch;

    statusrecep = "a";
    statusestimul = "a";
    numerillo = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connections()
{
    connect(ventanapuerto, SIGNAL(serial_configurado(QString)), this, SLOT(enciende_puerto_serial(QString)));
    connect(ui->verticalSlider_trainorsinglepulse, SIGNAL(valueChanged(int)), this, SLOT(mostrar_train()));
    connect(ui->verticalSlider_status, SIGNAL(valueChanged(int)), this, SLOT(status_changer()));
    connect(this, SIGNAL(estimular_y_recibir(QString&,QString&)), this, SLOT(EmpezarEstimulacion(QString&,QString&)));
    connect(ui->dial_masterp_1, SIGNAL(valueChanged(int)), this, SLOT(textmasterp(int)));
    connect(ui->dial_delay_1, SIGNAL(valueChanged(int)), this, SLOT(textdelay(int)));
    connect(ui->dial_traind_1, SIGNAL(valueChanged(int)), this, SLOT(texttraind(int)));
    connect(ui->dial_trainp_1, SIGNAL(valueChanged(int)), this, SLOT(texttrainp(int)));
    connect(ui->dial_pulsed_1, SIGNAL(valueChanged(int)), this, SLOT(textpulsed(int)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(Leerinfo()));

    connect(recepcion, SIGNAL(estimular_y_recibir2(QString&,QString&)), this, SLOT(EmpezarEstimulacion(QString&,QString&)));
    connect(livereception, SIGNAL(estimular_y_recibir3(QString&,QString&)), this, SLOT(EmpezarEstimulacion(QString&,QString&)));
    connect(this, SIGNAL(mostrartiemporeal(int&)), livereception, SLOT(mostrandotiemporeal(int&)));
    connect(this, SIGNAL(mostrartiemporeal2(QVector<double>&,QVector<double>&)), livereception,
            SLOT(mostrandotiemporeal2(QVector<double>&,QVector<double>&)));
    connect(this, SIGNAL(mensajito(QString&)), recepcion, SLOT(recibemensaje(QString&)));
    connect(livereception, SIGNAL(la_senal_mas_nula()), this, SLOT(la_respuesta_mas_nula()));

}

void MainWindow::on_actionConnect_triggered()
{
    ventanapuerto->show();
}

void MainWindow::on_actionReception_triggered()
{
    livereception->show();
    statusrecep = "e";
    emit estimular_y_recibir(statusestimul, statusrecep);

}

void MainWindow::enciende_puerto_serial(QString nombrepuerto3)
{
    serial->setPortName(nombrepuerto3);
    if (serial->open(QIODevice::ReadWrite))
    {
        serial->setBaudRate(QSerialPort::Baud115200);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->setReadBufferSize(0);
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
    }

    if (serial->isOpen())
    {
        permitir_uso();
    }
}

void MainWindow::mostrar_train()
{
    if(ui->verticalSlider_trainorsinglepulse->value() == 0)
    {
        no_permitir_uso_train();
    }
    else
    {
        permitir_uso_train();
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    apagar_todo();
    ui->verticalSlider_trainorsinglepulse->setSliderPosition(0);
    ui->verticalSlider_status->setSliderPosition(1);
    no_permitir_uso();
}

void MainWindow::apagar_todo()
{
    if(serial->isOpen())
    {
        statusestimul = "a";
        statusrecep = "a";
        emit estimular_y_recibir(statusestimul, statusrecep);
        serial->close();
    }
}

void MainWindow::inicioparapintar()
{
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

void MainWindow::pantallazo()
{
    tiempo = 1;
}

void MainWindow::textmasterp(int valor)
{
    double valorf = (double)valor / 10;
    ui->label_valormasterp->setText(QString::number(valorf, 'f', 1));
}

void MainWindow::textdelay(int valor)
{
    double valorf = (double)valor / 10;
    ui->label_valordelay->setText(QString::number(valorf, 'f', 1));
}

void MainWindow::texttraind(int valor)
{
    double valorf = (double)valor / 10;
    ui->label_valortraind->setText(QString::number(valorf, 'f', 1));
}

void MainWindow::texttrainp(int valor)
{
    double valorf = (double)valor / 10;
    ui->label_valortrainp->setText(QString::number(valorf, 'f', 1));
}

void MainWindow::textpulsed(int valor)
{
    double valorf = (double)valor / 10;
    ui->label_valorpulsed->setText(QString::number(valorf, 'f', 1));
}

void MainWindow::Leerinfo()
{
    /*int hola = serial->bytesAvailable();
    QString hola2 = QString(hola);
    emit mensajito(hola2);*/

    /*if(serial->bytesAvailable() > 0)
    {
        QByteArray recibir2 = serial->readAll();
        QString hola = recibir2;
        QMessageBox::information(this, "Hola voy por aqui", hola);
    }*/

    //QString hola = "Si mando datos";
    //QMessageBox::information(this, "Hola voy por aqui", hola);

    while(serial->bytesAvailable() < CAPACIDAD_BLOQUE)
    {
        QCoreApplication::processEvents();
    }
    if (serial->bytesAvailable() >0)
    {
        QByteArray recibir = serial->read(CAPACIDAD_BLOQUE);

        for (int i = 0; i < (CAPACIDAD_BLOQUE / 2); i++)
        {
            uint8_t datoL = recibir[(2 * i)];
            uint8_t datoH = recibir[((2*i)+1)];
            int datoLint = datoL;
            int datoHint = datoH;
            datoHint = datoHint << 8;

            numerillo++;

            datosY << (double)(datoHint | datoLint);
            datosX << (double)numerillo;
        }

        ui->widget->graph(0)->addData(datosX, datosY);
        ui->widget->graph(1)->clearData();
        ui->widget->graph(1)->addData(datosX.last(), datosY.last());

        ui->widget->xAxis->setRange(datosX.last() + 2, 8, Qt::AlignRight);
        ui->widget->replot();
        if(tiempo == 1)
        {
            ui->widget->replot();
            tiempo = 0;
        }
    }

    /*while(serial->bytesAvailable() < 3)
    {
        QCoreApplication::processEvents();
    }
    if (serial->bytesAvailable() >0)
    {
        QByteArray recibir = serial->read(3);
        QString mensaje = recibir;
        bool ok;
        int ari = mensaje.toInt(&ok, 16);

        emit mostrartiemporeal(ari);
    }*/






        //io << ari << "\n";
}


void MainWindow::status_changer()
{
    if (ui->verticalSlider_status->value() == 1)
    {
        statusestimul = "a";
        desconexiones_extra();
        emit estimular_y_recibir(statusestimul, statusrecep);
    }
    else
    {
        statusestimul = "e";
        conexiones_extra();
        emit estimular_y_recibir(statusestimul, statusrecep);
    }
}


void MainWindow::EmpezarEstimulacion(QString &stimuli, QString &recept)
{
    Valores v;
    establecer_valores(v);

    /*
     * Mi protocolo
     * 1.   Prender y apagar el Estimulador
     *      Apagar/Encender modo pulso/Encender modo tren
     *      Tamaño: 1 Character -> 1 Byte
     *      Apagado = 'a'
     *      Encendido modo pulso continuo= 'p'
     *      Encendido modo pulso single pulse = 's'
     *      Encendido modo tren continuo= 't'
     *      Encendido modo tren single pulse = 'u'
     *
     * 2.   Prender y apagar el Receptor
     *      Tamaño: 1 Character - 1 Byte
     *      Apagado = 'a'
     *      Encendido = 'e'
     *
     * Para el resto de los datos
     * n.   COMO MANDAR CADA UNO DE LOS DATOS, n>2
     *      Tamaño: 2 Bytes, valor del Dial en hexadecimal
     *              1 Byte, valor de la potencia a multiplicar el valor  y sumarle lo adecuado
     *                  para tener los datos en microsegundos
    */

    // Status de la estimulacion
    QByteArray datafinal;

    if (stimuli == "a")
    {
        if(recept == "a")
        {
            datafinal = QByteArray("aa000000000000000");
        }
        else
        {
            datafinal = QByteArray("ae000000000000000");
        }
    }
    else
    {
        if (ui->verticalSlider_trainorsinglepulse->value() == 0)
        {
            if(ui->verticalSlider_status->value()== 0)
            {
                datafinal = QByteArray("s");
            }
            else
            {
                datafinal = QByteArray("p");
            }
        }
        else
        {
            if(ui->verticalSlider_status->value() == 0)
            {
                datafinal = QByteArray("u");
            }
            else
            {
                datafinal = QByteArray("t");
            }
        }
        //Status de la recepcion
        QByteArray recepcion;
        if(recept == "a")
        {
            recepcion = QByteArray("a");
        }
        else
        {
            recepcion = QByteArray("e");
        }
        datafinal += recepcion;

        // Mando info del Dial Master Period
        QByteArray sendmastp1 = QByteArray::number(v.masterperiod1, 16);
        if (sendmastp1.size() == 1)
        {
            sendmastp1.prepend("0");
        }
        datafinal += sendmastp1;
        QByteArray sendmastp2 = QByteArray::number(v.masterperiodtotal);
        datafinal += sendmastp2;

        //Mando info del Dial Delay
        QByteArray senddelay1;
        if (v.delay2 == 0)
        {
            senddelay1 = QByteArray("00");
        }
        else
        {
            senddelay1 = QByteArray::number(v.delay1, 16);
            if (senddelay1.size() == 1)
            {
                senddelay1.prepend("0");
            }
        }
        datafinal += senddelay1;
        QByteArray senddelay2 = QByteArray::number(v.delaytotal);
        datafinal += senddelay2;

        // Mando info del Dial Train Duration
        QByteArray sendtraind1 = QByteArray::number(v.trainduration1, 16);
        if (sendtraind1.size() == 1)
        {
            sendtraind1.prepend("0");
        }
        datafinal += sendtraind1;
        QByteArray sendtraind2 = QByteArray::number(v.traindurationtotal);
        datafinal += sendtraind2;

        // Mando info del Dial Train Period
        QByteArray sendtrainp1 = QByteArray::number(v.trainperiod1, 16);
        if (sendtrainp1.size() == 1)
        {
            sendtrainp1.prepend("0");
        }
        datafinal += sendtrainp1;
        QByteArray sendtrainp2 = QByteArray::number(v.trainperiodtotal);
        datafinal += sendtrainp2;

        // Mando info del Dial Pulse Duration
        QByteArray sendpulsed1 = QByteArray::number(v.pulseduration1, 16);
        if (sendpulsed1.size() == 1)
        {
            sendpulsed1.prepend("0");
        }
        datafinal += sendpulsed1;
        QByteArray sendpulsed2 = QByteArray::number(v.pulsedurationtotal);
        datafinal += sendpulsed2;
    }

    QString mensajito = datafinal;
    QMessageBox::information(this, "mensaje", mensajito);



    serial->write(datafinal);

}

void MainWindow::establecer_valores(MainWindow::Valores &v)
{
    v.pulseduration1 = ui->dial_pulsed_1->value();
    v.pulseduration2 = ui->dial_pulsed_2->value();
    v.masterperiod1 = ui->dial_masterp_1->value();
    v.masterperiod2 = ui->dial_masterp_2->value();
    v.delay1 = ui->dial_delay_1->value();
    v.delay2 = ui->dial_delay_2->value();
    v.trainduration1 = ui->dial_traind_1->value();
    v.trainduration2 = ui->dial_traind_2->value();
    v.trainperiod1 = ui->dial_trainp_1->value();
    v.trainperiod2 = ui->dial_trainp_2->value();

    v.masterperiodtotal = v.masterperiod2 + 3;
    if (v.delay2 == 0)
    {
        v.delaytotal = 0;
    }
    else
    {
        v.delaytotal = v.delay2 + 2;
    }
    v.traindurationtotal = v.trainduration2 + 3;
    v.trainperiodtotal = v.trainperiod2 + 3;
    v.pulsedurationtotal = v.pulseduration2 + 3;
}

void MainWindow::conexiones_extra()
{
    connect(ui->dial_masterp_1, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    connect(ui->dial_masterp_2, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    connect(ui->dial_delay_1, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    connect(ui->dial_delay_2, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    connect(ui->dial_traind_1, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    connect(ui->dial_traind_2, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    connect(ui->dial_trainp_1, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    connect(ui->dial_trainp_2, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    connect(ui->dial_pulsed_1, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    connect(ui->dial_pulsed_2, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    connect(ui->verticalSlider_trainorsinglepulse, SIGNAL(valueChanged(int)), this, SLOT(estimulacion_extra()));
}

void MainWindow::desconexiones_extra()
{
    disconnect(ui->dial_masterp_1, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    disconnect(ui->dial_masterp_2, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    disconnect(ui->dial_delay_1, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    disconnect(ui->dial_delay_2, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    disconnect(ui->dial_traind_1, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    disconnect(ui->dial_traind_2, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    disconnect(ui->dial_trainp_1, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    disconnect(ui->dial_trainp_2, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    disconnect(ui->dial_pulsed_1, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    disconnect(ui->dial_pulsed_2, SIGNAL(sliderReleased()), this, SLOT(estimulacion_extra()));
    disconnect(ui->verticalSlider_trainorsinglepulse, SIGNAL(valueChanged(int)), this, SLOT(estimulacion_extra()));
}

void MainWindow::estimulacion_extra()
{
    emit estimular_y_recibir(statusestimul, statusrecep);
}

void MainWindow::permitir_uso()
{
    ui->verticalSlider_status->setEnabled(true);
    ui->label_on->setEnabled(true);
    ui->label_standby->setEnabled(true);
    ui->label_single->setEnabled(true);
    ui->label_cycle->setEnabled(true);
    ui->label_masterperiod->setEnabled(true);
    ui->label_miliseconds_1->setEnabled(true);
    ui->dial_masterp_1->setEnabled(true);
    ui->label_dial_masterp1_0->setEnabled(true);
    ui->label_dial_masterp1_1->setEnabled(true);
    ui->label_dial_masterp1_2->setEnabled(true);
    ui->label_dial_masterp1_3->setEnabled(true);
    ui->label_dial_masterp1_4->setEnabled(true);
    ui->label_dial_masterp1_5->setEnabled(true);
    ui->label_dial_masterp1_6->setEnabled(true);
    ui->label_dial_masterp1_7->setEnabled(true);
    ui->label_dial_masterp1_8->setEnabled(true);
    ui->label_dial_masterp1_9->setEnabled(true);
    ui->label_dial_masterp1_10->setEnabled(true);
    ui->label_dial_masterp1_11->setEnabled(true);
    ui->dial_masterp_2->setEnabled(true);
    ui->label_dial_masterp2_1->setEnabled(true);
    ui->label_dial_masterp2_10->setEnabled(true);
    ui->label_dial_masterp2_100->setEnabled(true);
    ui->label_dial_masterp2_1k->setEnabled(true);
    ui->label_dial_masterp2_10k->setEnabled(true);
    ui->label_delay->setEnabled(true);
    ui->label_miliseconds_2->setEnabled(true);
    ui->dial_delay_1->setEnabled(true);
    ui->label_dial_delay1_0->setEnabled(true);
    ui->label_dial_delay1_1->setEnabled(true);
    ui->label_dial_delay1_2->setEnabled(true);
    ui->label_dial_delay1_3->setEnabled(true);
    ui->label_dial_delay1_4->setEnabled(true);
    ui->label_dial_delay1_5->setEnabled(true);
    ui->label_dial_delay1_6->setEnabled(true);
    ui->label_dial_delay1_7->setEnabled(true);
    ui->label_dial_delay1_8->setEnabled(true);
    ui->label_dial_delay1_9->setEnabled(true);
    ui->label_dial_delay1_10->setEnabled(true);
    ui->label_dial_delay1_11->setEnabled(true);
    ui->dial_delay_2->setEnabled(true);
    ui->label_dial_delay2_0->setEnabled(true);
    ui->label_dial_delay2_1->setEnabled(true);
    ui->label_dial_delay2_10->setEnabled(true);
    ui->label_dial_delay2_100->setEnabled(true);
    ui->label_dial_delay2_1k->setEnabled(true);
    ui->verticalSlider_trainorsinglepulse->setEnabled(true);
    ui->label_train->setEnabled(true);
    ui->label_single_2->setEnabled(true);
    ui->label_pulse->setEnabled(true);
    ui->label_pulseduration->setEnabled(true);
    ui->label_miliseconds_5->setEnabled(true);
    ui->dial_pulsed_1->setEnabled(true);
    ui->label_dial_pulsed1_0->setEnabled(true);
    ui->label_dial_pulsed1_1->setEnabled(true);
    ui->label_dial_pulsed1_2->setEnabled(true);
    ui->label_dial_pulsed1_3->setEnabled(true);
    ui->label_dial_pulsed1_4->setEnabled(true);
    ui->label_dial_pulsed1_5->setEnabled(true);
    ui->label_dial_pulsed1_6->setEnabled(true);
    ui->label_dial_pulsed1_7->setEnabled(true);
    ui->label_dial_pulsed1_8->setEnabled(true);
    ui->label_dial_pulsed1_9->setEnabled(true);
    ui->label_dial_pulsed1_10->setEnabled(true);
    ui->label_dial_pulsed1_11->setEnabled(true);
    ui->dial_pulsed_2->setEnabled(true);
    ui->label_dial_pulsed2_1->setEnabled(true);
    ui->label_dial_pulsed2_10->setEnabled(true);
    ui->label_dial_pulsed2_100->setEnabled(true);
    ui->label_dial_pulsed2_1k->setEnabled(true);
    ui->label_valormasterp->setEnabled(true);
    ui->label_valordelay->setEnabled(true);
    ui->label_valorpulsed->setEnabled(true);
}

void MainWindow::no_permitir_uso()
{
    ui->verticalSlider_status->setEnabled(false);
    ui->label_on->setEnabled(false);
    ui->label_standby->setEnabled(false);
    ui->label_single->setEnabled(false);
    ui->label_cycle->setEnabled(false);
    ui->label_masterperiod->setEnabled(false);
    ui->label_miliseconds_1->setEnabled(false);
    ui->dial_masterp_1->setEnabled(false);
    ui->label_dial_masterp1_0->setEnabled(false);
    ui->label_dial_masterp1_1->setEnabled(false);
    ui->label_dial_masterp1_2->setEnabled(false);
    ui->label_dial_masterp1_3->setEnabled(false);
    ui->label_dial_masterp1_4->setEnabled(false);
    ui->label_dial_masterp1_5->setEnabled(false);
    ui->label_dial_masterp1_6->setEnabled(false);
    ui->label_dial_masterp1_7->setEnabled(false);
    ui->label_dial_masterp1_8->setEnabled(false);
    ui->label_dial_masterp1_9->setEnabled(false);
    ui->label_dial_masterp1_10->setEnabled(false);
    ui->label_dial_masterp1_11->setEnabled(false);
    ui->dial_masterp_2->setEnabled(false);
    ui->label_dial_masterp2_1->setEnabled(false);
    ui->label_dial_masterp2_10->setEnabled(false);
    ui->label_dial_masterp2_100->setEnabled(false);
    ui->label_dial_masterp2_1k->setEnabled(false);
    ui->label_dial_masterp2_10k->setEnabled(false);
    ui->label_delay->setEnabled(false);
    ui->label_miliseconds_2->setEnabled(false);
    ui->dial_delay_1->setEnabled(false);
    ui->label_dial_delay1_0->setEnabled(false);
    ui->label_dial_delay1_1->setEnabled(false);
    ui->label_dial_delay1_2->setEnabled(false);
    ui->label_dial_delay1_3->setEnabled(false);
    ui->label_dial_delay1_4->setEnabled(false);
    ui->label_dial_delay1_5->setEnabled(false);
    ui->label_dial_delay1_6->setEnabled(false);
    ui->label_dial_delay1_7->setEnabled(false);
    ui->label_dial_delay1_8->setEnabled(false);
    ui->label_dial_delay1_9->setEnabled(false);
    ui->label_dial_delay1_10->setEnabled(false);
    ui->label_dial_delay1_11->setEnabled(false);
    ui->dial_delay_2->setEnabled(false);
    ui->label_dial_delay2_0->setEnabled(false);
    ui->label_dial_delay2_1->setEnabled(false);
    ui->label_dial_delay2_10->setEnabled(false);
    ui->label_dial_delay2_100->setEnabled(false);
    ui->label_dial_delay2_1k->setEnabled(false);
    ui->verticalSlider_trainorsinglepulse->setEnabled(false);
    ui->label_train->setEnabled(false);
    ui->label_single_2->setEnabled(false);
    ui->label_pulse->setEnabled(false);
    ui->label_pulseduration->setEnabled(false);
    ui->label_miliseconds_5->setEnabled(false);
    ui->dial_pulsed_1->setEnabled(false);
    ui->label_dial_pulsed1_0->setEnabled(false);
    ui->label_dial_pulsed1_1->setEnabled(false);
    ui->label_dial_pulsed1_2->setEnabled(false);
    ui->label_dial_pulsed1_3->setEnabled(false);
    ui->label_dial_pulsed1_4->setEnabled(false);
    ui->label_dial_pulsed1_5->setEnabled(false);
    ui->label_dial_pulsed1_6->setEnabled(false);
    ui->label_dial_pulsed1_7->setEnabled(false);
    ui->label_dial_pulsed1_8->setEnabled(false);
    ui->label_dial_pulsed1_9->setEnabled(false);
    ui->label_dial_pulsed1_10->setEnabled(false);
    ui->label_dial_pulsed1_11->setEnabled(false);
    ui->dial_pulsed_2->setEnabled(false);
    ui->label_dial_pulsed2_1->setEnabled(false);
    ui->label_dial_pulsed2_10->setEnabled(false);
    ui->label_dial_pulsed2_100->setEnabled(false);
    ui->label_dial_pulsed2_1k->setEnabled(false);
    ui->label_valormasterp->setEnabled(false);
    ui->label_valordelay->setEnabled(false);
    ui->label_valorpulsed->setEnabled(false);

    no_permitir_uso_train();

}

void MainWindow::permitir_uso_train()
{
    ui->label_trainduration->setEnabled(true);
    ui->label_miliseconds_3->setEnabled(true);
    ui->dial_traind_1->setEnabled(true);
    ui->label_dial_traind1_0->setEnabled(true);
    ui->label_dial_traind1_1->setEnabled(true);
    ui->label_dial_traind1_2->setEnabled(true);
    ui->label_dial_traind1_3->setEnabled(true);
    ui->label_dial_traind1_4->setEnabled(true);
    ui->label_dial_traind1_5->setEnabled(true);
    ui->label_dial_traind1_6->setEnabled(true);
    ui->label_dial_traind1_7->setEnabled(true);
    ui->label_dial_traind1_8->setEnabled(true);
    ui->label_dial_traind1_9->setEnabled(true);
    ui->label_dial_traind1_10->setEnabled(true);
    ui->label_dial_traind1_11->setEnabled(true);
    ui->dial_traind_2->setEnabled(true);
    ui->label_dial_traind2_1->setEnabled(true);
    ui->label_dial_traind2_10->setEnabled(true);
    ui->label_dial_traind2_100->setEnabled(true);
    ui->label_dial_traind2_1k->setEnabled(true);
    ui->label_trainperiod->setEnabled(true);
    ui->label_miliseconds_4->setEnabled(true);
    ui->dial_trainp_1->setEnabled(true);
    ui->label_dial_trainp1_0->setEnabled(true);
    ui->label_dial_trainp1_1->setEnabled(true);
    ui->label_dial_trainp1_2->setEnabled(true);
    ui->label_dial_trainp1_3->setEnabled(true);
    ui->label_dial_trainp1_4->setEnabled(true);
    ui->label_dial_trainp1_5->setEnabled(true);
    ui->label_dial_trainp1_6->setEnabled(true);
    ui->label_dial_trainp1_7->setEnabled(true);
    ui->label_dial_trainp1_8->setEnabled(true);
    ui->label_dial_trainp1_9->setEnabled(true);
    ui->label_dial_trainp1_10->setEnabled(true);
    ui->label_dial_trainp1_11->setEnabled(true);
    ui->dial_trainp_2->setEnabled(true);
    ui->label_dial_trainp2_1->setEnabled(true);
    ui->label_dial_trainp2_10->setEnabled(true);
    ui->label_dial_trainp2_100->setEnabled(true);
    ui->label_valortraind->setEnabled(true);
    ui->label_valortrainp->setEnabled(true);

    ui->line_train_1->setPalette(p2);
    ui->line_train_2->setPalette(p2);
    ui->line_train_3->setPalette(p2);
    ui->line_singlepulse_1->setPalette(p1);
    ui->line_singlepulse_2->setPalette(p1);
}

void MainWindow::no_permitir_uso_train()
{
    ui->label_trainduration->setEnabled(false);
    ui->label_miliseconds_3->setEnabled(false);
    ui->dial_traind_1->setEnabled(false);
    ui->label_dial_traind1_0->setEnabled(false);
    ui->label_dial_traind1_1->setEnabled(false);
    ui->label_dial_traind1_2->setEnabled(false);
    ui->label_dial_traind1_3->setEnabled(false);
    ui->label_dial_traind1_4->setEnabled(false);
    ui->label_dial_traind1_5->setEnabled(false);
    ui->label_dial_traind1_6->setEnabled(false);
    ui->label_dial_traind1_7->setEnabled(false);
    ui->label_dial_traind1_8->setEnabled(false);
    ui->label_dial_traind1_9->setEnabled(false);
    ui->label_dial_traind1_10->setEnabled(false);
    ui->label_dial_traind1_11->setEnabled(false);
    ui->dial_traind_2->setEnabled(false);
    ui->label_dial_traind2_1->setEnabled(false);
    ui->label_dial_traind2_10->setEnabled(false);
    ui->label_dial_traind2_100->setEnabled(false);
    ui->label_dial_traind2_1k->setEnabled(false);
    ui->label_trainperiod->setEnabled(false);
    ui->label_miliseconds_4->setEnabled(false);
    ui->dial_trainp_1->setEnabled(false);
    ui->label_dial_trainp1_0->setEnabled(false);
    ui->label_dial_trainp1_1->setEnabled(false);
    ui->label_dial_trainp1_2->setEnabled(false);
    ui->label_dial_trainp1_3->setEnabled(false);
    ui->label_dial_trainp1_4->setEnabled(false);
    ui->label_dial_trainp1_5->setEnabled(false);
    ui->label_dial_trainp1_6->setEnabled(false);
    ui->label_dial_trainp1_7->setEnabled(false);
    ui->label_dial_trainp1_8->setEnabled(false);
    ui->label_dial_trainp1_9->setEnabled(false);
    ui->label_dial_trainp1_10->setEnabled(false);
    ui->label_dial_trainp1_11->setEnabled(false);
    ui->dial_trainp_2->setEnabled(false);
    ui->label_dial_trainp2_1->setEnabled(false);
    ui->label_dial_trainp2_10->setEnabled(false);
    ui->label_dial_trainp2_100->setEnabled(false);
    ui->label_valortraind->setEnabled(false);
    ui->label_valortrainp->setEnabled(false);

    ui->line_train_1->setPalette(p1);
    ui->line_train_2->setPalette(p1);
    ui->line_train_3->setPalette(p1);
    ui->line_singlepulse_1->setPalette(p2);
    ui->line_singlepulse_2->setPalette(p2);
}




void MainWindow::on_actionSave_Data_on_File_triggered()
{
    nombreArch = QFileDialog::getSaveFileName(this, "Save as",".", "Text Files (*.text)");
    if(nombreArch.isEmpty())
    {
        return;
    }
    arch.setFileName(nombreArch);
    arch.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!arch.isOpen())
    {
        QMessageBox::critical(this, "Error", arch.errorString());
    }
    io.setDevice(&arch);
    io << "Registro tomado con Estimulino\n\n\n";

    statusrecep = "e";
    emit estimular_y_recibir(statusestimul, statusrecep);
    //recepcion->show();


}

void MainWindow::on_actionStop_Data_Saving_triggered()
{
    arch.flush();
    arch.close();
    statusrecep = "a";
    emit estimular_y_recibir(statusestimul, statusrecep);

}

void MainWindow::on_actionStimmulation_Graphics_triggered()
{

}

void MainWindow::la_respuesta_mas_nula()
{
    numerillo = 0;
}

void MainWindow::on_pushButton_view_clicked()
{
    statusrecep = "e";
    emit estimular_y_recibir(statusestimul, statusrecep);
}

void MainWindow::on_pushButton_stop_clicked()
{
    statusrecep = "a";
    emit estimular_y_recibir(statusestimul, statusrecep);
}

void MainWindow::on_pushButton_reset_clicked()
{
    ui->widget->graph(0)->clearData();
    ui->widget->graph(1)->clearData();
    ui->widget->yAxis->setRange(-1, 1050);
    ui->widget->replot();
    numerillo = 0;
}

void MainWindow::on_pushButton_savedata_clicked()
{

}
