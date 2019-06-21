#include "abstractconfiguration.h"
#include "configurationmanager.h"
#include <QDebug>
QHash<QString,QVariant> AbstractConfiguration::m_configurationHash= QHash<QString,QVariant>();
AbstractConfiguration::AbstractConfiguration()
{
   m_configurationType = None;
}

AbstractConfiguration::~AbstractConfiguration()
{

}

const QString &AbstractConfiguration::getConfigurationPath()
{
    return c_configurationPath;
}

AbstractConfiguration::ConfigurationType &AbstractConfiguration::getConfigurationType()
{
    return m_configurationType;
}


