#include "serialconfiguration.h"
#include <QDebug>
SerialConfiguration::SerialConfiguration()
{
    m_configurationType = Serial;
}

SerialConfiguration::~SerialConfiguration()
{

}

QVariant SerialConfiguration::getConfiguration()
{
    QmlForJson qmlForJson;
    QVariant data=  qmlForJson.readFile(getConfigurationPath());
    QHash<QString,QVariant>serialConfigurationHash = data.toHash();
    return serialConfigurationHash["serial"];
}

void SerialConfiguration::setConfiguration(const QVariant &configurationValue)
{
     m_configurationHash["serial"] = configurationValue;
}
