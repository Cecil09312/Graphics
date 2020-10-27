#include "modbusobj.h"
#include <QSerialPort>
#include "jsonEdit/qmlforjson.h"
#include "control/controller.h"
#include <QSerialPortInfo>
ModbusObj::ModbusObj(QObject *parent)
    : QObject(parent),
      m_modbusType(ModbusType::SerialRtu)


{
    m_thread = new QThread;
    m_serialRtuServer = new QModbusRtuSerialSlave;
    m_tcpServer = new QModbusTcpServer;
    qRegisterMetaType<QModbusDataUnit::RegisterType>("QModbusDataUnit::RegisterType");
    this->moveToThread(m_thread);
    m_serialRtuServer->moveToThread(m_thread);
    m_tcpServer->moveToThread(m_thread);
    m_thread->start();

    QmlForJson qmlForJson;
    m_jsonHash=qmlForJson.readFile(m_fileDir).toHash();
    DataStore *dataStore= Controller::instance()->getDataStore();
    connect(m_serialRtuServer,&QModbusRtuSerialSlave::dataWritten,this,[&](QModbusDataUnit::RegisterType table, int address, int size)
    {

        int serverAddr= m_serialRtuServer->serverAddress();
        int extNum =0;
        if(serverAddr-1>0)
        {
            extNum=serverAddr-1;
        }
        for(int i=0;i<size;i++)
        {
            quint16 *dataValue;
            bool isOk= m_serialRtuServer->data(table,address+i,dataValue);
            if(isOk)
            {
                quint8 loopNum=0,addrNum=0;
                quint16 sendValue =0;
                if(dataStore!=nullptr)
                {
                    loopNum= (*dataValue)/256;

                    if(m_isBase0)
                    {
                        addrNum = (*dataValue)%256&0xff;
                    }
                    else
                    {
                        addrNum = (*dataValue)%256&0xff-1;
                    }
                    if(dataStore->haveTypeItem(tr("火警"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x8000;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("火警"))>=0)
                        {
                            sendValue |=0x8000;
                        }
                    }
                    if(dataStore->haveTypeItem(tr("监管"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x4000;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("监管"))>=0)
                        {
                            sendValue |=0x4000;
                        }
                    }
                    if(dataStore->haveTypeItem(tr("启动"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x2000;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("启动"))>=0)
                        {
                            sendValue |=0x2000;
                        }
                    }
                    if(dataStore->haveTypeItem(tr("反馈"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x1000;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("反馈"))>=0)
                        {
                            sendValue |=0x1000;
                        }
                    }
                    if(dataStore->haveTypeItem(tr("故障"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x0800;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("故障"))>=0)
                        {
                            sendValue |=0x0800;
                        }
                    }
                    if(dataStore->haveTypeItem(tr("屏蔽"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x0400;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("屏蔽"))>=0)
                        {
                            sendValue |=0x0400;
                        }
                    }

                }
                m_serialRtuServer->setData(table,address+i,sendValue);
            }
        }

    });

    connect(m_tcpServer,&QModbusTcpServer::dataWritten,this,[&](QModbusDataUnit::RegisterType table, int address, int size)
    {
        int serverAddr= m_serialRtuServer->serverAddress();
        int extNum =0;
        if(serverAddr-1>0)
        {
            extNum=serverAddr-1;
        }
        for(int i=0;i<size;i++)
        {
            quint16 *dataValue;
            bool isOk= m_serialRtuServer->data(table,address+i,dataValue);
            if(isOk)
            {
                quint8 loopNum=0,addrNum=0;
                quint16 sendValue =0;
                if(dataStore!=nullptr)
                {
                    loopNum= (*dataValue)/256;

                    if(m_isBase0)
                    {
                        addrNum = (*dataValue)%256&0xff;
                    }
                    else
                    {
                        addrNum = (*dataValue)%256&0xff-1;
                    }
                    if(dataStore->haveTypeItem(tr("火警"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x8000;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("火警"))>=0)
                        {
                            sendValue |=0x8000;
                        }
                    }
                    if(dataStore->haveTypeItem(tr("监管"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x4000;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("监管"))>=0)
                        {
                            sendValue |=0x4000;
                        }
                    }
                    if(dataStore->haveTypeItem(tr("启动"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x2000;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("启动"))>=0)
                        {
                            sendValue |=0x2000;
                        }
                    }
                    if(dataStore->haveTypeItem(tr("反馈"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x1000;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("反馈"))>=0)
                        {
                            sendValue |=0x1000;
                        }
                    }
                    if(dataStore->haveTypeItem(tr("故障"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x0800;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("故障"))>=0)
                        {
                            sendValue |=0x0800;
                        }
                    }
                    if(dataStore->haveTypeItem(tr("屏蔽"),QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0"))
                    {
                        sendValue |=0x0400;
                    }
                    else
                    {
                        if(dataStore->indexOfItem(QString::number(extNum),QString::number(loopNum),QString::number(addrNum),"0",tr("屏蔽"))>=0)
                        {
                            sendValue |=0x0400;
                        }
                    }

                }
                m_serialRtuServer->setData(table,address+i,sendValue);
            }
        }
    });

    connect(this,&ModbusObj::closeModbusSerialRtu,this,[=](){
        m_serialRtuServer->disconnectDevice();
    });
    connect(this,&ModbusObj::closeModbusTcp,this,[=](){
        m_tcpServer->disconnectDevice();
    });

    connect(this,&ModbusObj::startConnectModbusSerialRtu,this,[=](){

        QHash<QString,QVariant> rtuSerialHah= m_jsonHash.value("serialModbus").toHash();
        if(!rtuSerialHah.isEmpty())
        {
             m_serialRtuServer->disconnectDevice();
             rtuSerialHah.value("portName");
             rtuSerialHah.value("buadRate").toInt();
             rtuSerialHah.value("stopBit").toInt();
             rtuSerialHah.value("dataBit").toInt();
             QString parity=rtuSerialHah.value("parity").toString();
             int parityValue=0;
             if(parity.contains("NoParity"))
             {
                 parityValue=QSerialPort::NoParity;
             }
             m_serialRtuServer->setConnectionParameter(QModbusDevice::SerialPortNameParameter,rtuSerialHah.value("portName"));
             m_serialRtuServer->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,rtuSerialHah.value("buadRate"));

             m_serialRtuServer->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,rtuSerialHah.value("stopBit"));
             m_serialRtuServer->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,rtuSerialHah.value("dataBit"));
             m_serialRtuServer->setConnectionParameter(QModbusDevice::SerialParityParameter,parityValue);

             m_serialRtuServer->connectDevice();

        }
    });

    connect(this,&ModbusObj::startConnectModbusTcp,this,[=]()
    {

        QHash<QString,QVariant>tcpValueHash= m_jsonHash.value("tcpModbus").toHash();
        if(!tcpValueHash.isEmpty())
        {
            m_tcpServer->disconnectDevice();
            m_tcpServer->setConnectionParameter(QModbusDevice::NetworkAddressParameter,tcpValueHash.value("ipAddr"));
            m_tcpServer->setConnectionParameter(QModbusDevice::NetworkPortParameter,tcpValueHash.value("port"));
            m_tcpServer->connectDevice();
        }
    });


    connect(m_serialRtuServer,&QModbusRtuSerialSlave::stateChanged,this,[=](QModbusDevice::State state)
    {
        if(state==QModbusDevice::ConnectedState)
        {
            emit serialModbusState(true);
        }
        else
        {
            emit serialModbusState(false);
        }
    });


    connect(m_tcpServer,&QModbusTcpServer::stateChanged,this,[=](QModbusDevice::State state)
    {
        if(state==QModbusDevice::ConnectedState)
        {
            emit tcpModbusState(true);
        }
        else
        {
            emit tcpModbusState(false);
        }
    });
}

ModbusObj::~ModbusObj()
{
  saveToJson();
  m_serialRtuServer->disconnectDevice();
  m_serialRtuServer->deleteLater();
  m_tcpServer->disconnectDevice();
  m_tcpServer->deleteLater();
  m_thread->quit();
  m_thread->deleteLater();
}



void ModbusObj::closeRtuSerialModbus()
{

    emit closeModbusSerialRtu();
}

void ModbusObj::closeTcpModbus()
{
    //m_tcpServer->disconnectDevice();
    emit closeModbusTcp();
}

void ModbusObj::connectRtuSerialModbus()
{

    emit startConnectModbusSerialRtu();

   // QHash<QString,QVariant>m_tcpValueHash;

}

void ModbusObj::connectTcpModbus()
{

     emit startConnectModbusTcp();

}

void ModbusObj::saveToJson()
{
    QmlForJson qmlForJson;


    if(m_jsonHash.isEmpty())
    {
        QHash<QString,QVariant>tcpModbusHash,serialModbusHash;
        tcpModbusHash["ipAddr"]="127.0.0.1";
        tcpModbusHash["port"]=5502;
        serialModbusHash["portName"]="";
        serialModbusHash["buadRate"]=9600;
        serialModbusHash["stopBit"] =1;
        serialModbusHash["dataBit"] =8;
        serialModbusHash["parity"]="NoParity";
        m_jsonHash["tcpModbus"]=tcpModbusHash;
        m_jsonHash["serialModbus"]=serialModbusHash;
    }
    qmlForJson.writeFile(m_jsonHash,m_fileDir);
}

void ModbusObj::setIsBase0(bool isBase0)
{
    m_isBase0 = isBase0;
}

void ModbusObj::settingTcpModbus(const QString &ipAddr, int port)
{
    QHash<QString,QVariant>tcpModbusHash;
    tcpModbusHash["ipAddr"]=ipAddr;
    tcpModbusHash["port"]=port;
    m_jsonHash["tcpModbus"]=tcpModbusHash;
}

void ModbusObj::settingSerialModbus(const QString &portName, int baudRate, int dataBit, int stopBit, const QString &parity)
{
    QHash<QString,QVariant>serialModbusHash;
    serialModbusHash["portName"]=portName;
    serialModbusHash["buadRate"]=baudRate;
    serialModbusHash["stopBit"] =stopBit;
    serialModbusHash["dataBit"] =dataBit;
    serialModbusHash["parity"]=parity;
    m_jsonHash["serialModbus"]=serialModbusHash;

}

QList<QVariant> ModbusObj::portNameModel()
{
    QList<QVariant> portNameList;
    QList<QSerialPortInfo>infoList=  QSerialPortInfo::availablePorts();
    foreach(const QSerialPortInfo&info,infoList)
    {
        portNameList.push_back(info.portName());
    }
    return portNameList;
}

QList<QVariant> ModbusObj::baudRateModel()
{
    QList<QVariant>baudRateList;
    QList<qint32>curBaudRateList = QSerialPortInfo::standardBaudRates();
    foreach(qint32 baudRate,curBaudRateList)
    {
        baudRateList.push_back(baudRate);
    }
    return  baudRateList;
}

QString ModbusObj::ipAddr()
{
    QHash<QString,QVariant>tcpModbusHash;

    tcpModbusHash=m_jsonHash["tcpModbus"].toHash();
    return tcpModbusHash.value("ipAddr").toString();
}

QString ModbusObj::ipPort()
{
    QHash<QString,QVariant>tcpModbusHash;

    tcpModbusHash=m_jsonHash["tcpModbus"].toHash();
    return QString::number(tcpModbusHash.value("port").toUInt());
}

QString ModbusObj::portName()
{
    QHash<QString,QVariant>serialModbusHash;

    serialModbusHash =m_jsonHash.value("serialModbus").toHash();
    return serialModbusHash.value("portName").toString();
}

QVariant ModbusObj::baudRate()
{
    QHash<QString,QVariant>serialModbusHash;
    serialModbusHash =m_jsonHash.value("serialModbus").toHash();
    return serialModbusHash.value("buadRate");
}

QVariant ModbusObj::dataBits()
{
    QHash<QString,QVariant>serialModbusHash;

    serialModbusHash =m_jsonHash.value("serialModbus").toHash();
    return serialModbusHash.value("dataBit");
}

QVariant ModbusObj::stopBits()
{
    QHash<QString,QVariant>serialModbusHash;

    serialModbusHash =m_jsonHash.value("serialModbus").toHash();
    return serialModbusHash.value("stopBit");
}

QVariant ModbusObj::parity()
{
    QHash<QString,QVariant>serialModbusHash;

    serialModbusHash =m_jsonHash.value("serialModbus").toHash();
    return serialModbusHash.value("parity");
}

ModbusObj::ModbusType &ModbusObj::modbusType()
{
    return  m_modbusType;
}

void ModbusObj::setModbusType(ModbusObj::ModbusType curModbusType)
{
    m_modbusType = curModbusType;
}
