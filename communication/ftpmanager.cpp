#include "ftpmanager.h"
#include <QFile>
#include "control/controller.h"
FtpManager::FtpManager(QObject *parent) : QObject(parent)
{
    m_ftpConfiguration = Configuration(new FtpConfiguration);
}

FtpManager::~FtpManager()
{

}

void FtpManager::uploadFile(const QString &fileName )
{

    QFuture<void> future = QtConcurrent::run([=]()
    {
        QNetworkAccessManager manager;
        QHash<QString,QVariant> configurationHash= m_ftpConfiguration.data()->getConfiguration().toHash();
        QString hostStr = configurationHash["host"].toString();
        QString pathStr = Controller::instance()->fileNameFromQml(fileName);
        QString userStr = configurationHash["user"].toString();
        QString password = configurationHash["password"].toString();
        int port = configurationHash["port"].toInt();
        QUrl url;
        url.setScheme("ftp");
        url.setHost(hostStr);
        url.setPath(pathStr);
        url.setUserName(userStr);
        url.setPassword(password);
        url.setPort(port);
        QFile file(pathStr);
        QByteArray dataArray;
        if(file.open(QIODevice::ReadWrite))
        {
            dataArray = file.readAll();
            file.close();
        }
        manager.put(QNetworkRequest(url),dataArray);
        connect(&manager,&QNetworkAccessManager::finished,this,[=](QNetworkReply *reply)
        {
            if(reply!=nullptr)
            {
                reply->close();
                reply->deleteLater();
            }
        });
    });
    future.waitForFinished();

}
