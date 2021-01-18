#include <QHostAddress>
#include "scadata.h"
#include <iostream>
#include "packets.h"

Scadata::Scadata(PLCData* plcd_ptr)
{
    plcd = plcd_ptr;
    plcd->addOnRefreshCallback(std::bind(&Scadata::onCheck, this));

    socket.connectToHost(QHostAddress(SERVER_HOST), SERVER_PORT);
    connect(&socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}


Scadata::~Scadata() {};

void Scadata::onCheck(void) {
    if (this->shouldRequestPosition()) {
        requestNextPosition();
    }

    this->sendTraps();
}

bool Scadata::shouldRequestPosition(void) {
    bool newS0State = this->plcd->getBoolAt(0);
    bool changedToTrue = (newS0State && (newS0State != this->lastS0State));
    this->lastS0State = newS0State;
    return changedToTrue;
}

void Scadata::requestNextPosition(void) {
    char* packet = craft_packet(C2S_BOX_POSITION_REQUEST_CODE);
    C2S_box_position_request(packet);

    socket.write(packet, C2S_BOX_POSITION_REQUEST_LEN);
    socket.flush();

    delete[] packet;
}

void Scadata::sendTraps() {
    char* bool_trap = craft_packet(C2S_BOOLS_TRAP_CODE);
    C2S_bools_trap(bool_trap, this->plcd);

    char* short_trap = craft_packet(C2S_SHORTS_TRAP_CODE);
    C2S_shorts_trap(short_trap, this->plcd);

    socket.write(bool_trap, C2S_BOOLS_TRAP_LEN);
    socket.write(short_trap, C2S_SHORTS_TRAP_LEN);
    socket.flush();

    delete[] bool_trap;
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
