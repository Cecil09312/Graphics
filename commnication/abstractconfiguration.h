#ifndef ABSTRACTCONFIGURATION_H
#define ABSTRACTCONFIGURATION_H
#include <QHash>
#include <QVariant>
#include <QApplication>

class AbstractConfiguration
{
public:
    AbstractConfiguration();
    virtual ~AbstractConfiguration();
    virtual QList<QVariant> getConfiguration()=0;
    virtual void setConfiguration(const QList<QVariant>&configurationList)=0;
    const QString &getConfigurationPath();
protected:
    static QHash <QString,QVariant> m_configurationHash;
private:
    const QString c_configurationPath = QApplication::applicationDirPath()+"/configuration.json";

};

#endif // ABSTRACTCONFIGURATION_H
