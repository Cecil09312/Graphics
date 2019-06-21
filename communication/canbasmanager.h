#ifndef CANBASMANAGER_H
#define CANBASMANAGER_H

#include <QObject>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QThread>
#include <QSharedPointer>
#include "abstractconfiguration.h"

class CanbasManager : public QObject//CAN总线管理
{
    Q_OBJECT
public:
    enum ConnectCanState
    {
        ConnectedCan,
        DisConnectedCan
    };
    explicit CanbasManager( QObject *parent = nullptr);
    ~CanbasManager();
    void writeFrame(const QCanBusFrame &frame);
    void connectDevice(ConnectCanState state);
    void createDevice();
    void deleteDevice();
    Q_INVOKABLE  void setCanConfigure();
signals:
    void connectCan(ConnectCanState state);
    void readFrame(const QByteArray &frame);
    void writeData(const QCanBusFrame &frame);
    void errorHanppen(const QString &error);
    void currentState(QCanBusDevice::CanBusDeviceState state);

public slots:
private:
    QThread *m_thread;
    QSharedPointer<QCanBusDevice>m_canbasDevice;
    QString m_plugin;
    QString m_interfaceName;
    Configuration m_canConfiguration;
};

#endif // CANBASOBJ_H
