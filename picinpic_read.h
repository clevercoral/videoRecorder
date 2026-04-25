#ifndef PICINPIC_READ_H
#define PICINPIC_READ_H
#include <QObject>
#include<QImage>
#include<QImageReader>
#include"common.h"
#include<QBuffer>
#include<QPainter>
#include<QTime>
#include<QTimer>
#include<QApplication>
#include<QDesktopWidget>
#include<QScreen>
#include "opencv2/opencv.hpp"
#include <iostream>
class PicInPic_Read: public QObject
{
    Q_OBJECT
public:
    explicit PicInPic_Read(QObject *parent = 0);
signals:
    void SIG_sendVideoFrame( QImage img ); // 用于预览
    void SIG_sendVideoFrameData( uint8_t* picture_buf, int buffer_size ); //采集的数据格式 YUV420P
    void SIG_sendPicInPic( QImage img ); //用于显示画中画
public slots:
    void slot_getVideoFrame(); //定时器周期获取画面
    void slot_openVideo(); //开启采集
    void slot_closeVideo(); //关闭采集
    void startWhiteFace();//开启美白功能
    //opencv的人脸识别功能


private:
    VideoCapture cap; //opencv 采集摄像头对象
    QTimer * timer; //定时器
    int ImageToYuvBuffer(QImage &image,uint8_t **buffer); //RGB24 转为 yuv420p
    int m_whiteFaceFlag;//1表示开启美白功能
};
#endif // PICINPIC_READ_H
