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
//    Q_PROPERTY(QString sendAddress READ sendAddress WRITE setSendAddress)
//    Q_PROPERTY(QString readAddress READ readAddress WRITE setReadAddress)
//    Q_PROPERTY(quint16 sendPort READ sendPort WRITE setSendPort)
//    Q_PROPERTY(quint16 readPort READ readPort WRITE setReadPort)
public:
    explicit UdpLink(QObject *parent = nullptr);
    ~UdpLink();
//    Q_INVOKABLE void bindToHost(const QString &address, quint16 port);
//    QString sendAddress();
//    QString readAddress();
//    quint16 sendPort();
//    quint16 readPort();

//    void setSendAddress(const QString &address);
//    void setReadAddress(const QString &address);
//    void setSendPort(quint16 port);
//    void setReadPort(quint16 port);


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
