#include "speechobj.h"
#include <QDebug>
#include <QtConcurrent>
#include <QFuture>
SpeechObj::SpeechObj(QObject *parent):
    QTextToSpeech(parent),
    m_isStoped(false),
    m_currentAlarmPos(0)
{
    m_alarmPos =0;
    m_thread = new QThread;
    this->moveToThread(m_thread);
    m_thread->start();
    // m_voiceVec = availableVoices();
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
                if(m_alarmTextList.at(m_alarmPos).startsWith("首火警")||m_alarmTextList.at(m_alarmPos).startsWith("火警"))
                {
                    m_alarmPos++;
                    m_alarmPos = m_alarmPos%alarmTestListSize;
                }
            }
        }
    });


}

SpeechObj::~SpeechObj()
{
    stopSpeech();
    m_thread->quit();
    m_thread->deleteLater();
}

int SpeechObj::currentAlarmPos()
{
    return m_currentAlarmPos;
}

//void SpeechObj::setSelectVoice(int index)
//{
//    if(m_voiceVec.size()>index)
//    {
//        setVoice(m_voiceVec.at(index));
//    }

//}

//QList<QString> SpeechObj::voiceNameList()
//{
//    return m_voiceNameList;
//}

//int SpeechObj::numOfVoice()
//{
//    return availableVoices().size();
//}

//QString SpeechObj::voiceName(int index)
//{
//    if(m_voiceNameList.size()>index)
//    {
//        return m_voiceNameList.at(index);
//    }
//    else
//    {
//        return QString();
//    }
//}

//QString SpeechObj::currentVoiceName()
//{
//   return (voice().name()+"-年龄:"+QVoice::ageName(voice().age())+"-性别:"+QVoice::genderName(voice().gender()));
//}

void SpeechObj::stopSpeech()
{
    disconnect(this,&SpeechObj::stateChanged,0,0);
    stop();
    m_isStoped = false;
}

void SpeechObj::startSpeech()
{
    if(!m_isStoped)
    {
        if(m_alarmTextList.size()>0)
        {
            say(m_alarmTextList.at(0));
            m_isStoped = true;
        }
    }
}

void SpeechObj::insertAlarmText(const QString &alarmText)
{
    QFuture<void> future = QtConcurrent::run([&]()
    {
        if(alarmText.startsWith("首火警"))
        {
            if(m_alarmTextList.size()>0)
            {
                m_alarmTextList.insert(0,alarmText);
                m_currentAlarmPos = 0;
            }
            else
            {
                m_alarmTextList.push_back(alarmText);
                m_currentAlarmPos = m_alarmTextList.size()-1;
            }
        }
        else if(alarmText.startsWith("火警"))
        {
            int fireAlarmIndex =0;
            foreach (QString alarmValue, m_alarmTextList)
            {
                if(alarmValue.startsWith("火警")||alarmValue.startsWith("首火警"))
                {
                    fireAlarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
            }
            m_alarmTextList.insert(fireAlarmIndex,alarmText);
            m_currentAlarmPos = fireAlarmIndex;
        }
        else
        {
            m_alarmTextList.push_back(alarmText);
            m_currentAlarmPos = m_alarmTextList.size()-1;
        }

    });
    future.waitForFinished();
    startSpeech();
}

void SpeechObj::clearAlarmText()
{
    m_alarmTextList.clear();
}

void SpeechObj::removeAlarmText(const QString &alarmText)
{
    m_alarmTextList.removeOne(alarmText);
}

void SpeechObj::removeAlarmText(int pos)
{
    if(m_alarmTextList.size()>pos)
    {
        m_alarmTextList.removeAt(pos);
    }
}
