#include "recorderdialog.h"
#include <QApplication>
#include "kernel.h"
#include "QGuiApplication"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    avformat_network_init();
    Kernel kernel;
    return a.exec();
}
