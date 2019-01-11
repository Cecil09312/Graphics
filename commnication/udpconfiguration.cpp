#include "udpconfiguration.h"
#include "jsonEdit/qmlforjson.h"
UdpConfiguration::UdpConfiguration()
{

}

UdpConfiguration::~UdpConfiguration()
{

}

QVariant UdpConfiguration::getConfiguration()
{
    QVariant data=  QmlForJson::readFile(getConfigurationPath());
    QHash<QString,QVariant>udpConfigurationHash = data.toHash();
    return udpConfigurationHash["udp"];
}

void UdpConfiguration::setConfiguration(const QVariant &configurationValue)
{
    m_configurationHash["udp"] = configurationValue;
}
