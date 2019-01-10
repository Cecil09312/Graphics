#include "abstractconfiguration.h"
QHash<QString,QVariant> AbstractConfiguration::m_configurationHash= QHash<QString,QVariant>();
AbstractConfiguration::AbstractConfiguration()
{

}

AbstractConfiguration::~AbstractConfiguration()
{

}

const QString &AbstractConfiguration::getConfigurationPath()
{
    return c_configurationPath;
}


