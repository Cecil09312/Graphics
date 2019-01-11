#include "tcpconfiguration.h"
#include "jsonEdit/qmlforjson.h"
TcpConfiguration::TcpConfiguration()
{

}

TcpConfiguration::~TcpConfiguration()
{

}

QVariant TcpConfiguration::getConfiguration()
{
    QVariant data=  QmlForJson::readFile(getConfigurationPath());
    QHash<QString,QVariant>tcpConfigurationHash = data.toHash();
    return tcpConfigurationHash["tcp"];
}

void TcpConfiguration::setConfiguration(const QVariant &configurationValue)
{
    m_configurationHash["tcp"] = configurationValue;
}
