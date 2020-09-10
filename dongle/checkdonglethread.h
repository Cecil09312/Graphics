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
#endif
class CheckDongleThread : public QObject
{
    Q_OBJECT
public:
    explicit CheckDongleThread(QObject *parent = nullptr);
    ~CheckDongleThread();
    int getCheckValue();
    void startTimerMs(int mes);
    void startStopTimer();

signals:
    void sendCheckDongleResult(int result);
    void setTimer(int mes);
    void stopTimer();
public slots:
private:
    QThread *m_thread;
    QTimer*m_timer;
};

#endif // CHECKDONGLETHREAD_H
