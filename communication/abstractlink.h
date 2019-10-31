#ifndef ABSTRACTLINK_H
#define ABSTRACTLINK_H

#include <QObject>
#include <QByteArray>
#include <QSharedPointer>
#include "abstractconfiguration.h"
#include <QRunnable>
#include <QThreadPool>
#include <QIODevice>

class AbstractLink : public QObject
{
    Q_OBJECT
public:
    explicit AbstractLink(QObject *parent = nullptr);
    virtual ~AbstractLink();
    virtual QIODevice *device();

    virtual qint64 writeDataSize();
    virtual bool writeDataSuccess();
signals:
    void writeData(const QByteArray&array);
    void getData(const QByteArray&array);
    void errorInfo(const QString &error);
    void isConnected(bool success);
    void startConnect();
    void stopConnect();
    void sendAllData();
public slots:
    void sendData(const QByteArray&array);
    virtual void readData()=0;
    virtual void setConfiguration()=0;
    void connectLink();
    void disconnectLink();
    void sendAll();
private:

};
typedef QSharedPointer<AbstractLink> Link;
#endif // ABSTRACTLINK_H
