#include "messagedialog.h"
#include "ui_messagedialog.h"
#include "qmessagebox.h"
#include "qdebug.h"
MessageDialog::MessageDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::setRoomId(QString roomId)
{
    ui->le_roomId->setText(roomId);
}

void MessageDialog::on_pb_configure_clicked()
{
    roomInfo info;
    info.m_link=QString("rtmp://%1/videotest").arg(ui->le_ip->text());
    info.m_roomId=ui->le_roomId->text();
    info.m_roomTitle=ui->le_title->text();
    info.m_type=ui->cbx_type->currentIndex()+1;
    qDebug()<<info.m_type;
    emit sendMsg(info);
    QMessageBox::about(this,"发起直播成功","您的直播地址为: "+info.m_link+"请点击'开始'开启直播");
}

