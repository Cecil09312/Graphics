#ifndef ABSTRACTCONFIGURATION_H
#define ABSTRACTCONFIGURATION_H
#include <QHash>
#include <QVariant>
#include <QApplication>
#include <QSharedPointer>
class ConfigurationManager;

class AbstractConfiguration
{
    friend class ConfigurationManager;
public:
    AbstractConfiguration();
    virtual ~AbstractConfiguration();
    virtual QVariant getConfiguration()=0;
    virtual void setConfiguration(const QVariant &configurationValue)=0;
    const QString &getConfigurationPath();
protected:
    static QHash <QString,QVariant> m_configurationHash;
private:
    const QString c_configurationPath = QApplication::applicationDirPath()+"/configuration.json";

};

typedef QSharedPointer<AbstractConfiguration>  Configuration;

#endif // ABSTRACTCONFIGURATION_H
