#include "speechobj.h"

SpeechObj::SpeechObj(QObject *parent):
    QTextToSpeech(parent)
{
    m_alarmPos =0;
    m_thread = new QThread;
    this->moveToThread(m_thread);
    m_thread->start();
    connect(this,&SpeechObj::stateChanged,this,[=](QTextToSpeech::State state)
    {
        if(state==QTextToSpeech::Ready)
        {
            int alarmTestListSize =m_alarmTextList.size();
            if(alarmTestListSize>0)
            {
                if(m_alarmPos<alarmTestListSize)
                {
                    say(m_alarmTextList.at(m_alarmPos));
                }
                m_alarmPos++;
                m_alarmPos = m_alarmPos%alarmTestListSize;
            }
        }
    });
}

SpeechObj::~SpeechObj()
{
    m_thread->quit();
    m_thread->wait();
    m_thread->deleteLater();
}

void SpeechObj::stopSpeech()
{
    disconnect(this,&SpeechObj::stateChanged,0,0);
    stop();
}

void SpeechObj::startSpeech()
{
    if(m_alarmTextList.size()>0)
    {
        say(m_alarmTextList.at(0));
    }
}

void SpeechObj::insertAlarmText(const QString &alarmText)
{
    m_alarmTextList.push_back(alarmText);
}

void SpeechObj::clearAlarmText()
{
    m_alarmTextList.clear();
}

void SpeechObj::removeAlarmText(const QString &alarmText)
{
    m_alarmTextList.removeOne(alarmText);
}
