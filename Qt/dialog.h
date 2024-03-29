#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

signals:
    void serial_configurado(QString nombrepuerto2);

private slots:
    void on_pushButton_connect_clicked();

    void on_pushButton_cancel_clicked();

    void poninfo(int num);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
