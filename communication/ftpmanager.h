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

public slots:
    void uploadFile(const QString &fileName);
private:
    Configuration m_ftpConfiguration;

};

#endif // FTPMANAGER_H
