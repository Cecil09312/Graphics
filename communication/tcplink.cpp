#include "tcplink.h"

#include <QDebug>
#include <QHostAddress>
#include "control/controller.h"
#include <QMutexLocker>
TcpLink::TcpLink(QObject *parent)
    : AbstractLink(parent)

{
    m_tcpServer = new TcpServer;

    m_thread = new QThread();
    m_tcpConfiguration = Configuration(new TcpConfiguration);
    m_tcpServer->moveToThread(m_thread);
    moveToThread(m_thread);
    m_thread->start();

    qRegisterMetaType<QHostAddress>("QHostAddress");
    qRegisterMetaType<QHostAddress>("const QHostAddress&");
    qRegisterMetaType<QByteArray>("const QByteArray&");
    qRegisterMetaType<QByteArray>("QByteArray");
    connect(this,&TcpLink::startConnect,this,[=]()
    {
        setConfiguration();
        if(m_tcpServer->isListening())
        {
            return;
        }
       bool isListened= m_tcpServer->listen(QHostAddress::Any,m_port);
       if(!isListened)
       {
           emit isConnected(false);
       }

    });

    connect(m_tcpServer,&TcpServer::resendData,this,&TcpLink::sendAllData);
    connect(this,&TcpLink::stopConnect,this,[=](){

      m_tcpServer->close();
      emit isConnected(false);
      m_tcpServer->cleanSendData();

    });

    connect(m_tcpServer,&QTcpServer::acceptError,this,[=](QAbstractSocket::SocketError socketError)
    {
        Q_UNUSED(socketError);
        emit errorInfo(m_tcpServer->errorString());

    });

    connect(this,&TcpLink::writeData,this,[=](const QByteArray &array)
    {

        m_tcpServer->writeData(array);
    });

    connect(this,&TcpLink::oneFrameData,this,[=](const QByteArray &array){
        m_tcpServer->writeOneFrameData(array);
    });
    connect(m_tcpServer,&TcpServer::readData,this,&TcpLink::getData);
    connect(m_tcpServer,&TcpServer::newConnection,this,[=](){

        emit isConnected(true);
    });

    connect(m_tcpServer,&TcpServer::writeDataSuccess,this,&TcpLink::sendDataSuccess);

}

TcpLink::~TcpLink()
{
    disconnectLink();
    m_tcpServer->close();
    m_thread->quit();
    m_thread->deleteLater();
}

QIODevice *TcpLink::device()
{
    return nullptr;
    //return m_tcpSocket;
}

qint64 TcpLink::writeDataSize()
{
    return m_writeDataSize;
}

bool TcpLink::writeDataSuccess()
{
    return m_tcpServer->dataSendSuccess();
}


void TcpLink::readData()
{
//    if(m_tcpSocket==nullptr)
//    {
//        return ;
//    }
//    QMutexLocker locker(&m_mutex);
//    QByteArray dataArray=  m_tcpSocket->readAll();
//    emit getData(dataArray);
}

void TcpLink::setConfiguration()
{  
    QHash<QString,QVariant>valueHash = m_tcpConfiguration.data()->getConfiguration().toHash();
    m_address = valueHash["hostAddr"].toString();
    m_port = quint16(valueHash["port"].toUInt());
}




