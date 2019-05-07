#include "crtwidget.h"
#include <QApplication>
#include <QFile>


int main(int argc, char *argv[])
{
    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication a(argc, argv);
    QFile file(":/qss/styleSheet.qss");
    QByteArray array;
    if(file.open(QIODevice::ReadOnly))
    {
        array = file.readAll();
        file.close();
    }
    a.setStyleSheet(QString(array));

    CrtWidget w;
    w.showMaximized();

    return a.exec();
}


