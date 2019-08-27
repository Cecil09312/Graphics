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
/**
 * @brief The SpeechObj class
 * windows平台使用系统默认
 * linux系统使用ekho,使用版本ekho-7.7.1或者ekho-7.6
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
    double rate();
    double volume();
    double pitch();
    void setRate(double rate);
    void setVolume(double volume);
    void setPitch(double pitch);
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
signals:
    void speechStart();
    void textToSpeechStop();
    void insertText(const QString &alarmText);
    void clearText();
    void removeText(const QString &alarmText);
    void removeText(int pos);

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
    QList<QString>m_alarmTextList;
    int m_alarmPos;
    bool m_isStoped;
    int m_currentAlarmPos;
#ifdef Q_OS_WIN
    QTextToSpeech *m_textToSpeech;
#endif

#ifdef  Q_OS_LINUX
    QProcess *m_textToSpeechProcess;
    QTimer *m_startTimer;
#endif

    QThread *m_thread;
    QStringList m_engineNameList;
    QHash<QString,QVariant>m_languageHash;
    double m_rate;
    double m_pitch;
    double m_volume;
    QString m_currentLanguage;
    bool m_speechIsStop;
    int indexOfType(const QString &type);
};

#endif // SPEECHOBJ_H
