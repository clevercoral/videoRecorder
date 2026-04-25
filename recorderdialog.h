#ifndef RECORDERDIALOG_H
#define RECORDERDIALOG_H

#include <QDialog>
#include "camerawidget.h"
#include "savevediothread.h"
#include "room.h"

QT_BEGIN_NAMESPACE
namespace Ui { class recorderDialog; }
QT_END_NAMESPACE

class recorderDialog : public QDialog
{
    Q_OBJECT

public:
    recorderDialog(QWidget *parent = nullptr);
    ~recorderDialog();
    void setUrl(QString url);
    Room *getRoom() const;

    void setRoom(Room *newRoom);

signals:
    void startAlive();
public slots:
    void on_pb_start_clicked();

    void on_pb_end_clicked();

    void on_pb_setUrl_clicked();

    void slot_setImage(QImage img);//设置图像

private slots:
    void on_pb_whiteFace_clicked();

private:
    Ui::recorderDialog *ui;
    cameraWidget* m_camera;
    saveVedioThread* m_save;
    QString m_fileUrl;
    Room* m_room;
};
#endif // RECORDERDIALOG_H
