QT       += core gui
QT+= multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    audio_read.cpp \
    camerawidget.cpp \
    kernel.cpp \
    login.cpp \
    main.cpp \
    mainpage.cpp \
    messagedialog.cpp \
    network.cpp \
    picinpic_read.cpp \
    recorderdialog.cpp \
    register.cpp \
    room.cpp \
    savevediothread.cpp

HEADERS += \
    CJson.h \
    audio_read.h \
    camerawidget.h \
    common.h \
    kernel.h \
    login.h \
    mainpage.h \
    messagedialog.h \
    network.h \
    picinpic_read.h \
    recorderdialog.h \
    register.h \
    room.h \
    savevediothread.h

FORMS += \
    camerawidget.ui \
    login.ui \
    mainpage.ui \
    messagedialog.ui \
    recorderdialog.ui \
    register.ui \
    room.ui


INCLUDEPATH += $$PWD/ffmpeg-4.2.2/include\
               $$PWD/opencv-release/include/opencv2\
               $$PWD/opencv-release/include\
               $$PWD/opencv-4.5.4-with_contrib_qt_5.12.11_x86_mingw32/include\
               $$PWD/opencv-4.5.4-with_contrib_qt_5.12.11_x86_mingw32/include/opencv2

LIBS += $$PWD/ffmpeg-4.2.2/lib/avcodec.lib\
         $$PWD/ffmpeg-4.2.2/lib/avdevice.lib\
         $$PWD/ffmpeg-4.2.2/lib/avfilter.lib\
         $$PWD/ffmpeg-4.2.2/lib/avformat.lib\
         $$PWD/ffmpeg-4.2.2/lib/avutil.lib\
         $$PWD/ffmpeg-4.2.2/lib/postproc.lib\
         $$PWD/ffmpeg-4.2.2/lib/swresample.lib\
         $$PWD/ffmpeg-4.2.2/lib/swscale.lib\
         $$PWD/opencv-4.5.4-with_contrib_qt_5.12.11_x86_mingw32/lib/libopencv_*.dll.a

LIBS+=$$PWD/opencv-release/lib/libopencv_*.dll.a
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
