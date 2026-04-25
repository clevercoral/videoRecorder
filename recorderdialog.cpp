#include "recorderdialog.h"
#include "ui_recorderdialog.h"

recorderDialog::recorderDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::recorderDialog)
{
    ui->setupUi(this);
    m_camera=new cameraWidget;
    m_camera->hide();
    m_camera->move(0,0);
    m_camera->setWindowFlags(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);

    m_save=new saveVedioThread;
    connect(m_save,SIGNAL(SIG_sendPicInPic(QImage)),m_camera,SLOT(slot_getImage(QImage)));
    connect(m_save,SIGNAL(SIG_sendVideoFrame( QImage)),this,SLOT(slot_setImage(QImage)));
    m_room=new Room;
}

recorderDialog::~recorderDialog()
{
    delete ui;
}

void recorderDialog::setUrl(QString url)
{
    m_fileUrl=url;
}

void recorderDialog::on_pb_start_clicked()
{
    //申请结构体
    STRU_AV_FORMAT format;
    format.fileUrl=m_fileUrl;
    format.frame_rate=FRAME_RATE;
    format.hasAudio=true;
    format.hasCamera=true;
    format.hasDesktop=true;
    format.videoBitRate=4000000;
    QScreen* src=QApplication::primaryScreen();
    QRect rect=src->geometry();
    //分辨率的问题改动
    format.width=rect.width();
    format.height=rect.height();
//    qDebug()<<format.width;
//    qDebug()<<format.height;
    format.width=2240;
    format.height=1400;
    m_save->slot_setInfo(format);
    //打开设备
    m_save->slot_openVideo();
    //最小化
    this->showMinimized();
    m_camera->showMinimized();
    m_room->showNormal();

    //转发给kernel发送给服务端，登记直播信息
    emit startAlive();
}


void recorderDialog::on_pb_end_clicked()
{
    m_camera->close();
    m_save->slot_closeVideo();
    //直播结束，向服务端发送结束标志
}


void recorderDialog::on_pb_setUrl_clicked()
{
    //m_fileUrl=ui->le_url->text();
   // m_fileUrl=m_fileUrl.replace("/","\\");
}

void recorderDialog::slot_setImage(QImage img)
{
    QPixmap pix;
    if(!img.isNull())
    {
         pix=QPixmap::fromImage(img.scaled(img.size(),Qt::KeepAspectRatio));
    }
    else
    {
        pix=QPixmap::fromImage(img);
    }
    ui->lb_show->setPixmap(pix);
}

void recorderDialog::setRoom(Room *newRoom)
{
    m_room = newRoom;
}

Room *recorderDialog::getRoom() const
{
    return m_room;
}


void recorderDialog::on_pb_whiteFace_clicked()
{
    m_save->openWhiteFace();
}

