#ifndef TCPLINK_H
#define TCPLINK_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include "abstractlink.h"
class TcpLink : public AbstractLink
{
    Q_OBJECT
public:
    explicit TcpLink(QObject *parent = nullptr);
    ~TcpLink();
//    Q_INVOKABLE  void openTcp(const QString &address, quint16 port);
//    Q_INVOKABLE  bool isConnected();
//    void close();

//signals:
//    void connectTcp(const QString&address, quint16 port);
//    void closeTcp();
//    void send(const QByteArray &arry);

public slots:
    void sendData(const QByteArray &array);
    void readData();
    void setConfiguration();
     void connectLink();
     void disconnectLink();
private:
    QTcpSocket *m_tcpSocket;
    QThread *m_thread;
    QMutex m_mutex;
    QString m_address;
    quint16 m_port;
};

#endif // TCPOBJ_H
