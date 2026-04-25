#ifndef SAVEVEDIOTHREAD_H
#define SAVEVEDIOTHREAD_H

#include <QThread>
#include "picinpic_read.h"
#include "audio_read.h"
#include "qmutex.h"

struct STRU_AV_FORMAT
{
    bool hasCamera;
    bool hasDesktop;
    bool hasAudio;

    ///视频信息
    int width;
    int height;
    int frame_rate;
    int videoBitRate;
    int codec_id;//默认格式为h.264
    int pix_fmt;//默认格式为YUV420P 默认
    QString fileUrl;//传出的url
    ///音频信息
    void clear()
    {
        memset(this,0,sizeof(STRU_AV_FORMAT));
    }
};

//输出流
typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    uint8_t* frameBuffer;
    int frameBufferSize;
    AVFrame *tmp_frame;

    float t, tincr, tincr2;

    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;

struct BufferDataNode
{
    uint8_t * buffer;
    int bufferSize;
    int64_t time; //视频帧用于稳帧, 比较时间
};

//这个类用于保存当前的音频线程
class saveVedioThread : public QThread
{
    Q_OBJECT
signals:
    void SIG_sendVideoFrame( QImage img ); // 用于预览
    void SIG_sendPicInPic( QImage img ); //用于显示画中画
public:
    saveVedioThread();
    void add_video_stream(OutputStream *ost, AVFormatContext *oc, AVCodec **codec, AVCodecID codec_id);
    void open_video(AVFormatContext *oc, AVCodec *codec, OutputStream *ost);
    void close_stream(AVFormatContext *oc, OutputStream *ost);
    int write_frame(AVFormatContext *fmt_ctx, AVCodecContext *c, AVStream *st, AVFrame *frame);
    void add_audio_stream(OutputStream *ost, AVFormatContext *oc, AVCodec **codec, AVCodecID codec_id);
    void open_audio(AVFormatContext *oc, AVCodec *codec, OutputStream *ost);
    void videoDataQuene_Input(uint8_t *buffer, int size, int64_t time);
    BufferDataNode *videoDataQuene_get(int64_t time);
    void audioDataQuene_Input(const uint8_t *buffer, const int &size);
    BufferDataNode *audioDataQuene_get();
    virtual void run();
    int write_frame(AVFormatContext *fmt_ctx, AVCodecContext *c, AVStream *st, AVFrame *frame, int64_t &pts, OutputStream *ost);
    bool write_video_frame(AVFormatContext *oc, OutputStream *ost, double time);
    bool write_audio_frame(AVFormatContext *oc, OutputStream *ost);
public slots:
    void slot_getVideoFrameData( uint8_t* picture_buf, int buffer_size );//采集的数据格式 YUV420P
    void slot_getAudioFrameData( uint8_t* picture_buf, int buffer_size );//将采集到的音视频进行存储或者推流
    void slot_openVideo();
    void slot_closeVideo();
    //初始化一些音视频格式的基本信息，主要是STRU_FORMAT结构体中的成员
    void slot_setInfo(STRU_AV_FORMAT& avFormat);
    //开启美颜
    void openWhiteFace();
private:
    OutputStream video_st = { 0 }, audio_st = { 0 };
    const char *filename;
    AVOutputFormat *fmt;
    AVFormatContext *oc;
    AVCodec *audio_codec, *video_codec;
    int ret;
    int have_video = 0, have_audio = 0;
    int encode_video = 0, encode_audio = 0;
    PicInPic_Read* m_video;
    STRU_AV_FORMAT m_avFormat;
    Audio_Read* m_audio;
    int mAudioOneFrameSize=0;

    //音视频同步
    QList<BufferDataNode*> m_videoDataList;
    QList<BufferDataNode*> m_audioDataList;
    BufferDataNode* lastVideoNode=nullptr;
    bool m_videoBeginFlag=false;
    qint64 m_videoBeginTime=0;
    QMutex m_videoMutex;
    QMutex m_audioMutex;
    bool isStop=false;
    int64_t video_pts=0;
    int64_t audio_pts=0;
};

#endif // SAVEVEDIOTHREAD_H
