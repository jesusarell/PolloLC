#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _server(this)
{
    ui->setupUi(this);

    for (char i = 0; i < 33; i++) {
        this->plc_bool.insert(std::make_pair(i, false));
    }

    for (char i = 0; i < 4; i++) {
        this->plc_number.insert(std::make_pair(i, 0));
    }

    _server.listen(QHostAddress::Any, SERVER_PORT);
    connect(&_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewConnection()
{
   QTcpSocket *clientSocket = _server.nextPendingConnection();
   connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
   connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));

   _sockets.push_back(clientSocket);
}

void MainWindow::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        _sockets.removeOne(sender);
    }
}

void MainWindow::onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray data = sender->readAll();
    std::string msg = data.toStdString();
    const char* msgg = msg.c_str();

    std::cout << "Received (" << strlen(msgg) << " bytes) : " << msgg << std::endl;

    const int response_len = 1;
    char* response = new char[response_len];
    for (int i = 0; i < response_len; i++) response[i] = (char) this->ui->spinBox->value();
    sender->write(QByteArray::fromRawData(response, response_len));

    delete[] response;
}
