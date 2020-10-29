#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include <snap7.h>
#include "hexbits.h"
#include "plcdata.h"

#define GLED_ON "background-color:rgb(0,255,0);border-radius:10;border:2px solid black"
#define GLED_OFF "background-color:rgb(0,0,0);border-radius:10;border:2px solid rgb(0,255,0)"
#define RLED_ON "background-color:rgb(255,0,0);border-radius:10;border:2px solid black"
#define RLED_OFF "background-color:rgb(0,0,0);border-radius:10;border:2px solid rgb(255,0,0)"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    S7Object client;
    PLCData* plcd;

    const int TCintas = 3000;
    const int TCinta0 = 500;
    const short CapacidadMuelle = 10;
    const int TiempoSalidaCilindro = 500;
    const int TiempoRecogerCilindro = 500;
    const int TiempoCaidaCaja = 500;
    const int TPasarPorSensorMuelle = 500;
    const int TPasarPorSensorCinta = 500;

    QTimer* updateTimer;
    const int updateRate = 50;

    void setPLCInitState(void);
    void writeBit(int srcbyte, int bit, bool data);
    bool readBit(byte* data, int byte_index, int bit_number);
    inline void writeNumber(byte* container, int number, int offset);
    inline void writeNumber(byte* container, short number, int offset);
    inline short readShort(byte* container, int start_index);
    inline void swapShort(byte* data, int start_index);
    inline void swapInt(byte* data, int start_index);

private slots:
    void updatePLCState(void);
    void knownLocationOnClick(void);
    void acceptNextM1OnClick(void);
    void acceptNextM2OnClick(void);
    void acceptNextM3OnClick(void);
};
#endif // MAINWINDOW_H
