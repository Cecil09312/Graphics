#include "configurationmanager.h"
#include "jsonEdit/qmlforjson.h"
#include <QDebug>
ConfigurationManager::ConfigurationManager(Configuration configuration,QObject *parent) : QObject(parent)
{
    m_configuration = configuration;
    QVariant serialInfo = QmlForJson::readFile(m_configuration.data()->c_configurationPath);
    QHash<QString,QVariant>serialValueHash = serialInfo.toHash();
    m_serialConfigurationHash = serialValueHash["serial"].toHash();

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
    QmlForJson::writeFile(m_configuration.data()->m_configurationHash,m_configuration.data()->c_configurationPath);
}

QString ConfigurationManager::currentPortName()
{
   return m_serialConfigurationHash["portName"].toString();
}

quint32 ConfigurationManager::currentBaudRate()
{
    return m_serialConfigurationHash["baudRate"].toUInt();
}

QString ConfigurationManager::currentParity()
{
    return m_serialConfigurationHash["parity"].toString();
}

QString ConfigurationManager::currentFlowControl()
{
     return m_serialConfigurationHash["flowControl"].toString();
}

int ConfigurationManager::currentDataBits()
{
     return m_serialConfigurationHash["dataBits"].toInt();
}

int ConfigurationManager::currentStopBits()
{
    return m_serialConfigurationHash["stopBits"].toInt();
}

QList<QSerialPortInfo> ConfigurationManager::getSerialPortInfo()
{
    return QSerialPortInfo::availablePorts();
}

QList<QString> ConfigurationManager::portName()
{
    QList <QSerialPortInfo> portInfoList = getSerialPortInfo();
    static  QList <QString> portNameList;
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

