#include "tcpobj.h"
#include <QDebug>
#include <QHostAddress>
TcpObj::TcpObj(QObject *parent) : QObject(parent)
{
    m_tcpSocket = new QTcpSocket;
    m_thread = new QThread();
    m_tcpSocket->moveToThread(m_thread);
    moveToThread(m_thread);
    m_thread->start();
//    qRegisterMetaType<QHostAddress>("QHostAddress");
//    qRegisterMetaType<QHostAddress>("const QHostAddress&");
    qRegisterMetaType<QByteArray>("const QByteArray&");
    qRegisterMetaType<QByteArray>("QByteArray");
    connect(this,&TcpObj::connectTcp,this,[=](const QString &address, quint16 port)
    {
        m_tcpSocket->connectToHost(address,port);
        m_tcpSocket->waitForConnected(1000);
    });
    connect(this,&TcpObj::closeTcp,this,[=](){
        m_tcpSocket->close();
    });

    connect(this,&TcpObj::send,this,[=](const QByteArray &array)
    {
        if(m_tcpSocket->state()==QAbstractSocket::ConnectedState)
        {
            m_tcpSocket->write(array);
        }

    });
    connect(m_tcpSocket,&QTcpSocket::readyRead,this,&TcpObj::readData);


}

TcpObj::~TcpObj()
{
    close();
    m_tcpSocket->deleteLater();
    m_thread->quit();
    m_thread->deleteLater();
}

void TcpObj::openTcp(const QString &address, quint16 port)
{
    emit connectTcp(address ,port);
}

void TcpObj::close()
{
    emit closeTcp();
}

void TcpObj::sendData(const QByteArray &array)
{
    emit send(array);
}

void TcpObj::readData()
{
    QMutexLocker locker(&m_mutex);
    QByteArray dataArray=  m_tcpSocket->readAll();
 qDebug() << dataArray;
//    qDebug() << QThread::currentThread();
}


