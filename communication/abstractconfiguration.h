#ifndef ABSTRACTCONFIGURATION_H
#define ABSTRACTCONFIGURATION_H
#include <QHash>
#include <QVariant>
#include <QApplication>
#include <QSharedPointer>

class ConfigurationManager;
class AbstractConfiguration//配置虚基类
{
    friend class ConfigurationManager;
public:
    enum ConfigurationType
    {
        None,
        Serial,
        Tcp,
        Udp,
        Ftp,
        Can,
        Indicator
    };
    AbstractConfiguration();
    virtual ~AbstractConfiguration();
    virtual QVariant getConfiguration()=0;
    virtual void setConfiguration(const QVariant &configurationValue)=0;
    const QString &getConfigurationPath();
    ConfigurationType &getConfigurationType();
protected:
    static QHash <QString,QVariant> m_configurationHash;
    ConfigurationType m_configurationType;
private:
    const QString c_configurationPath = QApplication::applicationDirPath()+"/configuration.json";

};

typedef QSharedPointer<AbstractConfiguration>  Configuration;

#endif // ABSTRACTCONFIGURATION_H
