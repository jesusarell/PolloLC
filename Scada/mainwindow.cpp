#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "packets.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _server(this)
{
    PLC_constants[0].TSensorMuelle = 750;       PLC_constants[1].TSensorMuelle = 750;
    PLC_constants[0].TSensorCinta = 750;        PLC_constants[1].TSensorCinta = 750;
    PLC_constants[0].TCinta0 = 500;             PLC_constants[1].TCinta0 = 500;
    PLC_constants[0].TCintas = 1500;            PLC_constants[1].TCintas = 1500;
    PLC_constants[0].TSalidaCilindro = 500;     PLC_constants[1].TSalidaCilindro = 500;
    PLC_constants[0].TRecogerCilindro = 500;    PLC_constants[1].TRecogerCilindro = 500;
    PLC_constants[0].TCaidaCaja = 750;          PLC_constants[1].TCaidaCaja = 750;
    PLC_constants[0].CapacidadMuelle = 25;      PLC_constants[1].CapacidadMuelle = 10;


    ui->setupUi(this);

    for (int i = 0; i < numNaves; i++) {
        this->readBoxDestinations(filenames[i], &(naves[i].box_dst_len), &(naves[i].box_dst));

       for (char j = 0; j < 33; j++) {
            naves[i].plc_bool.insert(std::make_pair(j, false));
        }

        for (char j = 0; j < 4; j++) {
            naves[i].plc_number.insert(std::make_pair(j, 0));
        }
    }
    this->updateUI();

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
    int clientIndex = _sockets.indexOf(sender);

    QByteArray received_packet = sender->readAll();
    int s = received_packet.size();

    while (s > 0) {
        QByteArray packet = received_packet.right(s);
        char packet_code = C2S_get_packet_code(packet);
        switch (packet_code) {
            case C2S_BOOLS_TRAP_CODE: {
                s -= C2S_BOOLS_TRAP_LEN;
                C2S_parse_bools_trap(packet, &(naves[clientIndex].plc_bool));
                break;
            }
            case C2S_SHORTS_TRAP_CODE: {
                s -= C2S_SHORTS_TRAP_LEN;
                C2S_parse_shorts_trap(packet, &(naves[clientIndex].plc_number));
                break;
            }
            case C2S_BOX_POSITION_REQUEST_CODE: {
                s -= C2S_BOX_POSITION_REQUEST_LEN;
                short dst;
                if (naves[clientIndex].box_count >= naves[clientIndex].box_dst_len) dst = -1;
                else {
                    dst = naves[clientIndex].box_dst[naves[clientIndex].box_count];
                    naves[clientIndex].box_count++;
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
    this->updateUI();
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

void MainWindow::updateUI(void) {
    this->updateUI_1();
    this->updateUI_23();
}
void MainWindow::updateUI_1(void) {
    for (int i = 0; i < this->numNaves; i++) {
        this->ui->Resumen->findChild<QLabel*>(QString("n%1_ncR").arg(QString::number(i + 1)))->setText(QString("%1/%2").arg(
            QString::number(naves[i].box_dst_len - naves[i].box_count),
            QString::number(naves[i].box_dst_len)
        ));
        this->ui->Resumen->findChild<QLabel*>(QString("n%1_ncM1").arg(QString::number(i + 1)))->setText(QString("%1/%2").arg(
            QString::number(naves[i].plc_number.at(0)),
            QString::number(PLC_constants[i].CapacidadMuelle)
        ));
        this->ui->Resumen->findChild<QLabel*>(QString("n%1_ncM2").arg(QString::number(i + 1)))->setText(QString("%1/%2").arg(
            QString::number(naves[i].plc_number.at(1)),
            QString::number(PLC_constants[i].CapacidadMuelle)
        ));
        this->ui->Resumen->findChild<QLabel*>(QString("n%1_ncM3").arg(QString::number(i + 1)))->setText(QString("%1/%2").arg(
            QString::number(naves[i].plc_number.at(2)),
            QString::number(PLC_constants[i].CapacidadMuelle)
        ));
        this->ui->Resumen->findChild<QLabel*>(QString("n%1_ncM4").arg(QString::number(i + 1)))->setText(QString("%1/%2").arg(
            QString::number(naves[i].plc_number.at(3)),
            QString::number(PLC_constants[i].CapacidadMuelle)
        ));
    }
}

void MainWindow::updateUI_23(void) {
    for (int i = 0; i < this->numNaves; i++) {
        QWidget* qw = this->ui->tabWidget->findChild<QWidget*>(QString("n%1_rawVars").arg(QString::number(i + 1)));

        qw->findChild<QFrame*>(QString("n%1_S0").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(0) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_S1cinta").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(1) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_S2cinta").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(2) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_S3cinta").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(3) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_S1buffer").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(4) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_S2buffer").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(5) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_S3buffer").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(6) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_SDbuffer").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(7) ? GLED_ON : GLED_OFF);

        qw->findChild<QFrame*>(QString("n%1_C1e").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(8) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_C2e").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(9) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_C3e").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(10) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_C1r").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(11) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_C2r").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(12) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_C3r").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(13) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_C1f").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(14) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_C2f").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(15) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_C3f").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(16) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_C1d").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(17) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_C2d").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(18) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_C3d").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(19) ? GLED_ON : GLED_OFF);

        qw->findChild<QFrame*>(QString("n%1_M0").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(20) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_M1").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(21) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_M2").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(22) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_M3").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(23) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_M4").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(24) ? GLED_ON : GLED_OFF);

        qw->findChild<QFrame*>(QString("n%1_M1AceptarProximaCaja").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(25) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_M2AceptarProximaCaja").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(26) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_M3AceptarProximaCaja").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(27) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_DestinoCajaConocido").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(28) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_M1Lleno").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(29) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_M2Lleno").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(30) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_M3Lleno").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(31) ? GLED_ON : GLED_OFF);
        qw->findChild<QFrame*>(QString("n%1_MDLleno").arg(QString::number(i + 1)))->setStyleSheet(naves[i].plc_bool.at(32) ? GLED_ON : GLED_OFF);

        qw->findChild<QLabel*>(QString("n%1_CajasMuelle1").arg(QString::number(i + 1)))->setText(QString::number(naves[i].plc_number.at(0)));
        qw->findChild<QLabel*>(QString("n%1_CajasMuelle2").arg(QString::number(i + 1)))->setText(QString::number(naves[i].plc_number.at(1)));
        qw->findChild<QLabel*>(QString("n%1_CajasMuelle3").arg(QString::number(i + 1)))->setText(QString::number(naves[i].plc_number.at(2)));
        qw->findChild<QLabel*>(QString("n%1_CajasMuelleD").arg(QString::number(i + 1)))->setText(QString::number(naves[i].plc_number.at(3)));
    }
}
