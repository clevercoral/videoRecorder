#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QWidget>

namespace Ui {
class MessageDialog;
}
//生活，音乐，体育，游戏
struct roomInfo
{
    QString m_roomId;
    QString m_roomTitle;
    QString m_link;
    int m_type;
};
class MessageDialog : public QWidget
{
    Q_OBJECT

public:
    explicit MessageDialog(QWidget *parent = nullptr);
    ~MessageDialog();

    void setRoomId(QString roomId);
signals:
    void sendMsg(roomInfo& info);
private slots:
    void on_pb_configure_clicked();

private:
    Ui::MessageDialog *ui;
};

#endif // MESSAGEDIALOG_H
