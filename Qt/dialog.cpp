#include "dialog.h"
#include "ui_dialog.h"
#include <mainwindow.h>
#include <QSerialPortInfo>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox_port->addItem(info.portName());
    }
    ui->comboBox_port->setEditable(true);                   //para que el usuario pueda poner el nombre que desee

    ui->comboBox_baudrate->addItem("1200");
    ui->comboBox_baudrate->addItem("2400");
    ui->comboBox_baudrate->addItem("4800");
    ui->comboBox_baudrate->addItem("9600");
    ui->comboBox_baudrate->addItem("19200");
    ui->comboBox_baudrate->addItem("57600");
    ui->comboBox_baudrate->addItem("115200");
    ui->comboBox_baudrate->setCurrentIndex(6);
    ui->comboBox_baudrate->setEnabled(false);

    ui->comboBox_parity->addItem("NONE");
    ui->comboBox_parity->addItem("ODD");
    ui->comboBox_parity->addItem("EVEN");
    ui->comboBox_parity->setEnabled(false);

    ui->comboBox_databits->addItem("5");
    ui->comboBox_databits->addItem("6");
    ui->comboBox_databits->addItem("7");
    ui->comboBox_databits->addItem("8");
    ui->comboBox_databits->setCurrentIndex(3);
    ui->comboBox_databits->setEnabled(false);

    ui->comboBox_stopbits->addItem("1");
    ui->comboBox_stopbits->addItem("2");
    ui->comboBox_stopbits->setEnabled(false);

    connect(ui->comboBox_port, SIGNAL(activated(int)), this, SLOT(poninfo(int)));

    if (!QSerialPortInfo::availablePorts().isEmpty())
    {
        ui->plainTextEdit->clear();
        QSerialPortInfo info2 = QSerialPortInfo::availablePorts().at(0);
        QString s = QObject::tr("Port: ") + info2.portName() + "\n"
                    + QObject::tr("Location: ") + info2.systemLocation() + "\n"
                    + QObject::tr("Description: ") + info2.description() + "\n"
                    + QObject::tr("Manufacturer: ") + info2.manufacturer() + "\n"
                    + QObject::tr("Vendor Identifier: ") + (info2.hasVendorIdentifier() ? QString::number(info2.vendorIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Product Identifier: ") + (info2.hasProductIdentifier() ? QString::number(info2.productIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Busy: ") + (info2.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
        ui->plainTextEdit->insertPlainText(s);
    }






}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_connect_clicked()
{
    int numerito = ui->comboBox_port->currentIndex();
    QSerialPortInfo info3 = QSerialPortInfo::availablePorts().at(numerito);
    QString nombrepuerto = info3.portName();
    emit serial_configurado(nombrepuerto);
    close();
}

void Dialog::on_pushButton_cancel_clicked()
{
    close();
}

void Dialog::poninfo(int num)
{
    if (!QSerialPortInfo::availablePorts().isEmpty())
    {
        ui->plainTextEdit->clear();
        QSerialPortInfo info2 = QSerialPortInfo::availablePorts().at(num);
        QString s = QObject::tr("Port: ") + info2.portName() + "\n"
                    + QObject::tr("Location: ") + info2.systemLocation() + "\n"
                    + QObject::tr("Description: ") + info2.description() + "\n"
                    + QObject::tr("Manufacturer: ") + info2.manufacturer() + "\n"
                    + QObject::tr("Vendor Identifier: ") + (info2.hasVendorIdentifier() ? QString::number(info2.vendorIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Product Identifier: ") + (info2.hasProductIdentifier() ? QString::number(info2.productIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Busy: ") + (info2.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
        ui->plainTextEdit->insertPlainText(s);
    }
}
