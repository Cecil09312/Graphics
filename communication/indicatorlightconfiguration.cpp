#include "indicatorlightconfiguration.h"
#include "jsonEdit/qmlforjson.h"
IndicatorLightConfiguration::IndicatorLightConfiguration()
{
    m_configurationType = Indicator;
}

IndicatorLightConfiguration::~IndicatorLightConfiguration()
{

}

QVariant IndicatorLightConfiguration::getConfiguration()
{
    QmlForJson qmlForJson;
    QVariant data=  qmlForJson.readFile(getConfigurationPath());
    QHash<QString,QVariant>serialConfigurationHash = data.toHash();
    return serialConfigurationHash["indicator"];
}

void IndicatorLightConfiguration::setConfiguration(const QVariant &configurationValue)
{
    m_configurationHash["indicator"] = configurationValue;
}
