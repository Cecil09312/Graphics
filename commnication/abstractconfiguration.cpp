#include "abstractconfiguration.h"
#include "configurationmanager.h"
#include <QDebug>
QHash<QString,QVariant> AbstractConfiguration::m_configurationHash= QHash<QString,QVariant>();
AbstractConfiguration::AbstractConfiguration()
{

}

AbstractConfiguration::~AbstractConfiguration()
{
  qDebug() << "&&&&&&&&&&&&&&";
}

const QString &AbstractConfiguration::getConfigurationPath()
{
    return c_configurationPath;
}


