#include "plcdata.h"
#include "hexbits.h"

PLCData::PLCData() : lecturaTimer(new QTimer(this))
{
    client = Cli_Create();
    Cli_ConnectTo(client, this->PLC_IP, 0, 0);

    for (byte i = 0; i < 33; i++) {
        this->plc_bool.insert(std::make_pair(i, false));
    }

    for (byte i = 0; i < 4; i++) {
        this->plc_number.insert(std::make_pair(i, 0));
    }

    this->setPLCInitState();
    connect(lecturaTimer, SIGNAL(timeout()), this, SLOT(updatePLCState()));
    lecturaTimer->start(this->TEntreLecturas);
}

PLCData::~PLCData() {};

bool PLCData::getBoolAt(byte position) {
    return this->plc_bool.at(position);
}
short PLCData::getNumberAt(byte position) {
    return this->plc_number.at(position);
}

//TODO: mapear las direcciones de alguna forma mas bonica
void PLCData::writeBoolAt(byte position, bool value) {
    if (position == 25) this->writeBit(4, 0, value);
    if (position == 26) this->writeBit(4, 1, value);
    if (position == 27) this->writeBit(4, 2, value);
    if (position == 28) this->writeBit(4, 3, value);
}
void PLCData::decrementNumberAt(byte position) {
    if (position == 0) this->writeBit(49, 4, true);
    if (position == 1) this->writeBit(49, 5, true);
    if (position == 2) this->writeBit(49, 6, true);
    if (position == 3) this->writeBit(49, 7, true);
}


void PLCData::setPLCInitState(void) {
    byte* initData = new byte[50];
    //Hacemos reset de todas las variables
    for (int i = 0; i < 50; i++) initData[i] = 0x00;

    //Sensores -> a 0 -> Nothing to do
    //Cilindros -> todo a 0 menos el flag de esta dentro
    //  Estos flags estan en 2.1, 2.2, 2.3
    initData[2] = (0x00 | bitmap.at(1) | bitmap.at(2) | bitmap.at(3));

    //Motores -> a 0 -> Nothing to do
    //Aceptar cajas -> a 0 -> Nothing to do
    //Muelles llenos -> a 0 -> Nothing to do
    //Contador de cajas por muelle -> a 0 -> Nothing to do
    //Capacidad de los muelles
    writeNumber(initData, this->CapacidadMuelle, 16);

    //Tiempos que tardan las cajas en pasar por los sensores
    //  Tiempos de los muelles
    writeNumber(initData, this->TSensorMuelle, 18);
    //  Tiempos de las cintas
    writeNumber(initData, this->TSensorCinta, 22);
    //  Tiempo en cinta 0
    writeNumber(initData, this->TCinta0, 26);
    //  Tiempo en cintas largas
    writeNumber(initData, this->TCintas, 30);

    //Tiempos de los cilindros
    //  Tiempo en salir
    writeNumber(initData, this->TSalidaCilindro, 34);
    //  Tiempo en volver
    writeNumber(initData, this->TRecogerCilindro, 38);

    //Tiempo caida de cajas
    writeNumber(initData, this->TCaidaCaja, 42);

    Cli_MBWrite(client, 0, 50, initData);
}

inline void PLCData::writeNumber(byte* container, int number, int offset) {
    memcpy(container + offset, &number, sizeof(int));
    swapInt(container, offset);
}
inline void PLCData::writeNumber(byte* container, short number, int offset) {
    memcpy(container + offset, &number, sizeof(short));
    swapShort(container, offset);
}
inline void PLCData::writeBit(int srcbyte, int bit, bool on) {
    Cli_WriteArea(client, (int)S7AreaMK, 0, 8*srcbyte + bit, 1, S7WLBit, &on);
}

inline short PLCData::readShort(byte* container, int start_index) {
    short value;
    swapShort(container, start_index);
    memcpy(&value, container + start_index, sizeof(short));
    return value;
}
inline bool PLCData::readBit(byte* data, int byte_index, int bit_number) {
    return (data[byte_index] & bitmap.at(bit_number)) == bitmap.at(bit_number);
}

inline void PLCData::swapShort(byte* data, int start_index) {
    byte tmp = data[start_index];
    data[start_index] = data[start_index + 1];
    data[start_index + 1] = tmp;
}
inline void PLCData::swapInt(byte* data, int start_index) {
    byte tmp = data[start_index];
    data[start_index] = data[start_index + 3];
    data[start_index + 3] = tmp;

    tmp = data[start_index + 1];
    data[start_index + 1] = data[start_index + 2];
    data[start_index + 2] = tmp;
}

//Same TODO que antes
void PLCData::updatePLCState(void) {
    byte* read_data = new byte[15];
    Cli_MBRead(client, 0, 15, read_data);

    //Sensores de cintas y muelles
    this->plc_bool[0] = readBit(read_data, 0, 0);        //S0
    this->plc_bool[1] = readBit(read_data, 0, 1);        //S1
    this->plc_bool[2] = readBit(read_data, 0, 2);        //S2
    this->plc_bool[3] = readBit(read_data, 0, 3);        //S3
    this->plc_bool[4] = readBit(read_data, 0, 4);        //S1M
    this->plc_bool[5] = readBit(read_data, 0, 5);        //S2M
    this->plc_bool[6] = readBit(read_data, 0, 6);        //S3M
    this->plc_bool[7] = readBit(read_data, 0, 7);        //SD

    this->plc_bool[8] = readBit(read_data, 1, 0);        //C1E
    this->plc_bool[9] = readBit(read_data, 1, 1);        //C2E
    this->plc_bool[10] = readBit(read_data, 1, 2);        //C3E
    this->plc_bool[11] = readBit(read_data, 1, 3);        //C1R
    this->plc_bool[12] = readBit(read_data, 1, 4);        //C2R
    this->plc_bool[13] = readBit(read_data, 1, 5);        //C3R
    this->plc_bool[14] = readBit(read_data, 1, 6);        //C1F
    this->plc_bool[15] = readBit(read_data, 1, 7);        //C2F
    this->plc_bool[16] = readBit(read_data, 2, 0);        //C3F
    this->plc_bool[17] = readBit(read_data, 2, 1);        //C1D
    this->plc_bool[18] = readBit(read_data, 2, 2);        //C2D
    this->plc_bool[19] = readBit(read_data, 2, 3);        //C3D

    this->plc_bool[20] = readBit(read_data, 3, 0);        //MO
    this->plc_bool[21] = readBit(read_data, 3, 1);        //M1
    this->plc_bool[22] = readBit(read_data, 3, 2);        //M2
    this->plc_bool[23] = readBit(read_data, 3, 3);        //M3
    this->plc_bool[24] = readBit(read_data, 3, 4);        //M4

    this->plc_bool[25] = readBit(read_data, 4, 0);        //M1AceptarCaja
    this->plc_bool[26] = readBit(read_data, 4, 1);        //M2AceptarCaja
    this->plc_bool[27] = readBit(read_data, 4, 2);        //M3AceptarCaja
    this->plc_bool[28] = readBit(read_data, 4, 3);        //DestinoConocido
    this->plc_bool[29] = readBit(read_data, 4, 4);        //M1Lleno
    this->plc_bool[30] = readBit(read_data, 4, 5);        //M2Lleno
    this->plc_bool[31] = readBit(read_data, 4, 6);        //M3Lleno
    this->plc_bool[32] = readBit(read_data, 4, 7);        //MDLleno

    this->plc_number[0] = readShort(read_data, 6);       //CajasM1
    this->plc_number[1] = readShort(read_data, 8);       //CajasM2
    this->plc_number[2] = readShort(read_data, 10);       //CajasM3
    this->plc_number[3] = readShort(read_data, 12);       //CajasMD
}
