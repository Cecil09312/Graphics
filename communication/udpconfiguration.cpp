#include "udpconfiguration.h"

UdpConfiguration::UdpConfiguration()
{
  m_configurationType = Udp;
}

UdpConfiguration::~UdpConfiguration()
{

}

QVariant UdpConfiguration::getConfiguration()
{
    QmlForJson qmlForJson;
    QVariant data=  qmlForJson.readFile(getConfigurationPath());
    QHash<QString,QVariant>udpConfigurationHash = data.toHash();
    return udpConfigurationHash["udp"];
}

void UdpConfiguration::setConfiguration(const QVariant &configurationValue)
{
    m_configurationHash["udp"] = configurationValue;
}
