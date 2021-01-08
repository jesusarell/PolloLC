#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#define SERVER_PORT 53251
#define GLED_ON "background-color:rgb(0,255,0);border-radius:10;border:2px solid black"
#define GLED_OFF "background-color:rgb(0,0,0);border-radius:10;border:2px solid rgb(0,255,0)"
#define RLED_ON "background-color:rgb(255,0,0);border-radius:10;border:2px solid black"
#define RLED_OFF "background-color:rgb(0,0,0);border-radius:10;border:2px solid rgb(255,0,0)"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct EstadoNave {
    short box_dst_len = 0;  //Numero de lineas del fichero
    short* box_dst;         //Array con los valores
    short box_count = 0;    //Contador del numero de cajas ya enviadas

    std::map<char, bool> plc_bool;      //Variables booleanas de los plcs
    std::map<char, short> plc_number;   //Variables numericas de los plcs
};

struct Visualization_PLC_Constants {
    int TSensorMuelle;
    int TSensorCinta;
    int TCinta0;
    int TCintas;
    int TSalidaCilindro;
    int TRecogerCilindro;
    int TCaidaCaja;
    short CapacidadMuelle;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readBoxDestinations(const char* filename, short* box_dst_cycle_len, short** box_dst_cycle);

private:
    Ui::MainWindow *ui;
    QTcpServer _server;
    QList<QTcpSocket*>  _sockets;

    const int numNaves = 2;
    const char* filenames[2] = {"../box_dst_1.txt", "../box_dst_2.txt"};
    EstadoNave naves[2];
    Visualization_PLC_Constants PLC_constants[2];

    void updateUI(void);
    void updateUI_1(void);
    void updateUI_23(void);
    void updateUI_34(void);

public slots:
    void onNewConnection();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();
};
#endif // MAINWINDOW_H
