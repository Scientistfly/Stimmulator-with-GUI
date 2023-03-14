#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define CAPACIDAD_BLOQUE 2

#include <QMainWindow>
#include <QSerialPort>
#include <QFile>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    class Dialog *ventanapuerto;
    class Recepcion *recepcion;
    QSerialPort *serial;
    class ShowLiveReception *livereception;


    struct Valores
    {
        int pulseduration1;
        int pulseduration2;
        int delay1;
        int delay2;
        int masterperiod1;
        int masterperiod2;
        int trainduration1;
        int trainduration2;
        int trainperiod1;
        int trainperiod2;
        int pulsedurationtotal;
        int delaytotal;
        int masterperiodtotal;
        int traindurationtotal;
        int trainperiodtotal;
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString statusrecep;
    QString statusestimul;
    QFile arch;
    QTextStream io;
    QString nombreArch;
    QVector<double> datosY;
    QVector<double> datosX;
    int numerillo;
    QPalette p1;
    QPalette p2;
    QTimer *datatiempo;
    int tiempo;
    int sample;



signals:
    void estimular_y_recibir(QString &stimuli, QString &recept);
    void mensajito(QString &meme);
    void mostrartiemporeal(int &numero);
    void mostrartiemporeal2(QVector<double> &datillo, QVector<double> &datillo2);

private slots:
    void on_actionConnect_triggered();
    void on_actionReception_triggered();
    void enciende_puerto_serial(QString nombrepuerto3);
    void mostrar_train();
    void on_actionDisconnect_triggered();
    void EmpezarEstimulacion(QString &stimuli, QString &recept);
    void status_changer();
    void estimulacion_extra();

    void textmasterp(int valor);
    void textdelay(int valor);
    void texttraind(int valor);
    void texttrainp(int valor);
    void textpulsed(int valor);
    void Leerinfo();
    void on_actionSave_Data_on_File_triggered();
    void on_actionStop_Data_Saving_triggered();
    void on_actionStimmulation_Graphics_triggered();
    void la_respuesta_mas_nula();
    void pantallazo();

    void on_pushButton_view_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_reset_clicked();

    void on_pushButton_savedata_clicked();

private:
    Ui::MainWindow *ui;
    void permitir_uso();
    void no_permitir_uso();
    void connections();
    void permitir_uso_train();
    void no_permitir_uso_train();
    void establecer_valores(Valores &v);
    void conexiones_extra();
    void desconexiones_extra();
    void apagar_todo();
    void inicioparapintar();

};

#endif // MAINWINDOW_H
