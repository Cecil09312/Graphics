#ifndef CHECKDONGLETHREAD_H
#define CHECKDONGLETHREAD_H

#include <QObject>
#include <QThread>
#include "dongle/usbkeydev.h"
#include <QTimer>
#ifdef Q_OS_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "dongle/hasp_api.h"
#include "dongle/hasp_vcode.h"
#endif
class CheckDongleThread : public QObject
{
    Q_OBJECT
public:
    explicit CheckDongleThread(QObject *parent = nullptr);
    ~CheckDongleThread();
    int getCheckValue();
    bool getValue();
    void startTimerMs(int mes);
    void startStopTimer();

signals:
    void sendCheckDongleResult(bool isOk);
    void setTimer(int mes);
    void stopTimer();
public slots:
private:
    QThread *m_thread;
    QTimer*m_timer;
    bool m_loginState{false};

};

#endif // CHECKDONGLETHREAD_H
