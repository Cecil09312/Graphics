#ifndef ABSTRACTLINK_H
#define ABSTRACTLINK_H

#include <QObject>
#include <QByteArray>
#include <QSharedPointer>
#include "abstractconfiguration.h"
#include <QRunnable>
#include <QThreadPool>


class AbstractLink : public QObject
{
    Q_OBJECT
public:
    explicit AbstractLink(QObject *parent = nullptr);
    virtual ~AbstractLink();

signals:
    void writeData(const QByteArray&array);
    void getData(const QByteArray&array);
    void errorInfo(const QString &error);
    void isConnected(bool success);
    void startConnect();
    void stopConnect();
public slots:
    void sendData(const QByteArray&array);
    virtual void readData()=0;
    virtual void setConfiguration()=0;
    void connectLink();
    void disconnectLink();
private:

};
typedef QSharedPointer<AbstractLink> Link;
#endif // ABSTRACTLINK_H
