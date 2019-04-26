#include "crtwidget.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include "control/controller.h"
#include <QProcess>
//#include <QWindowsWindowFunctions>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
void hideTaskBar()
{
    //隐藏任务栏
#ifdef Q_OS_WIN
  HWND hWnd = ::FindWindow(TEXT("Shell_traywnd"),TEXT(""));
  ::SetWindowPos(hWnd,0,0,0,0,0,SWP_HIDEWINDOW);
#endif

#ifdef Q_OS_LINUX

//  system(/*"gsettings set org.gnome.shell.extensions.dash-to-dock autohide false"
//         "gsettings set org.gnome.shell.extensions.dash-to-dock dock-fixed false"*/
//         "gnome-terminal -x bash -c  'gsettings set org.gnome.shell.extensions.dash-to-dock intellihide false'");
//  QProcess process;
 // process.execute(/*"gnome-terminal -x'gsettings set org.gnome.shell.extensions.dash-to-dock autohide false'"*/
//            "gsettings set org.gnome.shell.extensions.dash-to-dock dock-fixed false"
 //           "gsettings set org.gnome.shell.extensions.dash-to-dock intellihide false");
//  process.waitForStarted();
//  process.waitForFinished();
#endif

}
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

   // hideTaskBar();
    CrtWidget w;
    w.showMaximized();

    return a.exec();
}


