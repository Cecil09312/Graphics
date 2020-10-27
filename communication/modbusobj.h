#ifndef MODBUSOBJ_H
#define MODBUSOBJ_H

#include <QObject>
#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusRtuSerialSlave>
#include <QModbusTcpServer>
#include <QThread>
#include <QCoreApplication>
class ModbusObj : public QObject
{
    Q_OBJECT
    Q_ENUMS(ModbusType)

public:
    enum class ModbusType
    {
        SerialRtu=1,
        Tcp
    };
    explicit ModbusObj(QObject *parent = nullptr);
    ~ModbusObj();

    Q_INVOKABLE void closeRtuSerialModbus();
    Q_INVOKABLE void closeTcpModbus();
    Q_INVOKABLE void connectRtuSerialModbus();
    Q_INVOKABLE void connectTcpModbus();
    Q_INVOKABLE void saveToJson();
    Q_INVOKABLE void setIsBase0(bool isBase0);
    Q_INVOKABLE void settingTcpModbus(const QString&ipAddr,int port);
    Q_INVOKABLE void settingSerialModbus(const QString&portName,int baudRate,int dataBit,int stopBit,const QString&parity);
    Q_INVOKABLE QList<QVariant>portNameModel();
    Q_INVOKABLE QList<QVariant>baudRateModel();
    Q_INVOKABLE QString ipAddr();
    Q_INVOKABLE QString ipPort();
    Q_INVOKABLE QString portName();
    Q_INVOKABLE QVariant baudRate();
    Q_INVOKABLE QVariant dataBits();
    Q_INVOKABLE QVariant stopBits();
    Q_INVOKABLE QVariant parity();
    Q_INVOKABLE ModbusType &modbusType();
    Q_INVOKABLE void setModbusType(ModbusType curModbusType);






signals:
    void closeModbusTcp();
    void closeModbusSerialRtu();
    void startConnectModbusTcp();
    void startConnectModbusSerialRtu();
    void serialModbusState(bool isOk);
    void tcpModbusState(bool isOk);

public slots:
private:
    QModbusRtuSerialSlave*m_serialRtuServer;
    QModbusTcpServer*m_tcpServer;
    ModbusType m_modbusType;
    QThread *m_thread;
    QHash<QString,QVariant>m_jsonHash;
    //QHash<QString,QVariant>m_rtuSerialHash;
    //QHash<QString,QVariant>m_tcpValueHash;
    const QString m_fileDir= QCoreApplication::applicationDirPath()+"/modbus.json";
    bool m_isBase0{false};
    //QList<QVariant>m_portNameList;

};

#endif // MODBUSOBJ_H
