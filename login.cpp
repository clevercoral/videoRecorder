#include "login.h"
#include "ui_login.h"
#include "CJson.h"
#include "qdebug.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

//协议1：type:10001 ,password:QString  , tel:QString
void Login::on_pb_login_clicked()
{
    QString tel=ui->le_tel->text();
    QString password=ui->le_password->text();
    CJson json;
    json.json_add_value("type",10001);
    json.json_add_value("tel",tel.toStdString().c_str());
    json.json_add_value("password",password.toStdString().c_str());
    json.json_add_value("isalive",1);
    emit SIG_sendMsg(json.json_to_string());
}


void Login::on_pb_register_clicked()
{
    qDebug()<<__func__;
    emit SIG_RegisterRs();
}

