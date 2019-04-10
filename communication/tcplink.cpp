#include "TcpLink.h"
#include <QDebug>
#include <QHostAddress>
TcpLink::TcpLink(QObject *parent)
    : AbstractLink(parent)
{
    m_tcpSocket = new QTcpSocket;
    m_thread = new QThread();
    m_tcpConfiguration = Configuration(new TcpConfiguration);
    m_tcpSocket->moveToThread(m_thread);
    moveToThread(m_thread);
    m_thread->start();
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qRegisterMetaType<QHostAddress>("const QHostAddress&");
    qRegisterMetaType<QByteArray>("const QByteArray&");
    qRegisterMetaType<QByteArray>("QByteArray");
    connect(this,&TcpLink::startConnect,this,[=]()
    {

        if(m_tcpSocket->state()==QTcpSocket::UnconnectedState)
        {
            setConfiguration();
            m_tcpSocket->connectToHost(m_address,m_port);
            m_tcpSocket->waitForConnected(1000);
            if(m_tcpSocket->state()!=QTcpSocket::ConnectedState)
            {
                emit isConnected(false);
            }
        }
        //qDebug() << m_tcpSocket->state();
    });
    connect(this,&TcpLink::stopConnect,this,[=](){
        m_tcpSocket->close();
    });

    connect(this,&TcpLink::writeData,this,[=](const QByteArray &array)
    {
        if(m_tcpSocket->state()==QAbstractSocket::ConnectedState)
        {
            m_tcpSocket->write(array);
        }

    });
    connect(m_tcpSocket,&QTcpSocket::readyRead,this,&TcpLink::readData);
    connect(m_tcpSocket,&QTcpSocket::connected,this,[=]()
    {
        emit isConnected(true);
    });
    connect(m_tcpSocket,&QTcpSocket::disconnected,this,[=]()
    {
        emit isConnected(false);
    });

}

TcpLink::~TcpLink()
{
    disconnectLink();
    m_tcpSocket->deleteLater();
    m_thread->quit();
    m_thread->deleteLater();
}

//void TcpLink::openTcp(const QString &address, quint16 port)
//{
//   // emit connectTcp(address ,port);
//}

//void TcpLink::close()
//{
//  //  emit closeTcp();
//}

//bool TcpLink::isConnected()
//{
//    if(m_tcpSocket->state()==QAbstractSocket::ConnectedState)
//    {
//        return true;
//    }
//    else
//    {
//      return false;
//    }

//}


void TcpLink::readData()
{
    QMutexLocker locker(&m_mutex);
    QByteArray dataArray=  m_tcpSocket->readAll();
    emit getData(dataArray);
}

void TcpLink::setConfiguration()
{  
    QHash<QString,QVariant>valueHash = m_tcpConfiguration.data()->getConfiguration().toHash();
    m_address = valueHash["hostAddr"].toString();
    m_port = quint16(valueHash["port"].toUInt());
}




