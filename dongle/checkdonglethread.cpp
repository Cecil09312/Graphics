#include "checkdonglethread.h"
#include <QDebug>
#ifdef Q_OS_LINUX
#define DEMO_MEMBUFFER_SIZE   128
unsigned char membuffer[DEMO_MEMBUFFER_SIZE];
#endif


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
      bool valueState1=false;
      if(value==1)
      {
          valueState1=true;
      }

      bool valueState2= getValue();
       emit sendCheckDongleResult(valueState1||valueState2);

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
#ifdef Q_OS_LINUX
    hasp_handle_t   handle;
    hasp_logout(handle);
#endif
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

bool CheckDongleThread::getValue()
{
    bool curState=false;
#ifdef Q_OS_LINUX
       hasp_status_t   status;
       hasp_handle_t   handle;
     //  hasp_time_t     time;
       hasp_size_t     fsize;
           status = hasp_login(HASP_DEFAULT_FID,
                           (hasp_vendor_code_t)s_vendor_code,
                           &handle);
           if(status==HASP_STATUS_OK)
           {
              // m_loginState=true;
               status = hasp_read(handle, HASP_FILEID_RW, 0,                 /* offset */
                                  fsize,             /* length */
                                  &membuffer);
               if(status==HASP_STATUS_OK)
               {
                   curState = true;
               }
               else
               {
                   curState = false;

               }
           }
           else
           {
               curState = false;
           }


  #endif

       return  curState;
}

void CheckDongleThread::startTimerMs(int mes)
{
    emit setTimer(mes);
}

void CheckDongleThread::startStopTimer()
{
    emit stopTimer();
}
