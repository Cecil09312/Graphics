#include "ftpmanager.h"
#include <QFile>
#include "control/controller.h"
FtpManager::FtpManager(QObject *parent) : QObject(parent)
{
    m_ftpConfiguration = Configuration(new FtpConfiguration);
    m_thread = new QThread;
    m_manager = new QNetworkAccessManager();
    m_manager->moveToThread(m_thread);
    this->moveToThread(m_thread);
    m_thread->start();
    connect(m_manager,&QNetworkAccessManager::finished,this,[=](QNetworkReply *reply)
    {
        if(reply!=nullptr)
        {
            //emit sendFileSuccess(true);
            reply->close();
            reply->deleteLater();
        }
        else
        {
            emit sendFileSuccess(false);
        }
    });

    connect(this,&FtpManager::sendData,this,[=](const QUrl&url,const QByteArray&array)
    {
        m_reply = m_manager->put(QNetworkRequest(url),array);
        Q_ASSERT(m_reply);
        connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),this,[=](QNetworkReply::NetworkError error)
        {
            Q_UNUSED(error)
            emit ftpError(m_reply->errorString());
        });

        connect(m_reply,&QNetworkReply::uploadProgress,this,[=](qint64 bytesSent, qint64 bytesTotal)
        {
            if(bytesTotal<=0)
            {
                emit sendFileSuccess(false);
            }
            else
            {
                emit uploadProgress(bytesSent, bytesTotal);
                if(bytesSent>=bytesTotal)
                {
                    emit sendFileSuccess(true);
                }
            }

        });

    });

}

FtpManager::~FtpManager()
{
    m_thread->quit();
    m_thread->deleteLater();
    m_manager->deleteLater();
}

void FtpManager::uploadFile(const QString &fileName )
{
    QUrl url;
    QByteArray dataArray;
    QFuture<void> future = QtConcurrent::run([&]()
    {
        QHash<QString,QVariant> configurationHash= m_ftpConfiguration.data()->getConfiguration().toHash();
        QString hostStr = configurationHash["host"].toString();
        QString pathStr = Controller::instance()->fileNameFromQml(fileName);
        QString userStr = configurationHash["user"].toString();
        QString password = configurationHash["password"].toString();
        int port = configurationHash["port"].toInt();
        url.setScheme("ftp");
        url.setHost(hostStr);
        url.setPath(QUrl(pathStr).fileName());
        url.setUserName(userStr);
        url.setPassword(password);
        url.setPort(port);
        QFile file(pathStr);
        if(file.open(QIODevice::ReadWrite))
        {
            dataArray = file.readAll();
            file.close();
        }
    });
    future.waitForFinished();
    emit sendData(url,dataArray);

}
