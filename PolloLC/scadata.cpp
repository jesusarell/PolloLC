#include <QHostAddress>
#include "scadata.h"
#include <iostream>
#include "packets.h"

Scadata::Scadata(PLCData* plcd_ptr) : cajaCheckTimer(new QTimer(this))
{
    plcd = plcd_ptr;
    connect(cajaCheckTimer, SIGNAL(timeout()), this, SLOT(onCheck()));
    cajaCheckTimer->start(100);

    socket.connectToHost(QHostAddress(SERVER_HOST), SERVER_PORT);
    connect(&socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}


Scadata::~Scadata() {};

//void Scadata::tmp_onCheck(void) { this->onCheck(); }
void Scadata::onCheck(void) {
    if (this->shouldRequestPosition()) {
        requestNextPosition();
    }
}

bool Scadata::shouldRequestPosition(void) {
    bool newS0State = this->plcd->getBoolAt(0);
    bool changedToTrue = (newS0State && (newS0State != this->lastS0State));
    this->lastS0State = newS0State;
    return changedToTrue;
//    return true;
}

void Scadata::requestNextPosition(void) {
    char* packet = craft_packet(C2S_BOX_POSITION_REQUEST_CODE);
    C2S_box_position_request(packet);

    socket.write(packet, C2S_BOX_POSITION_REQUEST_LEN);
    socket.flush();

    delete[] packet;
}

void Scadata::onReadyRead()
{
    std::string data = socket.readAll().toStdString();
    const char* packet = data.c_str();

    char packet_code = S2C_get_packet_code(packet);
    switch (packet_code) {
        case S2C_BOX_POSITION_RESPONSE_CODE: {
            short box_dst = S2C_parse_box_position_response(packet);
            if (box_dst <= 0) return;

            plcd->updateBoxLocation(box_dst);
            break;
        }
        default: {
            std::cout << "Unknown packet code" << std::endl;
            break;
        }
    }
}
