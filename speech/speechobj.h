#ifndef SPEECHOBJ_H
#define SPEECHOBJ_H
#include <QTextToSpeech>
#include <QString>
#include <QThread>
#include <QVoice>
#include <QVector>
class SpeechObj : public QTextToSpeech
{
    Q_OBJECT
public:
    SpeechObj(QObject*parent=nullptr);
    ~SpeechObj();
   // Q_INVOKABLE void setSelectVoice(int index);
   // Q_INVOKABLE QList<QString> voiceNameList();
    //Q_INVOKABLE int numOfVoice();
   // Q_INVOKABLE QString voiceName(int index);
    //Q_INVOKABLE QString currentVoiceName();
    int currentAlarmPos();
    QList<QString> &alarmTextList();
public slots:
    void stopSpeech();
    void startSpeech();
    void insertAlarmText(const QString &alarmText);
    void clearAlarmText();
    void removeAlarmText(const QString &alarmText);
    void removeAlarmText(int pos);
private:
    QList<QString>m_alarmTextList;
    int m_alarmPos;
    QThread *m_thread;
    bool m_isStoped;
    int m_currentAlarmPos;
   // QVector<QVoice>m_voiceVec;
//    QList<QString>m_voiceNameList;
   // QVoice m_voice;
};

#endif // SPEECHOBJ_H
