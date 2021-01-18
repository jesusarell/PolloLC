#ifndef SCADATA_H
#define SCADATA_H

#include <QTimer>
#include <QTcpSocket>
#include "plcdata.h"

#define SERVER_PORT 4242
#define SERVER_HOST "127.0.0.1"
//#define SERVER_HOST "172.18.67.196"

class Scadata : public QObject {
    Q_OBJECT

public:
    Scadata(PLCData* plcd);
    ~Scadata();

private:
    PLCData* plcd;
    QTcpSocket socket;

    bool lastS0State;

    void sendTraps(void);

private slots:
    void onCheck(void);
    bool shouldRequestPosition(void);
    void requestNextPosition(void);

    void onReadyRead();

};

#endif // SCADATA_H
