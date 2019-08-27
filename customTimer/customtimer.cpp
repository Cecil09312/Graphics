#include "customtimer.h"
#include <QDebug>

CustomTimer::CustomTimer(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer;
    m_thread = new QThread();
    m_timer->moveToThread(m_thread);
    this->moveToThread(m_thread);
    m_thread->start();
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
    m_thread->quit();
    m_thread->deleteLater();
}

void CustomTimer::start(int ms)
{
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
