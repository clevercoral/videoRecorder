#ifndef KERNEL_H
#define KERNEL_H

#include <QObject>
#include "network.h"
#include "login.h"
#include "recorderdialog.h"
#include "messagedialog.h"
#include "room.h"
#include "register.h"
class Kernel:public QObject
{
    Q_OBJECT
public:
    Kernel();
    void pushMsg();
    void loginRs(QByteArray bt);
    void commentRs(QByteArray bt);
    void RegisterRs(QByteArray bt);
public slots:
    void slot_login(QByteArray bt);
    void slot_getMsg(roomInfo& info);
    void slot_startAlive();
    void slot_getRegisterMsg(QString userName,QString tel,QString password);
    void SLOT_RegisterRs();
private:
    netWork* m_net;
    Login* m_login;
    recorderDialog* m_recorder;
    MessageDialog* m_aliveMsg;
    roomInfo m_info;
    Register* m_re;
    QString userName;
};

#endif // KERNEL_H
