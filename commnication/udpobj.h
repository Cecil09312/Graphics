#ifndef UDPOBJ_H
#define UDPOBJ_H

#include <QObject>
#include <QUdpSocket>
#include <QThread>
#include <QHostAddress>
class UdpObj : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sendAddress READ sendAddress WRITE setSendAddress)
    Q_PROPERTY(QString readAddress READ readAddress WRITE setReadAddress)
    Q_PROPERTY(quint16 sendPort READ sendPort WRITE setSendPort)
    Q_PROPERTY(quint16 readPort READ readPort WRITE setReadPort)
public:
    explicit UdpObj(QObject *parent = nullptr);
    ~UdpObj();
    Q_INVOKABLE void bindToHost(const QString &address, quint16 port);
    QString sendAddress();
    QString readAddress();
    quint16 sendPort();
    quint16 readPort();

    void setSendAddress(const QString &address);
    void setReadAddress(const QString &address);
    void setSendPort(quint16 port);
    void setReadPort(quint16 port);


signals:
     void bindHost(const QHostAddress &address, quint16 port);
     void send(const QByteArray &array,const QHostAddress &host, quint16 port);
     void getData(QByteArray array);
public slots:
     void sendData(const QByteArray &array,const QHostAddress &host, quint16 port);
private:
    QUdpSocket *m_udpSocket;
    QThread *m_thread;
    QString m_sendAddress;
    QString m_readAddress;
    quint16 m_sendPort;
    quint16 m_readPort;
};

#endif // UDPOBJ_H
