#include "speechobj.h"
#include <QDebug>
SpeechObj::SpeechObj(QObject *parent):
    QTextToSpeech(parent)
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
                m_alarmPos++;
                m_alarmPos = m_alarmPos%alarmTestListSize;
            }
        }
    });


}

SpeechObj::~SpeechObj()
{
    qDebug() << "SpeechObj";
    stopSpeech();
    m_thread->quit();
    m_thread->deleteLater();
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
