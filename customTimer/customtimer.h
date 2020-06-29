#ifndef CUSTOMTIMER_H
#define CUSTOMTIMER_H

#include <QObject>
#include <QTimer>
#include <QThread>


class CustomTimer : public QObject
{
    Q_OBJECT
public:
    explicit CustomTimer(QObject *parent = nullptr);
    ~CustomTimer();
    void start(int ms);
    void stop();
    bool isActive();
    void setSingleShot(bool singleShot);

signals:
    void startTimerMs(int ms);
    void stopTimer();
    void timeout();
public slots:
private:
    QTimer *m_timer;
    QThread *m_thread;
};

#endif // CUSTOMTIMER_H
