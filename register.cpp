#include "register.h"
#include "ui_register.h"
#include "qmessagebox.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void Register::on_pb_commit_clicked()
{
    //获取输入的数据
    QString userName;
    QString tel;
    QString config;
    QString password;
    userName=ui->le_user->text();
    tel=ui->le_tel->text();
    config=ui->le_config->text();
    password=ui->le_password->text();
    if(config!=password)
    {
        //如果不相同，则弹窗错误
        QMessageBox::about(this,"提示","两次输入的密码不一样,请重新输入");
        return;
    }
    this->hide();
    //向服务端发送数据
    emit sendRegisterMsg(userName,tel,password);
}

