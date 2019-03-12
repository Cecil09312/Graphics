#include "tcpconfiguration.h"
TcpConfiguration::TcpConfiguration()
{
  m_configurationType = Tcp;
}

TcpConfiguration::~TcpConfiguration()
{

}

QVariant TcpConfiguration::getConfiguration()
{

    QmlForJson qmlForJson;
    QVariant data=  qmlForJson.readFile(getConfigurationPath());
    QHash<QString,QVariant>tcpConfigurationHash = data.toHash();
    return tcpConfigurationHash["tcp"];
}

void TcpConfiguration::setConfiguration(const QVariant &configurationValue)
{
    m_configurationHash["tcp"] = configurationValue;
}
