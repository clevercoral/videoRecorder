#ifndef NETWORK_H
#define NETWORK_H
#include "qobject.h"
#include "qtcpsocket.h"

class netWork:public QObject
{
    Q_OBJECT
public:
    netWork();
    QTcpSocket* m_sock;
    void SendData(QByteArray bt);
    QByteArray recvData();
private:
    char* serverIp;
    int port;
};

#endif // NETWORK_H
