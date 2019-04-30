#ifndef UDPOBJ_H
#define UDPOBJ_H

#include <QObject>
#include <QUdpSocket>
#include <QThread>
#include <QHostAddress>
#include "abstractconfiguration.h"
#include "abstractlink.h"
class UdpLink : public AbstractLink
{
    Q_OBJECT
public:
    explicit UdpLink(QObject *parent = nullptr);
    ~UdpLink();

signals:

public slots:
     void readData();
     void setConfiguration();

private:
    QUdpSocket *m_udpSocket;
    QThread *m_thread;
    QString m_sendAddress;
    QString m_readAddress;
    quint16 m_sendPort;
    quint16 m_readPort;
    Configuration m_udpConfiguration;
};

#endif // UDPOBJ_H
