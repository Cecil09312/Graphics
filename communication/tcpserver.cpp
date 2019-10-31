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
                QThread::sleep(1);

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
    }

    foreach (QTcpSocket*curSocket, m_socketList)
    {
        if(curSocket!=nullptr)
        {
            if(curSocket->state()==QTcpSocket::ConnectedState)
            {
                foreach (const QByteArray &curArray, m_sendArrayList)
                {
                    int sendDataSize=curSocket->write(curArray);
                    curSocket->flush();
                    if(sendDataSize>=curArray.size())
                    {
                        m_sendSuccess = true;
                        m_sendArrayList.removeOne(curArray);

                    }
                    else
                    {
                        m_sendSuccess = false;
                    }
                    QThread::msleep(10);

                }
            }
            else
            {
                m_sendSuccess= false;
            }
            // qDebug() << "write";
        }
        else
        {
            m_sendSuccess= false;
        }
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
                    int sendSize=curSocket->write(array);
                    curSocket->flush();
                    if(sendSize>=array.size())
                    {
                        m_sendArrayList.removeOne(array); 
                    }
                    QThread::msleep(10);
                }
            }
            else
            {
                m_sendSuccess= false;
            }
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

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket*tcpSocket = new QTcpSocket();
    tcpSocket->setSocketDescriptor(socketDescriptor);
    m_socketList.push_back(tcpSocket);
    sendAllData();
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
