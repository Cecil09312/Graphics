#include "ftpconfiguration.h"
#include "jsonEdit/qmlforjson.h"

FtpConfiguration::FtpConfiguration()
{
    m_configurationType = Ftp;
}

QVariant FtpConfiguration::getConfiguration()
{
    QmlForJson qmlForJson;
    QVariant data=  qmlForJson.readFile(getConfigurationPath());
    QHash<QString,QVariant>ftpConfigurationHash = data.toHash();
    return ftpConfigurationHash["ftp"];
}

void FtpConfiguration::setConfiguration(const QVariant &configurationValue)
{
    m_configurationHash["ftp"] = configurationValue;
}
