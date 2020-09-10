#include "speechcomconfiguration.h"
#include "jsonEdit/qmlforjson.h"
SpeechComConfiguration::SpeechComConfiguration()
{
    m_configurationType = SpeechSerialCom;
}

SpeechComConfiguration::~SpeechComConfiguration()
{

}

QVariant SpeechComConfiguration::getConfiguration()
{
    QmlForJson qmlForJson;
    QVariant data=  qmlForJson.readFile(getConfigurationPath());
    QHash<QString,QVariant>serialConfigurationHash = data.toHash();
    return serialConfigurationHash["speechSerialCom"];
}

void SpeechComConfiguration::setConfiguration(const QVariant &configurationValue)
{
    m_configurationHash["speechSerialCom"] = configurationValue;
}
