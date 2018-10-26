#include "crtwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QApplication a(argc, argv);
    CrtWidget w;
    w.showFullScreen();

    return a.exec();
}
