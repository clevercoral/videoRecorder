#include "room.h"
#include "ui_room.h"
#include "qstringlistmodel.h"
#include "qdebug.h"
Room::Room(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->move(0,height()/2);
}

Room::~Room()
{
    delete ui;
}

void Room::addComment(QString &text)
{
    qDebug()<<__func__;
    QStringListModel *commentModel=new QStringListModel(this);
    ui->le_comment->setModel(commentModel);
    commentModel->insertRow(0); // 新评论插入顶部
    commentModel->setData(commentModel->index(0), text);
    ui->le_comment->scrollToTop(); // 自动滚动
}
