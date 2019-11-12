#include "ftpmanager.h"
#include <QFile>
#include "control/controller.h"
FtpManager::FtpManager(QObject *parent)
    : QObject(parent),
      m_sendDataSuccess(false)
{
    m_ftpConfiguration = Configuration(new FtpConfiguration);

    m_fileNameHash["消防控制室管理机构.pdf"]="Fire control room management organization.pdf";
    m_fileNameHash["系统竣工图纸.svg"]="System as built drawings.svg";
    m_fileNameHash["各分系统控制逻辑关系说明.pdf"]="Subsystem instructions.pdf";
    m_fileNameHash["设备使用说明书.pdf"]="Operation instructions.pdf";
    m_fileNameHash["系统操作规程.pdf"]="System operation procedures.pdf";
    m_fileNameHash["应急预案.pdf"]="Contingency plan.pdf";
    m_fileNameHash["值班制度.pdf"]="Duty rules.pdf";
    m_fileNameHash["维护保养制度.pdf"]="Maintenance rules.pdf";
    m_fileNameHash["维护保养记录.pdf"]="Maintenance records.pdf";

    m_thread = new QThread;
    m_manager = new QNetworkAccessManager();
    m_timeoutTimer = new CustomTimer;
    m_timeoutTimer->setSingleShot(true);
    m_manager->moveToThread(m_thread);
    this->moveToThread(m_thread);
    m_thread->start();
    connect(m_manager,&QNetworkAccessManager::finished,this,[=](QNetworkReply *reply)
    {
        if(reply!=nullptr)
        {
            //emit sendFileSuccess(true);
            if(m_replyList.contains(reply))
            {
                m_replyList.removeOne(reply);
            }
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
        QNetworkReply* reply = m_manager->put(QNetworkRequest(url),array);
        if(reply==nullptr)
        {
            emit sendFileSuccess(false);
            return;
        }
        m_replyList.push_back(reply);
        Q_ASSERT(reply);
        connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),this,[=](QNetworkReply::NetworkError error)
        {
            Q_UNUSED(error)
            emit ftpError(reply->errorString());
        });

        connect(reply,&QNetworkReply::uploadProgress,this,[=](qint64 bytesSent, qint64 bytesTotal)
        {
            m_sendDataSuccess = true;
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

    connect(m_timeoutTimer,&CustomTimer::timeout,this,[=]()
    {
        if(!m_sendDataSuccess)
        {
            emit sendFileSuccess(false);
            foreach (QNetworkReply *reply, m_replyList)
            {
                if(reply!=nullptr)
                {
                    reply->close();
                    reply->deleteLater();
                }
            }
            m_replyList.clear();
        }
    });

}

FtpManager::~FtpManager()
{
    foreach (QNetworkReply *reply, m_replyList) {
        if(reply!=nullptr)
        {
            reply->close();
            reply->deleteLater();
        }
    }
    m_timeoutTimer->stop();
    m_timeoutTimer->deleteLater();
    m_replyList.clear();
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
        QString curFileName=QUrl(pathStr).fileName();
//        QFile curFile;
//        if(!curFileName.isEmpty())
//        {
//            curFile.setFileName(curFileName);
//            curFile.rename(m_fileNameHash.value(curFileName));
//        }
       // qDebug() << "curFile.fileName()" << curFile.fileName();
        url.setPath(m_fileNameHash.value(curFileName));
        if(!userStr.isEmpty())
        {
            url.setUserName(userStr);
        }
        if(!password.isEmpty())
        {
            url.setPassword(password);
        }

        url.setPort(port);
        QFile file(pathStr);
        if(file.open(QIODevice::ReadWrite))
        {
            dataArray = file.readAll();
            file.close();
        }
    });
    future.waitForFinished();
    m_sendDataSuccess = false;
    if(!m_timeoutTimer->isActive())
    {
        m_timeoutTimer->start(1000);
    }
    emit sendData(url,dataArray);

}
