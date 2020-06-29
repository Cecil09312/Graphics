#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>
#include <QTcpSocket>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject *parent = Q_NULLPTR);
    ~TcpServer();
    void writeData(const QByteArray &array);
    void closeTcpSocket();
    void sendAllData();
    bool dataSendSuccess();
    void removeSocket();
    void cleanSendData();
    void writeOneFrameData(const QByteArray &array);
signals:
    void readData(const QByteArray &array);
    void closeSocket();
    void resendData();
    void writeDataSuccess(bool isSuccess);

protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    QList<QTcpSocket*>m_socketList;
    QList<QByteArray> m_sendArrayList;
    bool m_sendSuccess;
};

#endif // TCPSERVER_H
