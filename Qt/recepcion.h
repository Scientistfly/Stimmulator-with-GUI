#ifndef RECEPCION_H
#define RECEPCION_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>

namespace Ui {
class Recepcion;
}

class Recepcion : public QMainWindow
{
    Q_OBJECT

public:
    explicit Recepcion(QWidget *parent = 0);
    ~Recepcion();

    QString statusrecep;
    QString statusestimul;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void recibemensaje(QString &meme);

signals:

    void estimular_y_recibir2(QString &stimuli, QString &recept);

private:
    Ui::Recepcion *ui;
    QGraphicsScene *scene;
    QGraphicsLineItem *verticalline1;
    QGraphicsLineItem *verticalline2;
    QGraphicsLineItem *verticalline3;
    QGraphicsLineItem *verticalline4;
    QGraphicsLineItem *verticalline5;
};

#endif // RECEPCION_H
