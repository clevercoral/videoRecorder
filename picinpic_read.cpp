#include "picinpic_read.h"
#include "opencv2/face.hpp"
PicInPic_Read::PicInPic_Read(QObject *parent): QObject(parent)
{
    timer = new QTimer(this);
    connect( timer , SIGNAL(timeout()) , this , SLOT(slot_getVideoFrame()));
}
void PicInPic_Read::slot_openVideo()
{
    cap.open(0);
    if(!cap.isOpened()){
        QMessageBox::information(NULL,tr("提示"),tr("视频没有打开"));
        return;
    }
    //宁多勿少
    timer->start(1000/FRAME_RATE - 10 );
}
void PicInPic_Read::slot_closeVideo()
{
    timer->stop();
    if(cap.isOpened())
        cap.release();
}

void PicInPic_Read::startWhiteFace()
{
    //将标志位置为1
    if(m_whiteFaceFlag==0)
        m_whiteFaceFlag=1;
}
//使用opencv进行美颜

using namespace std;
using namespace cv;
using namespace std;
//大眼
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

using namespace cv;
using namespace std;
using namespace cv::face;
typedef struct
{
    cv::Rect face_rect;
    vector<Point2f> landmarks;
} FaceMark;

/**
 * @brief 大眼特效接口函数（整合版）
 * @param input_img 输入图像
 * @param output_img 输出图像（大眼处理结果）
 * @param enlarge_strength 大眼力度(0-10)，默认5.0
 * @return 是否处理成功
 */
bool EnlargeEyes(const cv::Mat& input_img, cv::Mat& output_img, float enlarge_strength = 5.0f);

// 内部函数声明
void DetectFaceMark(const cv::Mat& img, vector<FaceMark>& face_marks);
void EnlargeEyesCore(const cv::Mat& img, const vector<FaceMark>& face_marks, float strength, cv::Mat& output);
void drawPolyline(Mat &im, const vector<Point2f> &landmarks, const int start, const int end, bool isClosed = false);


/**
 * @brief 大眼特效主接口
 */
bool EnlargeEyes(const cv::Mat& input_img, cv::Mat& output_img, float enlarge_strength) {
    if (input_img.empty() || input_img.channels() != 3) {
        cout << "Error: 输入图像无效" << endl;
        return false;
    }

    input_img.copyTo(output_img);
    vector<FaceMark> face_marks;

    // 检测人脸与关键点
    DetectFaceMark(input_img, face_marks);
    if (face_marks.empty()) {
        cout << "Warning: 未检测到人脸，直接返回原图" << endl;
        return true;
    }

    // 执行大眼处理
    EnlargeEyesCore(output_img, face_marks, enlarge_strength, output_img);
    return true;
}


/**
 * @brief 人脸与关键点检测（保留原逻辑）
 */
void DetectFaceMark(const cv::Mat& img, vector<FaceMark>& face_marks) {
    // 创建人脸检测器与关键点检测器
    CascadeClassifier faceDetector("../../data/haarcascade_frontalface_alt.xml");
    Ptr<Facemark> facemark = FacemarkLBF::create();
    facemark->loadModel("../../data/lbfmodel.yaml");


    Mat gray;
    vector<Rect> faces;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    faceDetector.detectMultiScale(gray, faces);

    vector<vector<Point2f>> landmarks;
    face_marks.clear();
    if (facemark->fit(img, faces, landmarks)) {
        for (size_t i = 0; i < landmarks.size(); i++) {
            FaceMark mark;
            mark.face_rect = faces[i];
            mark.landmarks = landmarks[i];
            face_marks.push_back(mark);
        }
    }
}


/**
 * @brief 大眼核心算法（优化原逻辑）
 */
void EnlargeEyesCore(const cv::Mat& img, const vector<FaceMark>& face_marks, float strength, cv::Mat& output) {
    const float alpha = 0.08f * min(max(strength, 0.0f), 10.0f);
    img.copyTo(output);

    for (const auto& face : face_marks) {
        if (face.landmarks.size() < 48) continue;

        // 计算眼中心
        Point2f leye(0, 0), reye(0, 0);
        for (int i = 36; i < 42; i++) { leye += face.landmarks[i]; }
        for (int i = 42; i < 48; i++) { reye += face.landmarks[i]; }
        leye /= 6; reye /= 6;

        // 计算变形半径
        float eyeDis = norm(leye - reye);
        float radius = eyeDis * 0.4f;

        // 逐像素变形
        for (int h = 0; h < output.rows; h++) {
            for (int w = 0; w < output.cols; w++) {
                if (h < 2 || h >= output.rows-2 || w < 2 || w >= output.cols-2) continue;

                // 计算到眼中心距离
                float dL = norm(Point2f(w, h) - leye);
                float dR = norm(Point2f(w, h) - reye);
                Point2f center = (dL < dR) ? leye : reye;
                float dis = (dL < dR) ? dL : dR;

                if (dis > radius) continue;

                // 变形权重计算
                float weight = dis / radius;
                weight = 1 - alpha * (1 - weight * weight);

                // 原图映射坐标
                float srcU = center.x + (w - center.x) * weight;
                float srcV = center.y + (h - center.y) * weight;
                srcU = max(0.0f, min(srcU, (float)img.cols - 2));
                srcV = max(0.0f, min(srcV, (float)img.rows - 2));

                // 双线性插值
                int u = (int)srcU, v = (int)srcV;
                float uFrac = srcU - u, vFrac = srcV - v;
                float w0 = (1 - vFrac) * (1 - uFrac);
                float w1 = (1 - vFrac) * uFrac;
                float w2 = vFrac * (1 - uFrac);
                float w3 = 1 - w0 - w1 - w2;

                int idx = (v * img.cols + u) * 3;
                int idxCol = idx + img.cols * 3;

                // BGR通道插值
                uchar b = img.data[idx] * w0 + img.data[idx+3] * w1 + img.data[idxCol] * w2 + img.data[idxCol+3] * w3;
                uchar g = img.data[idx+1] * w0 + img.data[idx+4] * w1 + img.data[idxCol+1] * w2 + img.data[idxCol+4] * w3;
                uchar r = img.data[idx+2] * w0 + img.data[idx+5] * w1 + img.data[idxCol+2] * w2 + img.data[idxCol+5] * w3;

                output.data[(h * img.cols + w) * 3] = saturate_cast<uchar>(b);
                output.data[(h * img.cols + w) * 3 + 1] = saturate_cast<uchar>(g);
                output.data[(h * img.cols + w) * 3 + 2] = saturate_cast<uchar>(r);
            }
        }
    }
}


/**
 * @brief 绘制面部轮廓（保留原逻辑，用于调试）
 */
void drawPolyline(Mat &im, const vector<Point2f> &landmarks, const int start, const int end, bool isClosed) {
    vector<Point> pts;
    for (int i = start; i <= end; i++) pts.push_back(Point(landmarks[i].x, landmarks[i].y));
    polylines(im, pts, isClosed, Scalar(255, 200, 0), 1, LINE_AA);
}



void whiteFace(Mat& matSelfPhoto,double alpha, double beta)
{
    for (int y = 0; y < matSelfPhoto.rows; y++)
    {
        for (int x = 0; x < matSelfPhoto.cols; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                matSelfPhoto.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(matSelfPhoto.at<Vec3b>(y, x)[c]) + beta);
            }
        }
    }
}

void PicInPic_Read::slot_getVideoFrame()
{
    Mat frame;
    if( !cap.read(frame) )
    {
        return;
    }
    //cvtColor将数据转为RGB，BGR
    cvtColor(frame,frame,CV_BGR2RGB);
    //做滤镜美颜
    if(m_whiteFaceFlag==1)
        whiteFace(frame, 1.1, 68);  // 调整对比度与亮度，参数2为对比度，参数3为亮度

//    //大眼特效
//    // 从文件读取模型数据（实际应用中可能从内存或资源中获取）
//    FILE* file = fopen("../../data/lbfmodel.yaml", "rb");
//    if (!file) {
//        cout << "Error: 无法读取模型文件" << endl;
//    }

//    fseek(file, 0, SEEK_END);
//    size_t model_size = ftell(file);
//    rewind(file);

    cv::Mat output=frame;
//    float strength = 5.0f; // 大眼力度，范围0-10
//    EnlargeEyes(frame, output, strength);



    QImage iconImage((unsigned const
                      char*)output.data,output.cols,output.rows,QImage::Format_RGB888);
    iconImage = iconImage.scaled( QSize(320,240) ,Qt::KeepAspectRatio );
    //投递画中画图片
    Q_EMIT SIG_sendPicInPic( iconImage.copy() );
    //获取桌面截图
    QScreen *src = QApplication::primaryScreen();
    QPixmap map = src->grabWindow( QApplication::desktop()->winId());
    //    //使用Windows的方式来 截取
    //    // 获取真实物理分辨率（无视DPI缩放）
    //    int width = GetDeviceCaps(hdcScreen, DESKTOPHORZRES);  // 物理宽度
    //    int height = GetDeviceCaps(hdcScreen, DESKTOPVERTRES); // 物理高度

    //    // 创建兼容的内存DC
    //    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    //    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
    //    SelectObject(hdcMem, hBitmap);

    //    // 拷贝屏幕数据到内存位图
    //    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    //    // 转换HBITMAP到QPixmap
    //    QPixmap pixmap = QtWin::fromHBITMAP(hBitmap);

    //转化为 RGB24 QImage
    QImage image = map.toImage().convertToFormat(QImage::Format_RGB888);
    //计算视频帧
    long long time = 0;
    uint8_t * picture_buf = NULL;
    int buffer_size = ImageToYuvBuffer( image , &picture_buf );
    Q_EMIT SIG_sendVideoFrameData( picture_buf, buffer_size );
    Q_EMIT SIG_sendVideoFrame( image );
}
int PicInPic_Read::ImageToYuvBuffer( QImage& image , uint8_t ** buffer )
{
    int w = image.width();
    int h = image.height();
    int y_size = w * h;
    // image.invertPixels(QImage::InvertRgb);
    //==================== 创建 RGB 对应的空间 =========================
    AVFrame *pFrameRGB = av_frame_alloc();
    // Determine required buffer size and allocate buffer
    int numBytes1 = avpicture_get_size(AV_PIX_FMT_RGB24, w, h);
    uint8_t *buffer1 = (uint8_t *)av_malloc(numBytes1*sizeof(uint8_t));
    avpicture_fill((AVPicture *)pFrameRGB, buffer1, AV_PIX_FMT_RGB24, w, h);
    pFrameRGB->data[0] = image.bits();
    //==================== 创建 YUV 对应的空间 =========================
    AVFrame *pFrameYUV = av_frame_alloc();
    // Determine required buffer size and allocate buffer
    int numBytes2 = avpicture_get_size(AV_PIX_FMT_YUV420P, w, h);
    uint8_t *buffer2 = (uint8_t *)av_malloc(numBytes2*sizeof(uint8_t));
    avpicture_fill((AVPicture *)pFrameYUV, buffer2, AV_PIX_FMT_YUV420P, w, h);
    // qWarning() << "numBytes2 " << numBytes2;
    //==================== 创建转化器 ================================
    SwsContext * rgb_to_yuv_ctx = sws_getContext(w,h, AV_PIX_FMT_RGB24,
                                                 w,h, AV_PIX_FMT_YUV420P,
                                                 SWS_BICUBIC, NULL,NULL,NULL);
    sws_scale(rgb_to_yuv_ctx, pFrameRGB->data, pFrameRGB->linesize, 0,
              h, pFrameYUV->data, pFrameYUV->linesize);
    //将转换完的数据提取到缓冲区
    uint8_t * picture_buf = (uint8_t *)av_malloc(numBytes2);
    memcpy(picture_buf,pFrameYUV->data[0],y_size);
    memcpy(picture_buf+y_size,pFrameYUV->data[1],y_size/4);
    memcpy(picture_buf+y_size+y_size/4,pFrameYUV->data[2],y_size/4);
    //写返回空间
    *buffer = picture_buf;
    //qWarning() << rc << endl;
    sws_freeContext(rgb_to_yuv_ctx);
    av_free(buffer1);
    av_free(buffer2);
    av_free(pFrameRGB);
    av_free(pFrameYUV);
    return y_size*3/2;
}
