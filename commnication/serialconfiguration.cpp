#include "serialconfiguration.h"
#include "jsonEdit/qmlforjson.h"
SerialConfiguration::SerialConfiguration()
{

}

SerialConfiguration::~SerialConfiguration()
{

}

QList<QVariant> SerialConfiguration::getConfiguration()
{
    QVariant data=  QmlForJson::readFile(getConfigurationPath());
    QHash<QString,QVariant>serialConfigurationHash = data.toHash();
    return serialConfigurationHash["serial"].toList();
}

void SerialConfiguration::setConfiguration(const QList<QVariant> &configurationList)
{
     m_configurationHash["serial"] = configurationList;
}
