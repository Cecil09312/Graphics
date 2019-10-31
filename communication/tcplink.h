#ifndef TCPLINK_H
#define TCPLINK_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include "abstractlink.h"
#include "abstractconfiguration.h"
#include "tcpconfiguration.h"
#include "tcpserver.h"
class TcpLink : public AbstractLink
{
    Q_OBJECT
public:
    explicit TcpLink(QObject *parent = nullptr);
    ~TcpLink();
    virtual QIODevice *device();
    qint64 writeDataSize();
    bool writeDataSuccess();

public slots:
    void readData();
    void setConfiguration();
private:
    TcpServer *m_tcpServer;
  //  QTcpSocket *m_tcpSocket;
    QThread *m_thread;
    QMutex m_mutex;
    QString m_address;
    quint16 m_port;
    Configuration m_tcpConfiguration;
    qint64 m_writeDataSize;
    QList<QByteArray> m_sendDataList;

};

#endif // TCPOBJ_H
