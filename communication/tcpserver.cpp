#include "tcpserver.h"
#include <QThread>
TcpServer::TcpServer(QObject *parent):
    QTcpServer(parent),
    m_sendSuccess(false)
{
    connect(this,&TcpServer::closeSocket,this,[=]()
    {
        foreach (QTcpSocket*curSocket, m_socketList)
        {
            if(curSocket!=nullptr)
            {
                m_sendSuccess= false;
                curSocket->disconnectFromHost();
                curSocket->waitForDisconnected();

            }
        }
    });
}

TcpServer::~TcpServer()
{
    removeSocket();
    m_socketList.clear();
    this->close();
}

void TcpServer::writeData(const QByteArray &array)
{
    m_sendArrayList.push_back(array);
    if(m_socketList.isEmpty())
    {
        m_sendSuccess= false;
        emit writeDataSuccess(m_sendSuccess);
        return;
    }

    foreach (QTcpSocket*curSocket, m_socketList)
    {
        if(curSocket!=nullptr)
        {

            foreach (const QByteArray &curArray, m_sendArrayList)
            {
                if(curSocket->state()==QTcpSocket::ConnectedState)
                {
                    curSocket->write(curArray);
                    curSocket->flush();
                    m_sendSuccess = true;
                }
                else
                {
                    m_sendSuccess = false;
                }
                m_sendArrayList.removeOne(curArray);
                QThread::msleep(5);
            }


            // qDebug() << "write";
        }
        else
        {
            m_sendSuccess= false;
        }
        QThread::msleep(5);
    }

    if(m_sendArrayList.isEmpty())
    {
        emit writeDataSuccess(m_sendSuccess);
        QThread::msleep(5);
    }

}

void TcpServer::closeTcpSocket()
{
    emit closeSocket();
}

void TcpServer::sendAllData()
{
    if(m_sendArrayList.size()<=0)
    {
        return;
    }


    foreach (const QByteArray &array, m_sendArrayList)
    {

        foreach (QTcpSocket*curSocket, m_socketList)
        {
            if(curSocket!=nullptr)
            {
                if(curSocket->state()==QTcpSocket::ConnectedState)
                {
                    qint64 sendSize=curSocket->write(array);
                    curSocket->flush();
                    if(sendSize>=array.size())
                    {
                        m_sendArrayList.removeOne(array);
                    }
                    QThread::msleep(5);
                }
            }
            else
            {
                m_sendSuccess= false;
            }
            QThread::msleep(5);
        }
    }

    if(m_sendArrayList.isEmpty())
    {
        m_sendSuccess = true;
        emit resendData();
    }
    else
    {
        m_sendSuccess = false;
    }
}

bool TcpServer::dataSendSuccess()
{
    return m_sendSuccess;
}

void TcpServer::removeSocket()
{
    foreach (QTcpSocket*curSocket, m_socketList)
    {
        curSocket->deleteLater();
        curSocket= nullptr;
    }
}

void TcpServer::cleanSendData()
{
    m_sendArrayList.clear();
}

void TcpServer::writeOneFrameData(const QByteArray &array)
{
     foreach (QTcpSocket*curSocket, m_socketList)
     {
         if(curSocket!=nullptr)
         {
             if(curSocket->state()==QTcpSocket::ConnectedState)
             {
                 curSocket->write(array);
                 m_sendSuccess = true;
             }
             else
             {
                 m_sendSuccess = false;
             }
         }
     }
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket*tcpSocket = new QTcpSocket();
    tcpSocket->setSocketDescriptor(socketDescriptor);
    m_socketList.push_back(tcpSocket);
    //sendAllData();
    connect(tcpSocket,&QTcpSocket::readyRead,this,[=](){
        QByteArray array = tcpSocket->readAll();
        emit readData(array);
    });

    connect(tcpSocket,&QTcpSocket::stateChanged,this,[=](QAbstractSocket::SocketState state){
        if(state==QAbstractSocket::UnconnectedState)
        {
            m_sendSuccess = false;
            tcpSocket->deleteLater();
        }
    });
    connect(tcpSocket,&QTcpSocket::disconnected,this,[=](){
        m_sendSuccess= false;
        foreach (QTcpSocket*curSocket, m_socketList)
        {
            if(curSocket==tcpSocket)
            {
                m_socketList.removeOne(curSocket);
                curSocket->deleteLater();
                break;
            }
        }
        //tcpSocket->deleteLater();
        //tcpSocket= nullptr;

    });

    connect(tcpSocket,QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),this,[=]
    {
        m_sendSuccess= false;
        foreach (QTcpSocket*curSocket, m_socketList)
        {
            if(curSocket==tcpSocket)
            {
                m_socketList.removeOne(curSocket);
                curSocket->deleteLater();
                break;
            }
        }
    });
    //    QThread *thread = new QThread(tcpSocket);
    //    tcpSocket->moveToThread(thread);
    //    thread->start();
}
