#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "packets.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _server(this)
{
    ui->setupUi(this);
    this->readBoxDestinations(filename1, &(boxes[0].box_dst_len), &(boxes[0].box_dst));
    this->readBoxDestinations(filename2, &(boxes[1].box_dst_len), &(boxes[1].box_dst));

    for (int i = 0; i < boxes[0].box_dst_len; i++) {
        std::cout << boxes[0].box_dst[i] << std::endl;
    }

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
    std::string data = sender->readAll().toStdString();
    const char* packet = data.c_str();

    char packet_code = C2S_get_packet_code(packet);
    switch (packet_code) {
        case C2S_BOOLS_TRAP_CODE: {
            C2S_parse_bools_trap(packet, &plc_bool);
            break;
        }
        case C2S_SHORTS_TRAP_CODE: {
            C2S_parse_shorts_trap(packet, &plc_number);
            break;
        }
        case C2S_BOX_POSITION_REQUEST_CODE: {
            int clientIndex = _sockets.indexOf(sender);
            BoxDst b = boxes[clientIndex];

            short dst;
            if (b.box_count >= b.box_dst_len) dst = -1;
            else {
                dst = (short) b.box_dst[b.box_count];
                boxes[clientIndex].box_count++;
            }

            char* response_packet = craft_packet(S2C_BOX_POSITION_RESPONSE_CODE);
            S2C_box_position_response(response_packet, dst);

            sender->write(response_packet, S2C_BOX_POSITION_RESPONSE_LEN);
            sender->flush();

            delete[] response_packet;
            break;
        }
        default: {
            std::cout << "Unknown packet code" << std::endl;
            break;
        }
    }
}

void MainWindow::readBoxDestinations(const char* filename, short* box_dst_len, short** box_dst) {
    FILE* fid = fopen(filename, "r");
    char buf[16];

    while (fgets(buf, sizeof(buf), fid) != NULL) (*box_dst_len)++;
    std::fseek(fid, 0, SEEK_SET);

    (*box_dst) = new short[*box_dst_len];
    for (int i = 0; i < *box_dst_len; i++) {
        fgets(buf, 16, fid);
        sscanf(buf, "%hd\n", &((*box_dst)[i]));
    }
}
