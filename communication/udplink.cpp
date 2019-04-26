#include "udplink.h"
#include <QDebug>
#include "udpconfiguration.h"
UdpLink::UdpLink(QObject *parent) :
    AbstractLink(parent)
{
    m_udpSocket = new QUdpSocket;
    m_thread = new QThread;
    m_udpConfiguration = Configuration(new UdpConfiguration);
    m_udpSocket->moveToThread(m_thread);
    setConfiguration();
    this->moveToThread(m_thread);
    qRegisterMetaType<QByteArray>("QByteArray");
    qRegisterMetaType<QByteArray>("const QByteArray&");
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qRegisterMetaType<QHostAddress>("const QHostAddress&");
    m_thread->start();

    connect(this,&UdpLink::writeData,this,[=](const QByteArray &array)
    {
        QHostAddress sendAddress = QHostAddress(m_sendAddress);
        m_udpSocket->writeDatagram(array,sendAddress,m_sendPort);
    });
    connect(this,&UdpLink::startConnect,this,[=]()
    {
        QHostAddress sendHostAddr = QHostAddress(m_sendAddress);
        m_udpSocket->bind(sendHostAddr,m_sendPort);
        // qDebug() << m_udpSocket->state();

    });

    connect(this,&UdpLink::stopConnect,this,[=](){
        m_udpSocket->close();
    });

    connect(m_udpSocket,&QUdpSocket::readyRead,this,&UdpLink::readData);
}

UdpLink::~UdpLink()
{
    disconnectLink();
    m_udpSocket->close();
    m_udpSocket->deleteLater();
    m_thread->quit();
    //m_thread->wait();
    m_thread->deleteLater();
}

void UdpLink::readData()
{
    QByteArray dataArray;
    qint64 dataSize= m_udpSocket->bytesAvailable();
    QHostAddress address = QHostAddress(m_readAddress);
    m_udpSocket->readDatagram(dataArray.data(),dataSize,&address,&m_readPort);
    emit getData(dataArray);
}

void UdpLink::setConfiguration()
{
    QVariant data=  m_udpConfiguration.data()->getConfiguration();
    QHash<QString,QVariant> valueHash = data.toHash();
    m_readAddress= valueHash["readAddr"].toString();
    m_readPort = valueHash["readPort"].toInt();
    m_sendAddress= valueHash["sendAddr"].toString();
    m_sendPort = valueHash["sendPort"].toInt();
   // qDebug() << m_readAddress << m_readPort << m_sendAddress << m_sendPort;
}


