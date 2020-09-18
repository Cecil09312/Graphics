#include "crtwidget.h"
#include <QApplication>
#include <QFile>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QTransform>
#include <QObject>
#include "control/controller.h"
#include "dongle/checkdonglethread.h"
#include <QFontDatabase>
int main(int argc, char *argv[])
{

#ifdef Q_OS_LINUX
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads, true);
#endif
    QApplication a(argc, argv);
    // 创建信号量
    QSystemSemaphore semaphore("SingleAppSemaphore", 1);
    // 启用信号量，禁止其他实例通过共享内存一起工作

    semaphore.acquire();
    QSharedMemory sharedMemory("SingleAppSharedMemory");
    bool isRunning = false;
    // 试图将共享内存的副本附加到现有的段中。
    if (sharedMemory.attach())
    {
        // 如果成功，则确定已经存在运行实例
        isRunning = true;
    }
    else
    {
        // 否则申请一字节内存
        sharedMemory.create(1);
        // 确定不存在运行实例
        isRunning = false;
    }
    semaphore.release();

    // 如果您已经运行了应用程序的一个实例，那么我们将通知用户。
    if (isRunning)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("软件正在启动，数据量比较大，正在加载数据，请耐心等待！");
        msgBox.exec();
        return 1;
    }


    QFile file(":/qss/styleSheet.qss");
    QByteArray array;
    if(file.open(QIODevice::ReadOnly))
    {
        array = file.readAll();
        file.close();
    }
    a.setStyleSheet(QString(array));


    CrtWidget w;

    w.initArchiteView();
    w.showMaximized();
    w.setItemIconInfo();
    w.readDeviceOnlineInfoFromJson();
    Controller::instance()->getCommObj()->connectLink();
#ifdef Q_OS_LINUX
    Controller::instance()->getSpeechObj()->speechSetting();

    CheckDongleThread m_dongleThread;
    m_dongleThread.startTimerMs(5000);
    int num =0;
    QMessageBox messageBox(QMessageBox::Warning,QObject::tr("警告!"),QObject::tr("未识别到加密狗，请检查加密狗是否插好！"));
    messageBox.addButton(QMessageBox::Yes);
    messageBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);

    QObject::connect(&m_dongleThread,&CheckDongleThread::sendCheckDongleResult,&w,[&](int result)
    {

        if(result==1)
        {
            num=0;
            return;
        }
        else
        {
           num++;
        }

        if(num>=24)
        {
            m_dongleThread.startStopTimer();
            w.setDongleIsExist(false);
            w.lightsOff();
            w.close();
            num=0;
            messageBox.exec();
        }

    },Qt::QueuedConnection);
#endif

    return a.exec();
}


