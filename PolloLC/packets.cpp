#include "packets.h"
#include "hexbits.h"

char* craft_packet(char packet_code) {
    char* packet;

    switch (packet_code) {
        case C2S_BOOLS_TRAP_CODE: {
            packet = new char[C2S_BOOLS_TRAP_LEN];
            break;
        }
        case C2S_SHORTS_TRAP_CODE: {
            packet = new char[C2S_SHORTS_TRAP_LEN];
            break;
        }
        case C2S_BOX_POSITION_REQUEST_CODE: {
            packet = new char[C2S_BOX_POSITION_REQUEST_LEN];
            break;
        }
        case S2C_BOX_POSITION_RESPONSE_CODE: {
            packet = new char[S2C_BOX_POSITION_RESPONSE_LEN];
            break;
        }
        default: {
            throw "Unknown packet code";
            break;
         }
    }

    return packet;
}

void C2S_bools_trap(char* packet, PLCData* plcd) {
    packet[0] = C2S_BOOLS_TRAP_CODE;
    memcpy(packet + 1, &C2S_BOOLS_TRAP_PAYLOAD_LEN, sizeof(int));
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
    packet[0] = C2S_SHORTS_TRAP_CODE;
    memcpy(packet + 1, &C2S_SHORTS_TRAP_PAYLOAD_LEN, sizeof(int));

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
    packet[0] = C2S_BOX_POSITION_REQUEST_CODE;
    memcpy(packet + 1, &C2S_BOX_POSITION_REQUEST_PAYLOAD_LEN, sizeof(int));
}

void S2C_box_position_response(char* packet, short dst) {
    packet[0] = S2C_BOX_POSITION_RESPONSE_CODE;
    memcpy(packet + 1, &S2C_BOX_POSITION_RESPONSE_PAYLOAD_LEN, sizeof(int));
    memcpy(packet + 5, &dst, sizeof(short));
}

char C2S_get_packet_code(const char* packet) {
    return packet[0];
}
char S2C_get_packet_code(const char* packet) {
    return packet[0];
}

void C2S_parse_bools_trap(const char* packet, std::map<char, bool>* plc_bool) {
    (*plc_bool)[0]  = (packet[5] & bitmap.at(0)) == bitmap.at(0);
    (*plc_bool)[20] = (packet[5] & bitmap.at(1)) == bitmap.at(1);

    (*plc_bool)[1]  = (packet[6] & bitmap.at(0)) == bitmap.at(0);
    (*plc_bool)[21] = (packet[6] & bitmap.at(1)) == bitmap.at(1);
    (*plc_bool)[8]  = (packet[6] & bitmap.at(2)) == bitmap.at(2);
    (*plc_bool)[11] = (packet[6] & bitmap.at(3)) == bitmap.at(3);
    (*plc_bool)[14] = (packet[6] & bitmap.at(4)) == bitmap.at(4);
    (*plc_bool)[17] = (packet[6] & bitmap.at(5)) == bitmap.at(5);
    (*plc_bool)[29] = (packet[6] & bitmap.at(6)) == bitmap.at(6);

    (*plc_bool)[2]  = (packet[7] & bitmap.at(0)) == bitmap.at(0);
    (*plc_bool)[22] = (packet[7] & bitmap.at(1)) == bitmap.at(1);
    (*plc_bool)[9]  = (packet[7] & bitmap.at(2)) == bitmap.at(2);
    (*plc_bool)[12] = (packet[7] & bitmap.at(3)) == bitmap.at(3);
    (*plc_bool)[15] = (packet[7] & bitmap.at(4)) == bitmap.at(4);
    (*plc_bool)[18] = (packet[7] & bitmap.at(5)) == bitmap.at(5);
    (*plc_bool)[30] = (packet[7] & bitmap.at(6)) == bitmap.at(6);

    (*plc_bool)[3]  = (packet[8] & bitmap.at(0)) == bitmap.at(0);
    (*plc_bool)[23] = (packet[8] & bitmap.at(1)) == bitmap.at(1);
    (*plc_bool)[10] = (packet[8] & bitmap.at(2)) == bitmap.at(2);
    (*plc_bool)[13] = (packet[8] & bitmap.at(3)) == bitmap.at(3);
    (*plc_bool)[16] = (packet[8] & bitmap.at(4)) == bitmap.at(4);
    (*plc_bool)[19] = (packet[8] & bitmap.at(5)) == bitmap.at(5);
    (*plc_bool)[31] = (packet[8] & bitmap.at(6)) == bitmap.at(6);

    (*plc_bool)[32] = (packet[9] & bitmap.at(6)) == bitmap.at(6);
}

void C2S_parse_shorts_trap(const char* packet, std::map<char, short>* plc_number) {
    /*
    short m1, m2, m3, m4;

    memcpy(&m1, packet + 5,  sizeof(short));
    memcpy(&m2, packet + 7,  sizeof(short));
    memcpy(&m3, packet + 9,  sizeof(short));
    memcpy(&m4, packet + 11, sizeof(short));

    (*plc_number)[0] = m1;
    (*plc_number)[1] = m2;
    (*plc_number)[2] = m3;
    (*plc_number)[3] = m4;
    */

    memcpy(&((*plc_number)[0]), packet + 5,  sizeof(short));
    memcpy(&((*plc_number)[1]), packet + 7,  sizeof(short));
    memcpy(&((*plc_number)[2]), packet + 9,  sizeof(short));
    memcpy(&((*plc_number)[3]), packet + 11, sizeof(short));
}

short S2C_parse_box_position_response(const char* packet) {
    short dst;
    memcpy(&dst, packet + 5, sizeof(short));
    return dst;
}
