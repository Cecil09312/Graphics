#ifndef SPEECHOBJ_H
#define SPEECHOBJ_H
#include <QTextToSpeech>
#include <QString>
#include <QThread>
#include <QVoice>
#include <QVector>
#include <QProcess>
#include <QMutex>
#include "customTimer/customtimer.h"
#include "jsonEdit/qmlforjson.h"
#include <QSharedPointer>
#include "communication/abstractlink.h"
#include "communication/speechcom.h"
/**
 * @brief The SpeechObj class
 * windows平台使用系统默认
 * linux系统使用espeak
 */
class SpeechObj : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double pitch READ pitch WRITE setPitch)
    Q_PROPERTY(double rate READ rate WRITE setRate)
    Q_PROPERTY(double volume READ volume WRITE setVolume)

    Q_PROPERTY(QStringList engineNameList READ engineNameList WRITE setEngineNameList)
public:
    SpeechObj(QObject*parent=nullptr);
    ~SpeechObj();
    int currentAlarmPos();
    QList<QString> &alarmTextList();
    //QString &alarmText(int pos);
    double rate();
    double volume();
    double pitch();
    void setRate(double rate);
    void setVolume(double volume);
    void setPitch(double pitch);
   Q_INVOKABLE double pitchMin();
   Q_INVOKABLE double pitchMax();
   Q_INVOKABLE double pitchStep();
   Q_INVOKABLE double rateMin();
   Q_INVOKABLE double rateMax();
   Q_INVOKABLE double rateStep();

   Q_INVOKABLE double volumeMin();
   Q_INVOKABLE double volumeMax();
   Q_INVOKABLE double volumeStep();


    Q_INVOKABLE bool alarmTextExist(const QString &alarmText);
    Q_INVOKABLE void engineSelected(const QString &engineName);
    QStringList engineNameList();
    void setEngineNameList(const QStringList&engine);
    QList<QVariant>languageValueList();
    QList<QString>languageNameList();
    Q_INVOKABLE QString engineName(int index);
    Q_INVOKABLE int engineNameNum();
    Q_INVOKABLE QString languageName(int index);
    Q_INVOKABLE int languageNum();
    Q_INVOKABLE void setLanguage(const QString &languageName);
    Q_INVOKABLE QString currentLanguage();
    Q_INVOKABLE void saveSpeechInfoToJson();
    void setIsEnglish(bool is);
    Q_INVOKABLE bool isEnglish();
    AbstractLink *speechCom();
    void speechSetting();
signals:
    void speechStart();
    void textToSpeechStop();
    void insertText(const QString &alarmText);
    void clearText();
    void removeText(const QString &alarmText);
    void removeText(int pos);
    void languageChangeToEnglish(bool isEnglish);

public slots:
    void stopSpeech();
    void startSpeech();
    void insertAlarmText(const QString &alarmText);
    void clearAlarmText();
    void removeAlarmText(const QString &alarmText);
    void removeAlarmText(int pos);
private slots:
    void repeatSpeak();
    void runSpeech();
    void speechStop();
private:
    void processText(const QString &alarmText);
private:
    QList<QString>m_alarmTextList;
    int m_alarmPos;
    bool m_isStoped;
    int m_currentAlarmPos;
    bool m_isEnglish;
#ifdef Q_OS_WIN
    QTextToSpeech *m_textToSpeech;
#endif

#ifdef  Q_OS_LINUX
    QProcess *m_textToSpeechProcess;

#endif

    QThread *m_thread;
    QStringList m_engineNameList;
    QHash<QString,QVariant>m_languageHash;
    QSharedPointer<AbstractLink>m_speechCom;
    double m_rate;
    double m_pitch;
    double m_volume;
    bool m_isStop;
    QString m_currentLanguage;
    bool m_speechIsStop;
    int indexOfType(const QString &type);
    QmlForJson m_speechJson;
    QByteArray m_readArray;
    QList<QString>m_initSettingList;
    bool m_isFinished{false};
    bool m_isFinishedSetting{false};
    const QString c_speechJsonDir = QCoreApplication::applicationDirPath()+"/speech.json";

};

#endif // SPEECHOBJ_H
