#ifndef TCPOBJ_H
#define TCPOBJ_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
class TcpObj : public QObject
{
    Q_OBJECT
public:
    explicit TcpObj(QObject *parent = nullptr);
    ~TcpObj();
    void openTcp(const QString &address, quint16 port);
    void close();

signals:
    void connectTcp(const QString&address, quint16 port);
    void closeTcp();
    void send(const QByteArray &arry);

public slots:
    void sendData(const QByteArray &array);
    void readData();
private:
    QTcpSocket *m_tcpSocket;
    QThread *m_thread;
    QMutex m_mutex;
};

#endif // TCPOBJ_H
