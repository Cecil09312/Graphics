#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <QObject>
#include "abstractconfiguration.h"
#include <QSharedPointer>
#include <QSerialPortInfo>
#include "jsonEdit/qmlforjson.h"
//typedef QSharedPointer<AbstractConfiguration> Configuration;
class ConfigurationManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigurationManager(Configuration configuration, QObject *parent = nullptr);
    ~ConfigurationManager();
    Q_INVOKABLE void setConfiguration();
    static QList<QSerialPortInfo> getSerialPortInfo();
    static QList<QString> portName();
    static QList<qint32> baudRates();
    Q_INVOKABLE int portNameNum();
    Q_INVOKABLE int baudRatesNum();
    Q_INVOKABLE QString portNameValue(int pos);
    Q_INVOKABLE quint32 baudRatesValue(int pos);
    Q_INVOKABLE void setConfigurationValue(const QString &key,const QVariant &value);
    Q_INVOKABLE void saveConfiguration();
    Q_INVOKABLE QString currentPortName();
    Q_INVOKABLE quint32 currentBaudRate();
    Q_INVOKABLE QString currentParity();
    Q_INVOKABLE QString currentFlowControl();
    Q_INVOKABLE int currentDataBits();
    Q_INVOKABLE int currentStopBits();
    Q_INVOKABLE QString ftpHost();
    Q_INVOKABLE int ftpPort();
    Q_INVOKABLE QString ftpUser();
    Q_INVOKABLE QString ftpPassword();
    Q_INVOKABLE QString tcpAddr();
    Q_INVOKABLE int tcpPort();

signals:

public slots:

private:
    QHash<QString,QVariant> m_configurationValue;
    Configuration m_configuration;
    QHash<QString,QVariant>m_serialConfigurationHash;
    QHash<QString,QVariant>m_tcpConfigurationHash;
    QHash<QString,QVariant>m_canConfigurationHash;
    QHash<QString,QVariant>m_udpConfigurationHash;
    QHash<QString,QVariant>m_ftpConfigurationHash;
    QHash<QString,QVariant>m_indicatorConfigurationHash;
};

#endif // CONFIGURATIONMANAGER_H
