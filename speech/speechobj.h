#ifndef SPEECHOBJ_H
#define SPEECHOBJ_H
#include <QTextToSpeech>
#include <QString>
#include <QThread>
#include <QVoice>
#include <QVector>
class SpeechObj : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double pitch READ pitch WRITE setPitch)
    Q_PROPERTY(double rate READ rate WRITE setRate)
    Q_PROPERTY(double volume READ volume WRITE setVolume)
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
    bool m_isStoped;
    int m_currentAlarmPos;
    QTextToSpeech *m_textToSpeech;
    QThread *m_thread;
};

#endif // SPEECHOBJ_H
