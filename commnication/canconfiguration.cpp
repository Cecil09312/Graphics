#include "canconfiguration.h"
#include "jsonEdit/qmlforjson.h"
CanConfiguration::CanConfiguration()
{
   m_configurationType = Can;
}

CanConfiguration::~CanConfiguration()
{

}

QVariant CanConfiguration::getConfiguration()
{
    QVariant data=  QmlForJson::readFile(getConfigurationPath());
    QHash<QString,QVariant>canConfigurationHash = data.toHash();
    return canConfigurationHash["can"];
}

void CanConfiguration::setConfiguration(const QVariant &configurationValue)
{
    m_configurationHash["can"] = configurationValue;
}
