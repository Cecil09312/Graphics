#include "tcpconfiguration.h"
#include "jsonEdit/qmlforjson.h"
TcpConfiguration::TcpConfiguration()
{

}

TcpConfiguration::~TcpConfiguration()
{

}

QList<QVariant> TcpConfiguration::getConfiguration()
{
    QVariant data=  QmlForJson::readFile(getConfigurationPath());
    QHash<QString,QVariant>serialConfigurationHash = data.toHash();
    return serialConfigurationHash["tcp"].toList();
}

void TcpConfiguration::setConfiguration(const QList<QVariant> &configurationList)
{
    m_configurationHash["tcp"] = configurationList;
}
