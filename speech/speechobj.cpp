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
    m_thread->setPriority(QThread::HighPriority);

    m_isStop = false;


#ifdef Q_OS_LINUX
   // m_languageHash[tr("粤语")] = "Cantonese";

    if(m_isEnglish)
    {
        m_languageHash[tr("英语")] = "English";
    }
    else
    {
        m_languageHash[tr("中文")] = "Chinese";
    }

   // m_languageHash[tr("台山话")] = "Toisanese";
    m_textToSpeechProcess = new QProcess;
    m_textToSpeechProcess->moveToThread(m_thread);
    // m_currentLanguage = "Mandarin";
    connect(m_textToSpeechProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        Q_UNUSED(exitCode);
        Q_UNUSED(exitStatus);
        if(!m_isStop)
        {
            repeatSpeak();
        }

    });
#endif
    QVariant speechValue=  m_speechJson.readFile(c_speechJsonDir);
    QHash<QString,QVariant> curSpeechHash=speechValue.toHash();
    if(curSpeechHash.isEmpty())
    {
#ifdef Q_OS_WIN
        m_pitch = 0;
        m_volume = 0.5;
        m_rate = 0;
#elif defined (Q_OS_LINUX)
        m_pitch = -5;

        m_rate = -15;
      if(m_isEnglish)
        {
           m_volume = 50.0;
            m_currentLanguage = "English";
        }
       else
        {
            m_volume = 1.0;
            m_currentLanguage = "Chinese";
        }

#endif
    }
    else
    {
        m_pitch=  curSpeechHash.value("pitch").toReal();
        m_rate = curSpeechHash.value("rate").toReal();
        m_volume = curSpeechHash.value("volume").toReal();
        m_currentLanguage = curSpeechHash.value("language").toString();

    }


    connect(this,&SpeechObj::speechStart,this,&SpeechObj::runSpeech);
    connect(this,&SpeechObj::textToSpeechStop,this,&SpeechObj::speechStop);
    connect(this,&SpeechObj::insertText,this,[&](const QString &alarmText)
    {
        m_isStop = false;
        if(!m_alarmTextList.contains(alarmText))
        {
            if(alarmText.startsWith(tr("首火警"),Qt::CaseInsensitive))
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
            else if(alarmText.startsWith(tr("火警"),Qt::CaseInsensitive))
            {
                int fireAlarmIndex =0;
                foreach (QString alarmValue, m_alarmTextList)
                {
                    if(alarmValue.startsWith(tr("火警"),Qt::CaseInsensitive)||alarmValue.startsWith(tr("首火警"),Qt::CaseInsensitive))
                    {
                        fireAlarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                    }
                }
                m_alarmTextList.insert(fireAlarmIndex,alarmText);
                m_currentAlarmPos = fireAlarmIndex;
            }
            else if(alarmText.startsWith(tr("监管"),Qt::CaseInsensitive)||alarmText.startsWith(tr("启动"),Qt::CaseInsensitive)||alarmText.startsWith(tr("反馈"),Qt::CaseInsensitive)||alarmText.startsWith(tr("故障"),Qt::CaseInsensitive)||alarmText.startsWith(tr("屏蔽"),Qt::CaseInsensitive))
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
                if(alarmText.contains(tr("首火警"),Qt::CaseInsensitive))
                {
                    m_alarmPos=0;
                    m_currentAlarmPos =0;
                    break;
                }
            }


        }

        runSpeech();
        m_thread->msleep(5);

    });


    connect(this,&SpeechObj::clearText,this,[=]()
    {
        m_alarmTextList.clear();
        m_alarmPos=0;
    });
    connect(this,QOverload<int>::of(&SpeechObj::removeText),this,[&](int pos)
    {

        if(m_alarmTextList.size()>pos)
        {
            m_alarmTextList.removeAt(pos);

        }
        if(m_alarmPos>m_alarmTextList.size())
        {
            m_alarmPos =0;
        }

    });

    connect(this,QOverload<const QString &>::of(&SpeechObj::removeText),this,[&](const QString & alarmText)
    {
        if(m_alarmTextList.contains(alarmText))
        {
            m_alarmTextList.removeOne(alarmText);
        }
        if(m_alarmPos>m_alarmTextList.size())
        {
            m_alarmPos =0;
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
    if(m_isEnglish)
    {
        return  -100;
    }
    else
    {
        return 0;
    }

#endif
}

double SpeechObj::volumeMax()
{
#ifdef Q_OS_WIN
    return 1;
#elif defined Q_OS_LINUX
    if(m_isEnglish)
    {

        return  100.0;
    }
    else
    {
        return 2.0;
    }

#endif
}

double SpeechObj::volumeStep()
{
#ifdef Q_OS_WIN
    return 0.1;
#elif defined Q_OS_LINUX
    if(m_isEnglish)
    {
        return  1.0;
    }
    else
    {
        return 0.1;
    }

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
    //    m_pitch = m_textToSpeech->pitch();
    //    m_volume = m_textToSpeech->volume();
    //    m_rate = m_textToSpeech->rate();

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

void SpeechObj::saveSpeechInfoToJson()
{
    QHash<QString,QVariant>speechHash;
    speechHash["volume"] = m_volume;
    speechHash["rate"] = m_rate;
    speechHash["pitch"] = m_pitch;
    speechHash["language"]=m_currentLanguage;

    m_speechJson.writeFile(speechHash,c_speechJsonDir);
}

void SpeechObj::setIsEnglish(bool is)
{
    m_isEnglish = is;
    emit languageChangeToEnglish(is);
}

bool SpeechObj::isEnglish()
{
    return m_isEnglish;
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
                if(m_alarmPos>=m_alarmTextList.size())
                {
                    m_alarmPos =0;
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
            if(m_isEnglish)
            {
                m_textToSpeechProcess->start(QString("ekho -v %1 -p %2 -a %3 -s %4 '%5'").arg(m_currentLanguage).arg(m_pitch).arg(m_volume).arg(m_rate).arg(curSpeechText));
            }
            else
            {
                m_textToSpeechProcess->start(QString("play -v %1 %2").arg(m_volume).arg(QCoreApplication::applicationDirPath()+"/media/"+ curSpeechText+".mp3"));
            }




#endif
        }
        else
        {
            m_alarmPos =0;
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
    repeatSpeak();
#endif


}

void SpeechObj::speechStop()
{


#ifdef Q_OS_WIN
    m_textToSpeech->pause();
#elif defined Q_OS_LINUX
    m_isStop = true;
    m_textToSpeechProcess->kill();
    m_textToSpeechProcess->terminate();
#endif
}

int SpeechObj::indexOfType(const QString &type)
{
    int alarmIndex =0;
    foreach (QString alarmValue, m_alarmTextList)
    {
        if(alarmValue.startsWith(tr("火警"),Qt::CaseInsensitive)||alarmValue.startsWith(tr("首火警"),Qt::CaseInsensitive))
        {
            alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
        }
        else
        {
            if(type.startsWith(tr("监管"),Qt::CaseInsensitive))
            {
                if(alarmValue.startsWith(tr("监管"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }

            }
            else if(type.startsWith(tr("启动"),Qt::CaseInsensitive))
            {
                if(alarmValue.startsWith(tr("监管"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith(tr("启动"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }

            }
            else if(type.startsWith(tr("反馈"),Qt::CaseInsensitive))
            {
                if(alarmValue.startsWith(tr("监管"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith(tr("启动"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith(tr("反馈"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }

            }
            else if(type.startsWith(tr("故障"),Qt::CaseInsensitive))
            {
                if(alarmValue.startsWith(tr("监管"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith(tr("启动"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith(tr("反馈"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith(tr("故障"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }

            }
            else if(type.startsWith(tr("屏蔽"),Qt::CaseInsensitive))
            {
                if(alarmValue.startsWith(tr("监管"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith(tr("启动"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith(tr("反馈"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith(tr("故障"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }
                else if(alarmValue.startsWith(tr("屏蔽"),Qt::CaseInsensitive))
                {
                    alarmIndex= m_alarmTextList.indexOf(alarmValue)+1;
                }

            }
        }

    }
    return alarmIndex;
}
