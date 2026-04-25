#include "network.h"

netWork::netWork()
{
    m_sock=new QTcpSocket;
    m_sock->connectToHost("192.168.167.1",8888);
}

void netWork::SendData(QByteArray bt)
{
    int len=bt.size();
    m_sock->write((char*)&len,sizeof(int));
    m_sock->write(bt.data(),bt.size());
}

QByteArray netWork::recvData()
{
    int len=0;
    m_sock->read((char*)&len,sizeof(int));
    qDebug()<<"recv data size:"<<len;
    char Buf[1024];
    memset(Buf,0,sizeof(Buf));
    m_sock->read(Buf,len);
    QByteArray bt(Buf,len);
    qDebug()<<"recv data content:"<<bt.data();
    return bt;
}
