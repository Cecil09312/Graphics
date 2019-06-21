#include "speechobj.h"
#include <QDebug>
#include <QtConcurrent>
#include <QFuture>
SpeechObj::SpeechObj(QObject *parent):
    QObject(parent),
    m_isStoped(false),
    m_currentAlarmPos(0),
    m_textToSpeech(nullptr)
{
    m_alarmPos =0;

    m_thread = new QThread;
    engineSelected("default");
    this->moveToThread(m_thread);
    m_thread->start();




}

SpeechObj::~SpeechObj()
{
    stopSpeech();
    disconnect(m_textToSpeech,&QTextToSpeech::stateChanged,nullptr,nullptr);
    m_thread->quit();
    m_thread->deleteLater();
    m_textToSpeech->deleteLater();
}

int SpeechObj::currentAlarmPos()
{
    return m_currentAlarmPos;
}

QList<QString> &SpeechObj::alarmTextList()
{
    return m_alarmTextList;
}

double SpeechObj::rate()
{
    return m_textToSpeech->rate();
}

double SpeechObj::volume()
{
    return m_textToSpeech->volume();
}

double SpeechObj::pitch()
{
    return m_textToSpeech->pitch();
}

void SpeechObj::setRate(double rate)
{
    m_textToSpeech->setRate(rate);
}

void SpeechObj::setVolume(double volume)
{
    m_textToSpeech->setVolume(volume);
}

void SpeechObj::setPitch(double pitch)
{
    m_textToSpeech->setPitch(pitch);
}

bool SpeechObj::alarmTextExist(const QString &alarmText)
{
    return m_alarmTextList.contains(alarmText);
}

void SpeechObj::engineSelected(const QString &engineName)
{
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
             int alarmTestListSize =m_alarmTextList.size();
             if(alarmTestListSize>0)
             {
                 if(m_alarmPos<alarmTestListSize)
                 {
                     m_textToSpeech->say(m_alarmTextList.at(m_alarmPos));
                 }

                 foreach (QString alarmText, m_alarmTextList)
                 {
                     if(alarmText.startsWith("首火警")||alarmText.startsWith("火警"))
                     {
                         m_alarmPos=0;
                         return;
                     }
                 }
                 if(!m_alarmTextList.at(m_alarmPos).startsWith("首火警")||!m_alarmTextList.at(m_alarmPos).startsWith("火警"))
                 {
                     m_alarmPos++;
                     m_alarmPos = m_alarmPos%alarmTestListSize;
                 }
                 else
                 {
                     m_alarmPos =0;
                 }
             }
             else {
                 stopSpeech();
             }
         }
     });

    startSpeech();
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
    m_textToSpeech->setLocale(m_languageHash[languageName].toLocale());
}

QString SpeechObj::currentLanguage()
{

    return m_languageHash.key(m_textToSpeech->locale());
}


void SpeechObj::stopSpeech()
{
#ifdef Q_OS_WIN
    m_textToSpeech->pause();
#elif
    m_textToSpeech->stop();
#endif
}

void SpeechObj::startSpeech()
{

    // qDebug() << "state:" << m_textToSpeech->state();
    if(m_textToSpeech->state()!=QTextToSpeech::Speaking)
    {
        if(m_alarmTextList.size()>0)
        {
            m_textToSpeech->say(m_alarmTextList.at(0));
        }
    }
}

void SpeechObj::insertAlarmText(const QString &alarmText)
{
    QFuture<void> future = QtConcurrent::run([=]()
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
    if(m_alarmTextList.contains(alarmText))
    {
        m_alarmTextList.removeOne(alarmText);
    }

}

void SpeechObj::removeAlarmText(int pos)
{
    if(m_alarmTextList.size()>pos)
    {
        m_alarmTextList.removeAt(pos);
    }
}
