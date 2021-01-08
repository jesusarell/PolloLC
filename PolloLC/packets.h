#ifndef PACKETS_H
#define PACKETS_H
#include "plcdata.h"

const char C2S_BOOLS_TRAP_CODE = 0x00;
const char C2S_SHORTS_TRAP_CODE = 0x01;
const char C2S_BOX_POSITION_REQUEST_CODE = 0x02;
const char S2C_BOX_POSITION_RESPONSE_CODE = 0x64;

const int C2S_BOOLS_TRAP_PAYLOAD_LEN = 6;
const int C2S_SHORTS_TRAP_PAYLOAD_LEN = 8;
const int C2S_BOX_POSITION_REQUEST_PAYLOAD_LEN = 0;
const int S2C_BOX_POSITION_RESPONSE_PAYLOAD_LEN = 2;

const int C2S_BOOLS_TRAP_LEN = C2S_BOOLS_TRAP_PAYLOAD_LEN + 5;
const int C2S_SHORTS_TRAP_LEN = C2S_SHORTS_TRAP_PAYLOAD_LEN + 5;
const int C2S_BOX_POSITION_REQUEST_LEN = C2S_BOX_POSITION_REQUEST_PAYLOAD_LEN + 5;
const int S2C_BOX_POSITION_RESPONSE_LEN = S2C_BOX_POSITION_RESPONSE_PAYLOAD_LEN + 5;

char* craft_packet(char packet_code);

void C2S_bools_trap(char* packet, PLCData* plcd);
void C2S_shorts_trap(char* packet);
void C2S_box_position_request(char* packet);
void S2C_box_position_response(char* packet, short dst);

char C2S_get_packet_code(const char* packet);
char S2C_get_packet_code(const char* packet);

void C2S_parse_bools_trap(const char* packet, std::map<char, bool>* plc_bool);
void C2S_parse_shorts_trap(const char* packet, std::map<char, short>* plc_number);
short S2C_parse_box_position_response(const char* packet);

#endif // PACKETS_H


//TODOLIST:
    //Paquetes del protocolo
    // Header : 1byte code, 4 byte len, payload
    //  - estado bools : code 0x00, len n
    //      - Scinta, Motor, Cilindros (Empujando, Retrocediendo, Fuera, Dentro), Smuelle
    //  - estado muelles : code 0x01, len 2 * 4 = 8
    //      - muelle1, muelle2, muelle3, muelle4
    //  - request box position : code 0x02, len 0
    //  - enviar destino caja : code dec100 -> 0x64, len : 1
    //      - nº muelle destino

    //Leer el fichero con los destinos de las cajas y utilizarlo para responder
//Interfaz grafica
//  Simular cajas moviendose --> ¡Jisus!

//Mapear las variables del pcl a algo mas tragable --> Vic
//Autentificacion y CIFRADO CON GILTZARRAPO --> Jisus
