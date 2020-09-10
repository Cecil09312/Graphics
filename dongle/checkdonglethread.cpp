#include "checkdonglethread.h"

CheckDongleThread::CheckDongleThread(QObject *parent)
    : QObject(parent)
{
    m_thread = new QThread;
    m_timer=new QTimer;
    this->moveToThread(m_thread);
    m_timer->moveToThread(m_thread);
    m_thread->start();
    connect(m_timer,&QTimer::timeout,this,[=]()
    {
       int value=  getCheckValue();
       emit sendCheckDongleResult(value);

    });

    connect(this,&CheckDongleThread::setTimer,this,[=](int value)
    {
        m_timer->start(value);
    });

    connect(this,&CheckDongleThread::stopTimer,m_timer,&QTimer::stop);

}

CheckDongleThread::~CheckDongleThread()
{
    startStopTimer();
    m_timer->deleteLater();
    m_thread->quit();
    m_thread->deleteLater();
}

int CheckDongleThread::getCheckValue()
{
    int fd = 0;
    int cmd;
    int arg = 0;
   // char Buf[4096]={0};
#ifdef Q_OS_LINUX

    /*打开设备文件*/
    fd = open("/dev/usbkey_test",O_RDWR);
    if (fd < 0)
    {
       // printf("Open Dev usbkey_test Error!\n");
       // arg= -1;
        close(fd);
        return  -1;
    }

    /* 调用命令USBKEYDEV_IOCPRINT */
   // printf("<--- Call USBKEYDEV_IOCPRINT --->\n");
    cmd = USBKEYDEV_IOCPRINT;
    if (ioctl(fd, cmd, &arg) < 0)
    {
        close(fd);
        return  -1;
    }
    /* 调用命令USBKEYDEV_IOCSETDATA */
   // printf("<--- Call USBKEYDEV_IOCSETDATA --->\n");
    cmd = USBKEYDEV_IOCSETDATA;
    arg = 2007;
    if (ioctl(fd, cmd, &arg) < 0)
    {
        // printf("Call cmd USBKEYDEV_IOCSETDATA fail\n");
        close(fd);
        return  -1;
    }

    /* 调用命令USBKEYDEV_IOCGETDATA */

    cmd = USBKEYDEV_IOCGETDATA;
    if (ioctl(fd, cmd, &arg) < 0)
    {
        close(fd);
        return  -1;
    }


    close(fd);
#endif
    return  arg;
}

void CheckDongleThread::startTimerMs(int mes)
{
    emit setTimer(mes);
}

void CheckDongleThread::startStopTimer()
{
    emit stopTimer();
}
