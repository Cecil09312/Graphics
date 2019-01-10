#include "udpobj.h"
#include <QDebug>

UdpObj::UdpObj(QObject *parent) : QObject(parent)
{
    m_udpSocket = new QUdpSocket;
    m_thread = new QThread;
    m_udpSocket->moveToThread(m_thread);
    this->moveToThread(m_thread);
    qRegisterMetaType<QByteArray>("QByteArray");
    qRegisterMetaType<QByteArray>("const QByteArray&");
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qRegisterMetaType<QHostAddress>("const QHostAddress&");
    m_thread->start();
//    qRegisterMetaType<QHostAddress,quint16>("const QHostAddress&,quint16");
//    qRegisterMetaType<QHostAddress,quint16>("QHostAddress,quint16");
    connect(this,&UdpObj::send,this,[=](const QByteArray &array,const QHostAddress &host, quint16 port)
    {
        m_udpSocket->writeDatagram(array,host,port);
    });
    connect(this,&UdpObj::bindHost,this,[=](const QHostAddress &address, quint16 port)
    {
        m_udpSocket->bind(address,port);

    });

    connect(m_udpSocket,&QUdpSocket::readyRead,this,[=](){
        QByteArray dataArray;
        qint64 dataSize= m_udpSocket->bytesAvailable();
        QHostAddress address = QHostAddress(m_readAddress);
        m_udpSocket->readDatagram(dataArray.data(),dataSize,&address,&m_readPort);
        emit getData(dataArray);
    });
}

UdpObj::~UdpObj()
{
    m_udpSocket->close();
    m_udpSocket->deleteLater();
    m_thread->quit();
    m_thread->deleteLater();
}

void UdpObj::bindToHost(const QString &address, quint16 port)
{
    QHostAddress hostAddress = QHostAddress(address);
    emit bindHost(hostAddress,port);
}

QString UdpObj::sendAddress()
{
    return m_sendAddress;
}

QString UdpObj::readAddress()
{
    return m_readAddress;
}

quint16 UdpObj::sendPort()
{
    return m_sendPort;
}

quint16 UdpObj::readPort()
{
    return m_readPort;
}

void UdpObj::setSendAddress(const QString &address)
{
    m_sendAddress = address;
}

void UdpObj::setReadAddress(const QString &address)
{
    m_readAddress = address;
}

void UdpObj::setSendPort(quint16 port)
{
    m_sendPort = port;
}

void UdpObj::setReadPort(quint16 port)
{
    m_readPort = port;
}

void UdpObj::sendData(const QByteArray &array,const QHostAddress &host, quint16 port)
{
    emit send(array,host,port);
}
