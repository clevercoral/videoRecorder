#include "camerawidget.h"
#include "ui_camerawidget.h"

cameraWidget::cameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cameraWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
}

cameraWidget::~cameraWidget()
{
    delete ui;
}



//根据获取到的数据将图像显示在控件上
void cameraWidget::slot_getImage(QImage img)
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
