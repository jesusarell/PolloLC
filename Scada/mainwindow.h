#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#define SERVER_PORT 53251

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct BoxDst {
    short box_dst_len = 0;  //Numero de lineas del fichero
    short* box_dst;         //Array con los valores
    short box_count = 0;    //Contador del numero de cajas ya enviadas
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

    std::map<char, bool> plc_bool;
    std::map<char, short> plc_number;

    const char* filename1 = "../box_dst_1.txt";
    const char* filename2 = "../box_dst_2.txt";
    BoxDst boxes[2];

public slots:
    void onNewConnection();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();
};
#endif // MAINWINDOW_H
