#ifndef PLCDATA_H
#define PLCDATA_H

#include <snap7.h>
#include <map>
#include <QTimer>

class PLCData : public QObject {
    Q_OBJECT

public:
    PLCData();
    ~PLCData();
    bool getBoolAt(byte position);
    short getNumberAt(byte position);
    void writeBoolAt(byte position, bool value);
    void writeNumberAt(byte position, short value);
    void decrementNumberAt(byte position);
    void updateBoxLocation(short position);

private:
    //Cliente s7
    S7Object client;
    const char* PLC_IP = "172.18.67.99";

    //Tiempos en ms
    const int TSensorMuelle = 750;
    const int TSensorCinta = 750;
    const int TCinta0 = 500;
    const int TCintas = 1500;
    const int TSalidaCilindro = 500;
    const int TRecogerCilindro = 500;
    const int TCaidaCaja = 750;

    //Capacidad en unidades
    const short CapacidadMuelle = 3;

    //Tiempo entre actualizaciones de memoria en ms
    const int TEntreLecturas = 100;

    //Diccionarios para almacenamiento de datos
    std::map<byte, bool> plc_bool;
    std::map<byte, short> plc_number;

    //Timer para desencadenar la lectura
    QTimer* lecturaTimer;

private:
    void setPLCInitState(void);
    inline void writeNumber(byte* container, int number, int offset);
    inline void writeNumber(byte* container, short number, int offset);
    inline void writeBit(int srcbyte, int bit, bool data);
    inline short readShort(byte* container, int start_index);
    inline bool readBit(byte* data, int byte_index, int bit_number);
    inline void swapShort(byte* data, int start_index);
    inline void swapInt(byte* data, int start_index);

private slots:
    void updatePLCState(void);

};

#endif // PLCDATA_H
