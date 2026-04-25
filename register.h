#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

signals:
    void sendRegisterMsg(QString userName,QString tel,QString password);


private slots:
    void on_pb_commit_clicked();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
