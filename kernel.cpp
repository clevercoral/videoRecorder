#include "kernel.h"
#include "qdebug.h"
#include "CJson.h"
#include "qmessagebox.h"

Kernel::Kernel()
{
    m_login=new Login;
    m_login->show();
    connect(m_login,SIGNAL(SIG_sendMsg(QByteArray)),this,SLOT(slot_login(QByteArray)));
    m_net=new netWork;
    connect(m_net->m_sock,&QTcpSocket::readyRead,this,&Kernel::pushMsg);
    m_recorder=new recorderDialog;
    connect(m_recorder,SIGNAL(startAlive()),this,SLOT(slot_startAlive()));
    m_aliveMsg=new MessageDialog;
    m_aliveMsg->setWindowTitle("请输入直播信息");
    connect(m_aliveMsg,SIGNAL(sendMsg(roomInfo&)),this,SLOT(slot_getMsg(roomInfo&)));
    m_re=new Register;
    connect(m_re,SIGNAL(sendRegisterMsg(QString,QString,QString)),this,SLOT(slot_getRegisterMsg(QString, QString, QString)));
    connect(m_login,SIGNAL(SIG_RegisterRs()),this,SLOT(SLOT_RegisterRs()));
}

void Kernel::pushMsg()
{
    QByteArray bt=m_net->recvData();
    CJson json(bt);
    int type=json.json_get_int("type");
    //登录请求
    if(type==10002)
    {
        loginRs(bt);
    }//评论请求
    else  if(type==10006)
    {
        commentRs(bt);
    }//注册请求
    else if(type==10008)
    {
        RegisterRs(bt);
    }
}

void Kernel::loginRs(QByteArray bt)
{
    qDebug()<<__func__;
    CJson json(bt);
    int res=json.json_get_int("result");
    if(res==1)
    {
        m_login->hide();
        m_recorder->show();
        QString roomId=json.json_get_string("roomId");
        userName=json.json_get_string("userName");
        qDebug()<<roomId;
        qDebug()<<userName;
        m_aliveMsg->setRoomId(roomId);
        m_aliveMsg->show();
        QMessageBox::about(m_login,"提示","欢迎您: "+userName);
    }
    else {
        QMessageBox::about(m_login,"提示","登录失败");
    }
}

void Kernel::commentRs(QByteArray bt)
{
    CJson json(bt);
    QString content=json.json_get_string("content");
    Room* aliveRoom=m_recorder->getRoom();
    aliveRoom->addComment(content);
}

void Kernel::slot_login(QByteArray bt)
{
    qDebug()<<__func__;
    m_net->SendData(bt);
    //m_net->m_sock->write(bt);
}
void Kernel::slot_getMsg(roomInfo &info)
{
    m_info=info;
    m_recorder->setUrl(info.m_link);
    m_aliveMsg->hide();
}

void Kernel::slot_startAlive()
{
    CJson json;
    json.json_add_value("type",10003);
    json.json_add_value("roomId",m_info.m_roomId.toStdString().c_str());
    json.json_add_value("roomTitle",m_info.m_roomTitle.toStdString().c_str());
    json.json_add_value("link",m_info.m_link.toStdString().c_str());
    json.json_add_value("aliveType",m_info.m_type);
    QByteArray bt=json.json_to_string();
    m_net->SendData(bt);
}

void Kernel::slot_getRegisterMsg(QString userName, QString tel, QString password)
{
    qDebug()<<__func__;
    CJson json;
    json.json_add_value("type",10007);
    json.json_add_value("userName",userName.toStdString().c_str());
    json.json_add_value("tel",tel.toStdString().c_str());
    json.json_add_value("password",password.toStdString().c_str());
    QByteArray bt=json.json_to_string();
    m_net->SendData(bt);
}

void Kernel::SLOT_RegisterRs()
{
    //弹出注册窗口
    m_re->show();
}

void Kernel::RegisterRs(QByteArray bt)
{
    qDebug()<<__func__;
    CJson json(bt);
    if(json.json_get_int("result")==1)
    {
        QMessageBox::about(m_re,"提示","注册成功");
    }
    else
    {
         QMessageBox::about(m_re,"提示","注册失败");
    }
}
