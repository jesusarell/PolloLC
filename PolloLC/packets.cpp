#include "packets.h"
#include "hexbits.h"

void C2S_bools_trap(char* packet, PLCData* plcd) {
    packet[0] = 0x00;
    memcpy(packet + 1, &C2S_BOOLS_TRAP_LEN, sizeof(int));
    packet[5] = 0x00 | (plcd->getBoolAt(0)  ? bitmap.at(0) : 0x00)
                     | (plcd->getBoolAt(20) ? bitmap.at(1) : 0x00);

    packet[6] = 0x00 | (plcd->getBoolAt(1)  ? bitmap.at(0) : 0x00)
                     | (plcd->getBoolAt(21) ? bitmap.at(1) : 0x00)
                     | (plcd->getBoolAt(8)  ? bitmap.at(2) : 0x00)
                     | (plcd->getBoolAt(11) ? bitmap.at(3) : 0x00)
                     | (plcd->getBoolAt(14) ? bitmap.at(4) : 0x00)
                     | (plcd->getBoolAt(17) ? bitmap.at(5) : 0x00)
                     | (plcd->getBoolAt(29) ? bitmap.at(6) : 0x00);

    packet[7] = 0x00 | (plcd->getBoolAt(2)  ? bitmap.at(0) : 0x00)
                     | (plcd->getBoolAt(22) ? bitmap.at(1) : 0x00)
                     | (plcd->getBoolAt(9)  ? bitmap.at(2) : 0x00)
                     | (plcd->getBoolAt(12) ? bitmap.at(3) : 0x00)
                     | (plcd->getBoolAt(15) ? bitmap.at(4) : 0x00)
                     | (plcd->getBoolAt(18) ? bitmap.at(5) : 0x00)
                     | (plcd->getBoolAt(30) ? bitmap.at(6) : 0x00);

    packet[8] = 0x00 | (plcd->getBoolAt(3)  ? bitmap.at(0) : 0x00)
                     | (plcd->getBoolAt(23) ? bitmap.at(1) : 0x00)
                     | (plcd->getBoolAt(10) ? bitmap.at(2) : 0x00)
                     | (plcd->getBoolAt(13) ? bitmap.at(3) : 0x00)
                     | (plcd->getBoolAt(16) ? bitmap.at(4) : 0x00)
                     | (plcd->getBoolAt(19) ? bitmap.at(5) : 0x00)
                     | (plcd->getBoolAt(31) ? bitmap.at(6) : 0x00);

    packet[9] = 0x00 | (plcd->getBoolAt(32) ? bitmap.at(6) : 0x00);
}

void C2S_shorts_trap(char* packet, PLCData* plcd) {
    packet[0] = 0x01;
    memcpy(packet + 1, &C2S_SHORTS_TRAP_LEN, sizeof(int));

    short m1, m2, m3, m4;
    m1 = plcd->getNumberAt(0);
    m2 = plcd->getNumberAt(1);
    m3 = plcd->getNumberAt(2);
    m4 = plcd->getNumberAt(3);

    memcpy(packet + 5,  &m1, sizeof(short));
    memcpy(packet + 7,  &m2, sizeof(short));
    memcpy(packet + 9,  &m3, sizeof(short));
    memcpy(packet + 11, &m4, sizeof(short));
}

void C2S_box_position_request(char* packet) {
    packet[0] = 0x02;
    memcpy(packet + 1, &C2S_BOX_POSITION_REQUEST_LEN, sizeof(int));
}

void S2C_box_position_response(char* packet, char dst) {
    packet[0] = 0x64;
    memcpy(packet + 1, &S2C_BOX_POSITION_RESPONSE_LEN, sizeof(int));
    packet[5] = dst;
}
