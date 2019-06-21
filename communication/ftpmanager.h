#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "communication/abstractconfiguration.h"
#include "communication/ftpconfiguration.h"
#include <QtConcurrent>
#include <QFuture>

class FtpManager : public QObject
{
    Q_OBJECT
public:
    explicit FtpManager(QObject *parent = nullptr);
    ~FtpManager();


signals:
    void sendFileSuccess(bool isOk);
    void sendData(const QUrl &url,const QByteArray &array);
    void ftpError(const QString&error);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
public slots:
    void uploadFile(const QString &fileName);
private:
    Configuration m_ftpConfiguration;
    QNetworkReply*m_reply;
    QNetworkAccessManager *m_manager;
    QThread *m_thread;

};

#endif // FTPMANAGER_H
