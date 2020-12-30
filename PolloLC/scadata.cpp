#include <QHostAddress>
#include "scadata.h"
#include <iostream>
#include "packets.h"

Scadata::Scadata(PLCData* plcd_ptr) : cajaCheckTimer(new QTimer(this))
{
    plcd = plcd_ptr;
    connect(cajaCheckTimer, SIGNAL(timeout()), this, SLOT(onCheck()));

    socket.connectToHost(QHostAddress(SERVER_HOST), SERVER_PORT);
    connect(&socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}


Scadata::~Scadata() {};

void Scadata::tmp_onCheck(void) { this->onCheck(); }
void Scadata::onCheck(void) {
//    if (this->shouldRequestPosition()) {
        requestNextPosition();
//    }
}

bool Scadata::shouldRequestPosition(void) {
//    bool newS0State = this->plcd->getBoolAt(0);
//    bool should = (newS0State && (newS0State == this->lastS0State));
//    this->lastS0State = newS0State;
//    return should;
    return true;
}

void Scadata::requestNextPosition(void) {
//    socket.write(C2S_REQUEST_BOX_POSITION, C2S_REQUEST_BOX_POSITION_LEN);
}

void Scadata::onReadyRead()
{
    QByteArray data = socket.readAll();
    std::string msg = data.toStdString();
    const char* msgg = msg.c_str();

    std::cout << "Received (" << strlen(msgg) << " bytes) : " << msgg << std::endl;

    //if (where_caja_response) plcd.updateBoxLocation(response.location);
}
