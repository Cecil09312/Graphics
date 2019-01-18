#ifndef MODBUSMANAGER_H
#define MODBUSMANAGER_H

#include <QObject>
#include "abstractconfiguration.h"
#include <QModbusClient>
#include <QSharedPointer>
#include <QModbusReply>
#include <QThread>
#include <QEventLoop>
#include <QModbusDataUnit>
class ModbusManager:public QObject
{
    Q_OBJECT
public:
    enum ConnectState
    {
       DisConnected,
       Connected
    };
public:
    ModbusManager(Configuration configuration, QObject*parent=nullptr);
    ~ModbusManager();
    void connectDevice(ConnectState state);
    void sendWriteRequest(const QModbusDataUnit &write, int serverAddress);
    void sendReadRequest(const QModbusDataUnit &read, int serverAddress);

signals:
    void connectModbus(ConnectState state);
    void writeRequest(const QModbusDataUnit &write, int serverAddress);
    void readRequest(const QModbusDataUnit &read, int serverAddress);
    void readRequestData(const QModbusDataUnit &read);
    void writeRequestData(const QModbusDataUnit &write);
    void errorHappen(const QString &error);
    void currentState(QModbusDevice::State state);
private:
    QSharedPointer<QModbusClient>m_modbusClient;
   // QSharedPointer<QModbusReply>m_modbusReply;
    Configuration m_configuration;
    QThread *m_thread;
};

#endif // ABSTRACTMODBUS_H
