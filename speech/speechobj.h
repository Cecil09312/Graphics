#ifndef SPEECHOBJ_H
#define SPEECHOBJ_H
#include <QTextToSpeech>
#include <QString>
#include <QThread>
class SpeechObj : public QTextToSpeech
{
    Q_OBJECT
public:
    SpeechObj(QObject*parent=nullptr);
    ~SpeechObj();
public slots:
    void stopSpeech();
    void startSpeech();
    void insertAlarmText(const QString &alarmText);
    void clearAlarmText();
    void removeAlarmText(const QString &alarmText);
private:
    QList<QString>m_alarmTextList;
    int m_alarmPos;
    QThread *m_thread;
};

#endif // SPEECHOBJ_H
