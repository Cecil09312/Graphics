#include "customtimer.h"
#include <QDebug>
#include "control/controller.h"
CustomTimer::CustomTimer(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    m_timer = new QTimer;

    m_timer->moveToThread(m_thread);
    this->moveToThread(m_thread);


    connect(this,&CustomTimer::stopTimer,this,[=](){
        m_timer->stop();
    });
    connect(this,&CustomTimer::startTimerMs,this,[=](int ms){
        m_timer->start(ms);
    });
    connect(m_timer,&QTimer::timeout,this,&CustomTimer::timeout);

}

CustomTimer::~CustomTimer()
{
    stop();
    m_timer->deleteLater();
    if(m_thread!=nullptr)
    {
        m_thread->quit();
        m_thread->deleteLater();
    }

}

void CustomTimer::start(int ms)
{
    if(!m_thread->isRunning())
    {
        m_thread->start();
    }
    emit startTimerMs(ms);
}

void CustomTimer::stop()
{
    emit stopTimer();
}

bool CustomTimer::isActive()
{
    return m_timer->isActive();
}

void CustomTimer::setSingleShot(bool singleShot)
{
    m_timer->setSingleShot(singleShot);
}
