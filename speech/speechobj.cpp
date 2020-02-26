#include "speechobj.h"
#include <QDebug>
#include <QtConcurrent>
#include <QFuture>
#include <QMutexLocker>
#include <QtConcurrent>

SpeechObj::SpeechObj(QObject *parent):
    QObject(parent),
    m_isStoped(false),
    m_currentAlarmPos(0)
{

#ifdef Q_OS_WIN
    m_textToSpeech = nullptr;
#endif  
    m_alarmPos =0;
    m_thread = new QThread;
    engineSelected("default");
    this->moveToThread(m_thread);
    m_thread->start();

#ifdef Q_OS_WIN
    m_pitch = 0;
    m_volume = 0.5;
    m_rate = 0;
#elif defined (Q_OS_LINUX)
    m_languageHash["粤语"] = "Cantonese";
    m_languageHash["普通话"] = "Mandarin";
    m_languageHash["台山话"] = "Toisanese";
    m_textToSpeechProcess = new QProcess;
    m_startTimer = new QTimer();
    m_startTimer->moveToThread(m_thread);
    m_startTimer->setInterval(100);
    m_textToSpeechProcess->moveToThread(m_thread);
    m_pitch = 0;
    m_volume = 70;
    m_rate = 100;
    m_currentLanguage = "Mandarin";

    connect(m_startTimer,&QTimer::timeout,this,&SpeechObj::repeatSpeak);
    connect(m_thread,SIGNAL(started()),m_startTimer,SLOT(start()));
    connect(m_thread,&QThread::finished,m_startTimer,&QTimer::stop);
#endif

    connect(this,&SpeechObj::speechStart,this,&SpeechObj::runSpeech);
    connect(this,&SpeechObj::textToSpeechStop,this,&SpeechObj::speechStop);
    connect(this,&SpeechObj::insertText,this,[&](const QString &alarmText)
    {
        if(!m_alarmTextList.contains(alarmText))
        {
            if(alarmText.startsWith("首火警"))
            {
                if(m_alarmTextList.size()>0)
                {
                    m_alarmTextList.insert(0,alarmText);

                }
                else
                {
                    m_alarmTextList.push_back(alarmText);
                }
                m_currentAlarmPos = 0;
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
            else if(alarmText.startsWith("监管")||alarmText.startsWith("启动")||alarmText.startsWith("反馈")||alarmText.startsWith("故障")||alarmText.startsWith("屏蔽"))
            {
                int curIndex= indexOfType(alarmText);
                m_alarmTextList.insert(curIndex,alarmText);
                m_currentAlarmPos = curIndex;
            }
            else
            {
                m_alarmTextList.push_back(alarmText);
                m_currentAlarmPos = m_alarmTextList.size()-1;
            }
            foreach (QString alarmText, m_alarmTextList)
            {
                if(alarmText.contains("首火警"))
                {
                    m_alarmPos=0;
                    break;
                }
            }

            runSpeech();
        }

    });


    connect(this,&SpeechObj::clearText,this,[=]()
    {
        m_alarmTextList.clear();
    });
    connect(this,QOverload<int>::of(&SpeechObj::removeText),this,[&](int pos)
    {

        if(m_alarmTextList.size()>pos)
        {
            m_alarmTextList.removeAt(pos);
        }

    });

    connect(this,QOverload<const QString &>::of(&SpeechObj::removeText),this,[&](const QString & alarmText)
    {
        if(m_alarmTextList.contains(alarmText))
        {
            m_alarmTextList.removeOne(alarmText);
        }
    });


}

SpeechObj::~SpeechObj()
{
    stopSpeech();
#ifdef Q_OS_WIN
    disconnect(m_textToSpeech,&QTextToSpeech::stateChanged,nullptr,nullptr);
    m_textToSpeech->deleteLater();

#elif defined Q_OS_LINUX
    m_startTimer->deleteLater();
    m_textToSpeechProcess->terminate();
    m_textToSpeechProcess->close();
    m_textToSpeechProcess->deleteLater();
#endif
    m_thread->quit();
    m_thread->deleteLater();

}

int SpeechObj::currentAlarmPos()
{
    return m_currentAlarmPos;
}

QList<QString> &SpeechObj::alarmTextList()
{
    return m_alarmTextList;
}

//QString &SpeechObj::alarmText(int pos)
//{
//    int size =alarmTextList().size();
//    if(size>pos)
//    {
//        return alarmTextList().at(pos);
//    }
//    else
//    {
//        return "";
//    }
//}

double SpeechObj::rate()
{
    return m_rate;
}

double SpeechObj::volume()
{
    return m_volume;
}

double SpeechObj::pitch()
{
    return m_pitch;
}

void SpeechObj::setRate(double rate)
{
    m_rate = rate;
#ifdef Q_OS_WIN
    m_textToSpeech->setRate(rate);
#endif
}

void SpeechObj::setVolume(double volume)
{
    m_volume = volume;
#ifdef Q_OS_WIN
    m_textToSpeech->setVolume(volume);
#endif
}

void SpeechObj::setPitch(double pitch)
{
    m_pitch = pitch;
#ifdef Q_OS_WIN
    m_textToSpeech->setPitch(pitch);
#endif
}

double SpeechObj::pitchMin()
{
#ifdef Q_OS_WIN
    return -1.0;
#elif defined Q_OS_LINUX
    return -100.0;
#endif


}

double SpeechObj::pitchMax()
{
#ifdef Q_OS_WIN
    return 1.0;
#elif defined Q_OS_LINUX
    return 100.0;
#endif
}

double SpeechObj::pitchStep()
{
#ifdef Q_OS_WIN
    return 0.1;
#elif defined Q_OS_LINUX
    return 1.0;
#endif
}

double SpeechObj::rateMin()
{
#ifdef Q_OS_WIN
    return -1.0;
#elif defined Q_OS_LINUX
    return -50.0;
#endif
}

double SpeechObj::rateMax()
{
#ifdef Q_OS_WIN
    return 1.0;
#elif defined Q_OS_LINUX
    return 300.0;
#endif
}

double SpeechObj::rateStep()
{
#ifdef Q_OS_WIN
    return 0.1;
#elif defined Q_OS_LINUX
    return 1.0;
#endif
}

double SpeechObj::volumeMin()
{
#ifdef Q_OS_WIN
    return 0;
#elif defined Q_OS_LINUX
    return -100.0;
#endif
}

double SpeechObj::volumeMax()
{
#ifdef Q_OS_WIN
    return 1;
#elif defined Q_OS_LINUX
    return 100.0;
#endif
}

double SpeechObj::volumeStep()
{
#ifdef Q_OS_WIN
    return 0.1;
#elif defined Q_OS_LINUX
    return 1.0;
#endif
}

bool SpeechObj::alarmTextExist(const QString &alarmText)
{
    return m_alarmTextList.contains(alarmText);
}

void SpeechObj::engineSelected(const QString &engineName)
{
#ifdef Q_OS_WIN
    if(m_textToSpeech!=nullptr)
    {
        disconnect(m_textToSpeech,&QTextToSpeech::stateChanged,nullptr,nullptr);
        m_textToSpeech->deleteLater();
        m_textToSpeech = nullptr;
    }

    if(engineName=="default" ||engineName.isEmpty())
    {
        m_textToSpeech = new QTextToSpeech();
    }
    else
    {
        m_textToSpeech = new QTextToSpeech(engineName);
    }
    m_textToSpeech->moveToThread(m_thread);
    m_languageHash.clear();
    m_engineNameList.clear();
    m_pitch = m_textToSpeech->pitch();
    m_volume = m_textToSpeech->volume();
    m_rate = m_textToSpeech->rate();

    setEngineNameList(QTextToSpeech::availableEngines());

    QVector<QLocale>locales= m_textToSpeech->availableLocales();
    foreach (const QLocale &locale, locales) {
        QString name(QString("%1 (%2)")
                     .arg(QLocale::languageToString(locale.language()))
                     .arg(QLocale::countryToString(locale.country())));
        QVariant localeVariant(locale);
        m_languageHash[name] = localeVariant;
    }

    connect(m_textToSpeech,&QTextToSpeech::stateChanged,this,[=](QTextToSpeech::State state)
    {
        if(state==QTextToSpeech::Ready)
        {
            repeatSpeak();
        }

    });
    startSpeech();

#elif defined Q_OS_LINUX
    Q_UNUSED(engineName);

#endif
}

QStringList SpeechObj::engineNameList()
{
    return m_engineNameList;
}

void SpeechObj::setEngineNameList(const QStringList &engine)
{
    m_engineNameList = engine;
}

QList<QString> SpeechObj::languageNameList()
{
    return m_languageHash.keys();
}

QList<QVariant> SpeechObj::languageValueList()
{
    return m_languageHash.values();
}



QString SpeechObj::engineName(int index)
{
    if(index<m_engineNameList.size())
    {
        return m_engineNameList.at(index);

    }
    else
    {
        return "";
    }

}

int SpeechObj::engineNameNum()
{
    return m_engineNameList.size();
}

QString SpeechObj::languageName(int index)
{
    QList<QString> languageList= m_languageHash.keys();
    if(languageList.size()>index)
    {
        return languageList.at(index);
    }
    else
    {
        return "";
    }
}

int SpeechObj::languageNum()
{
    return m_languageHash.size();
}

void SpeechObj::setLanguage(const QString &languageName)
{
#ifdef Q_OS_WIN
    m_textToSpeech->setLocale(m_languageHash[languageName].toLocale());

#elif defined Q_OS_LINUX
    m_currentLanguage= m_languageHash[languageName].toString();
#endif
}

QString SpeechObj::currentLanguage()
{
#ifdef Q_OS_WIN
    return  m_languageHash.key(m_textToSpeech->locale());

#elif defined Q_OS_LINUX
    return m_languageHash.key(m_currentLanguage);
#endif

}


void SpeechObj::stopSpeech()
{
    emit textToSpeechStop();
    //speechStop();
}

void SpeechObj::startSpeech()
{
    emit speechStart();
}

void SpeechObj::insertAlarmText(const QString &alarmText)
{
    emit insertText(alarmText);
}

void SpeechObj::clearAlarmText()
{
    emit clearText();
}

void SpeechObj::removeAlarmText(const QString &alarmText)
{
    emit removeText(alarmText);

}

void SpeechObj::removeAlarmText(int pos)
{

    emit removeText(pos);
}

void SpeechObj::repeatSpeak()
{
    if(m_alarmTextList.size()>0)
    {
        if(m_alarmPos<m_alarmTextList.size())
        {
            QString curSpeechText = m_alarmTextList.at(m_alarmPos);
            int index = curSpeechText.indexOf(";");
            if(index>0)
            {
                curSpeechText = curSpeechText.left(index);
            }

            if((!curSpeechText.startsWith("首火警"))&&(!curSpeechText.startsWith("火警")))
            {
                m_alarmPos++;
                if(m_alarmTextList.size()>0)
                {
                    m_alarmPos = m_alarmPos%m_alarmTextList.size();
                }

            }
            else
            {
                m_alarmPos =0;
            }
#ifdef Q_OS_WIN
            m_textToSpeech->say(curSpeechText);

#elif defined Q_OS_LINUX

            //if(m_textToSpeechProcess->state()==QProcess::NotRunning)
            m_textToSpeechProcess->start(QString("ekho -v %1 -p %2 -a %3 -s %4 '%5'").arg(m_currentLanguage).arg(m_pitch).arg(m_volume).arg(m_rate).arg(curSpeechText));
           // m_textToSpeechProcess->waitForStarted();
           // m_textToSpeechProcess->waitForFinished();

#endif
        }
    }
    else {
        speechStop();
    }
}

void SpeechObj::runSpeech()
{

#ifdef Q_OS_WIN
    if(m_alarmTextList.size()>0)
    {

        if(m_textToSpeech->state()!=QTextToSpeech::Speaking)
        {
            QString curAlarmInfo= m_alarmTextList.at(0);
            int index = curAlarmInfo.indexOf(";");
            if(index>0)
            {
                curAlarmInfo = curAlarmInfo.left(index);
            }
            m_textToSpeech->say(curAlarmInfo);

        }
    }


#elif defined Q_OS_LINUX
    if(!m_startTimer->isActive())
    {
        m_startTimer->start();
        m_alarmPos =0;
    }

#endif
}

void SpeechObj::speechStop()
{


#ifdef Q_OS_WIN
    m_textToSpeech->pause();
#elif defined Q_OS_LINUX
    //m_textToSpeechProcess->kill();
    m_textToSpeechProcess->terminate();
    m_startTimer->stop();
#endif
}

int SpeechObj::indexOfType(const QString &type)
{
    int alarmIndex =0;
    foreach (QString alarmValue, m_alarmTextList)
    {
        if(alarmValue.startsWith("火警")||alarmValue.startsWith("首火警"))
        {
            alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
        }
        else
        {
            if(type.startsWith("监管"))
            {
                if(alarmValue.startsWith("监管"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }

            }
            else if(type.startsWith("启动"))
            {
                if(alarmValue.startsWith("监管"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith("启动"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }

            }
            else if(type.startsWith("反馈"))
            {
                if(alarmValue.startsWith("监管"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith("启动"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith("反馈"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }

            }
            else if(type.startsWith("故障"))
            {
                if(alarmValue.startsWith("监管"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith("启动"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith("反馈"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith("故障"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }

            }
            else if(type.startsWith("屏蔽"))
            {
                if(alarmValue.startsWith("监管"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith("启动"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith("反馈"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith("故障"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith("屏蔽"))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }

            }
        }

    }
    return alarmIndex;
}
