#include "modbusmanager.h"
#include <QModbusRtuSerialMaster>
#include <QModbusTcpClient>
#include <QSerialPort>
#include <QDebug>

ModbusManager::ModbusManager(Configuration configuration, QObject *parent):
    QObject(parent)
{
    m_configuration = configuration;
    m_thread = new QThread;
    qRegisterMetaType<ConnectState>("ConnectState");
    qRegisterMetaType<QModbusDataUnit>("const QModbusDataUnit &");
    qRegisterMetaType<QModbusDataUnit>("QModbusDataUnit");
    qRegisterMetaType<QModbusDevice::Error>("QModbusDevice::Error");
    qRegisterMetaType<QModbusDevice::State>("QModbusDevice::State");
    if(m_modbusClient.isNull())
    {
        QHash<QString,QVariant>valueHash =  m_configuration.data()->getConfiguration().toHash();
        if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Serial)
        {
            m_modbusClient = QSharedPointer<QModbusClient> (new QModbusRtuSerialMaster,&QObject::deleteLater);
            m_modbusClient.data()->setConnectionParameter(QModbusRtuSerialMaster::SerialPortNameParameter, valueHash["serialPort"].toString());
            m_modbusClient.data()->setConnectionParameter(QModbusRtuSerialMaster::SerialBaudRateParameter,valueHash["baudRate"].toInt());
            m_modbusClient.data()->setConnectionParameter(QModbusRtuSerialMaster::SerialStopBitsParameter, valueHash["stopBits"].toInt());
            m_modbusClient.data()->setConnectionParameter(QModbusRtuSerialMaster::SerialDataBitsParameter,valueHash["dataBits"].toInt());
            if(valueHash["parity"].toString()=="无校验")
            {
                m_modbusClient.data()->setConnectionParameter(QModbusRtuSerialMaster::SerialParityParameter,QSerialPort::NoParity);
            }
            else if(valueHash["parity"].toString()=="奇校验")
            {
                m_modbusClient.data()->setConnectionParameter(QModbusRtuSerialMaster::SerialParityParameter,QSerialPort::OddParity);
            }
            else if(valueHash["parity"].toString()=="偶校验")
            {
                m_modbusClient.data()->setConnectionParameter(QModbusRtuSerialMaster::SerialParityParameter,QSerialPort::EvenParity);
            }

        }
        else if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Tcp)
        {
            m_modbusClient = QSharedPointer<QModbusClient> (new QModbusTcpClient,&QObject::deleteLater);
            m_modbusClient.data()->setConnectionParameter(QModbusTcpClient::NetworkAddressParameter,valueHash["hostAddr"].toString());
            m_modbusClient.data()->setConnectionParameter(QModbusTcpClient::NetworkPortParameter,valueHash["port"].toUInt());
        }

    }


    m_modbusClient->moveToThread(m_thread);
    m_modbusClient.data()->moveToThread(m_thread);
    this->moveToThread(m_thread);
    m_thread->start();
    QObject::connect(this,&ModbusManager::connectModbus,this,[=](ConnectState state)
    {
        if(state==DisConnected)
        {
            m_modbusClient.data()->disconnectDevice();
        }
        else if(state==Connected)
        {
            m_modbusClient.data()->connectDevice();
        }
        emit currentState(m_modbusClient.data()->state());
    });

    connect(this,&ModbusManager::writeRequest,this,[=](const QModbusDataUnit &write, int serverAddress)
    {
        QModbusReply *reply=  m_modbusClient.data()->sendWriteRequest(write,serverAddress);
        if(reply!=nullptr)
        {
            connect(reply,&QModbusReply::finished,this,[=]()
            {
                emit writeRequestData(reply->result());
                reply->deleteLater();
            });

            connect(reply,&QModbusReply::errorOccurred,this,[=](QModbusDevice::Error error)
            {
                if(error!=QModbusDevice::NoError)
                {
                    emit errorHappen(reply->errorString());
                }
                reply->deleteLater();
            });

        }
    });


    connect(this,&ModbusManager::readRequest,this,[=](const QModbusDataUnit &read, int serverAddress)
    {
        QModbusReply *reply=  m_modbusClient.data()->sendReadRequest(read,serverAddress);
        if(reply!=nullptr)
        {
            connect(reply,&QModbusReply::finished,this,[=]()
            {
                emit readRequestData(reply->result());
                reply->deleteLater();
            });
            connect(reply,&QModbusReply::errorOccurred,this,[=](QModbusDevice::Error error)
            {
                if(error!=QModbusDevice::NoError)
                {
                    emit errorHappen(reply->errorString());
                }
                reply->deleteLater();
            });

        }
    });

    connect(m_modbusClient.data(),&QModbusClient::stateChanged,this,&ModbusManager::currentState);
}

ModbusManager::~ModbusManager()
{
    m_thread->quit();
    m_thread->deleteLater();
}

void ModbusManager::connectDevice(ConnectState state)
{
    emit connectModbus(state);
}

void ModbusManager::sendWriteRequest(const QModbusDataUnit &write, int serverAddress)
{
    emit writeRequest(write,serverAddress);
}

void ModbusManager::sendReadRequest(const QModbusDataUnit &read, int serverAddress)
{
    emit readRequest(read,serverAddress);
}

