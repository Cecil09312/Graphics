#include "configurationmanager.h"
#include <QDebug>
ConfigurationManager::ConfigurationManager(Configuration configuration,QObject *parent) : QObject(parent)
{
    m_configuration = configuration;

    QmlForJson qmlForJson;
    QVariant configurationInfo = qmlForJson.readFile(m_configuration.data()->c_configurationPath);
    QHash<QString,QVariant>configurationValueHash = configurationInfo.toHash();
    m_serialConfigurationHash = configurationValueHash["serial"].toHash();
    m_tcpConfigurationHash = configurationValueHash["tcp"].toHash();
    m_canConfigurationHash = configurationValueHash["can"].toHash();
    m_udpConfigurationHash = configurationValueHash["udp"].toHash();
    m_ftpConfigurationHash = configurationValueHash["ftp"].toHash();
    m_indicatorConfigurationHash = configurationValueHash["indicator"].toHash();
    if(m_serialConfigurationHash.isEmpty())
    {
        m_serialConfigurationHash["portName"]="";
        m_serialConfigurationHash["baudRate"] = 9600;
        m_serialConfigurationHash["dataBits"] = 8;
        m_serialConfigurationHash["stopBits"] = 1;
        m_serialConfigurationHash["parity"]  = "无校验";
        m_serialConfigurationHash["flowControl"] = "无";
        // m_configuration.data()->setConfiguration(m_serialConfigurationHash);

    }

    if(m_indicatorConfigurationHash.isEmpty())
    {
        m_indicatorConfigurationHash["portName"]="";
        m_indicatorConfigurationHash["baudRate"] = 9600;
        m_indicatorConfigurationHash["dataBits"] = 8;
        m_indicatorConfigurationHash["stopBits"] = 1;
        m_indicatorConfigurationHash["parity"]  = "无校验";
        m_indicatorConfigurationHash["flowControl"] = "无";
    }
    if(m_tcpConfigurationHash.isEmpty())
    {
        m_tcpConfigurationHash["hostAddr"] = "127.0.0.1";
        m_tcpConfigurationHash["port"] = 8080;

        // m_configuration.data()->setConfiguration(m_tcpConfigurationHash);
    }
    if(m_canConfigurationHash.isEmpty())
    {
        m_canConfigurationHash["plugin"] ="socketcan";
        m_canConfigurationHash["interfaceName"] = "vcan0";
    }
    if(m_udpConfigurationHash.isEmpty())
    {
        m_udpConfigurationHash["readAddr"] = "127.0.0.1";
        m_udpConfigurationHash["readPort"] = 8080;
        m_udpConfigurationHash["sendAddr"] = "127.0.0.1";
        m_udpConfigurationHash["sendPort"] = 8080;
    }

    if(m_ftpConfigurationHash.isEmpty())
    {
        m_ftpConfigurationHash["host"] = "127.0.0.1";
        m_ftpConfigurationHash["port"] =21;
        m_ftpConfigurationHash["user"] = "sss";
        m_ftpConfigurationHash["password"] = "1234";
    }
    if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Serial)
    {
        m_configuration.data()->setConfiguration(m_serialConfigurationHash);
    }
    else if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Tcp)
    {
        m_configuration.data()->setConfiguration(m_tcpConfigurationHash);
    }
    else if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Can)
    {
        m_configuration.data()->setConfiguration(m_canConfigurationHash);
    }
    else if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Udp)
    {
        m_configuration.data()->setConfiguration(m_udpConfigurationHash);
    }
    else if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Ftp)
    {
        m_configuration.data()->setConfiguration(m_ftpConfigurationHash);
    }
    else if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Indicator){
        m_configuration.data()->setConfiguration(m_indicatorConfigurationHash);
    }
}

ConfigurationManager::~ConfigurationManager()
{

}

void ConfigurationManager::setConfiguration()
{
    m_configuration.data()->setConfiguration(m_configurationValue);
}

void ConfigurationManager::setConfigurationValue(const QString &key, const QVariant &value)
{
    m_configurationValue[key] = value;
}

void ConfigurationManager::saveConfiguration()
{
    QmlForJson qmlForJson;
    qmlForJson.writeFile(m_configuration.data()->m_configurationHash,m_configuration.data()->c_configurationPath);
}

QString ConfigurationManager::currentPortName()
{
    if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Indicator)
    {
        return  m_indicatorConfigurationHash["portName"].toString();
    }
    else
    {
        return m_serialConfigurationHash["portName"].toString();
    }
}

quint32 ConfigurationManager::currentBaudRate()
{
    if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Indicator)
    {
        return m_indicatorConfigurationHash["baudRate"].toUInt();
    }
    else
    {
        return m_serialConfigurationHash["baudRate"].toUInt();
    }

}

QString ConfigurationManager::currentParity()
{
    if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Indicator)
    {
        return m_indicatorConfigurationHash["parity"].toString();
    }
    else
    {
        return m_serialConfigurationHash["parity"].toString();
    }

}

QString ConfigurationManager::currentFlowControl()
{
    if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Indicator)
    {
        return m_indicatorConfigurationHash["flowControl"].toString();
    }
    else
    {
        return m_serialConfigurationHash["flowControl"].toString();
    }
}

int ConfigurationManager::currentDataBits()
{
    if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Indicator)
    {
        return m_indicatorConfigurationHash["dataBits"].toInt();
    }
    else
    {
        return m_serialConfigurationHash["dataBits"].toInt();
    }
}

int ConfigurationManager::currentStopBits()
{   if(m_configuration.data()->getConfigurationType()==AbstractConfiguration::Indicator)
    {
        return m_indicatorConfigurationHash["stopBits"].toInt();
    }
    else
    {
        return m_serialConfigurationHash["stopBits"].toInt();
    }
}

QString ConfigurationManager::ftpHost()
{
    return m_ftpConfigurationHash["host"].toString();
}

int ConfigurationManager::ftpPort()
{
    return m_ftpConfigurationHash["port"].toString().toInt();
}

QString ConfigurationManager::ftpUser()
{
    return m_ftpConfigurationHash["user"].toString();
}

QString ConfigurationManager::ftpPassword()
{
    return m_ftpConfigurationHash["password"].toString();
}

QString ConfigurationManager::tcpAddr()
{
    return m_tcpConfigurationHash["hostAddr"].toString();
}

int ConfigurationManager::tcpPort()
{
    return m_tcpConfigurationHash["port"].toInt();
}

QList<QSerialPortInfo> ConfigurationManager::getSerialPortInfo()
{
    return QSerialPortInfo::availablePorts();
}

QList<QString> ConfigurationManager::portName()
{
    QList <QSerialPortInfo> portInfoList = getSerialPortInfo();
   QList <QString> portNameList;
    foreach (QSerialPortInfo portInfo, portInfoList)
    {
        portNameList.push_back(portInfo.portName());
    }
    return portNameList;
}

QList<qint32> ConfigurationManager::baudRates()
{
    static QList<qint32>baudRateList= QSerialPortInfo::standardBaudRates();
    std::sort(baudRateList.begin(),baudRateList.end(),std::greater<qint32>());
    //qSort(baudRateList.begin(),baudRateList.end(),qGreater<qint32>());
    return baudRateList ;
}

int ConfigurationManager::portNameNum()
{
    return portName().size();
}

int ConfigurationManager::baudRatesNum()
{
    return baudRates().size();
}

QString ConfigurationManager::portNameValue(int pos)
{
    if(pos<portNameNum()&&pos>=0)
    {
        return portName().at(pos);
    }
    else
    {
        return "";
    }
}

quint32 ConfigurationManager::baudRatesValue(int pos)
{
    if(pos<baudRatesNum()&&pos>=0)
    {
        return baudRates().at(pos);
    }
    else
    {
        return 0;
    }
}

