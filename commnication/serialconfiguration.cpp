#include "serialconfiguration.h"
#include "jsonEdit/qmlforjson.h"
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
    QVariant data=  QmlForJson::readFile(getConfigurationPath());
    QHash<QString,QVariant>serialConfigurationHash = data.toHash();
    return serialConfigurationHash["serial"];
}

void SerialConfiguration::setConfiguration(const QVariant &configurationValue)
{
     m_configurationHash["serial"] = configurationValue;
}
