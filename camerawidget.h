#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>

namespace Ui {
class cameraWidget;
}

class cameraWidget : public QWidget
{
    Q_OBJECT

public:
    explicit cameraWidget(QWidget *parent = nullptr);
    ~cameraWidget();
public slots:
    void slot_getImage(QImage img);

private:
    Ui::cameraWidget *ui;
};

#endif // CAMERAWIDGET_H
