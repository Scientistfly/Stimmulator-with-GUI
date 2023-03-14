#ifndef SHOWLIVERECEPTION_H
#define SHOWLIVERECEPTION_H

#include <QMainWindow>

namespace Ui {
class ShowLiveReception;
}

class ShowLiveReception : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShowLiveReception(QWidget *parent = 0);
    ~ShowLiveReception();

    int sample;
    QString statusrecep;
    QString statusestimul;
    QTimer *datatiempo;
    int tiempo;

signals:
     void estimular_y_recibir3(QString &stimuli, QString &recept);
     void la_senal_mas_nula();

private slots:
    void mostrandotiemporeal(int &numero);

    void mostrandotiemporeal2(QVector<double> &datillos, QVector<double> &datillos2);

    void pantallazo();

    void on_stop_pushButton_clicked();

    void on_start_pushButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::ShowLiveReception *ui;
};

#endif // SHOWLIVERECEPTION_H
