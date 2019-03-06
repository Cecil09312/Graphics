#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <QObject>
#include "abstractconfiguration.h"
#include <QSharedPointer>
#include <QSerialPortInfo>
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

signals:

public slots:

private:
    QHash<QString,QVariant> m_configurationValue;
    Configuration m_configuration;
    QHash<QString,QVariant>m_serialConfigurationHash;
    QHash<QString,QVariant>m_tcpConfigurationHash;
    QHash<QString,QVariant>m_canConfigurationHash;
    QHash<QString,QVariant>m_udpConfigurationHash;
};

#endif // CONFIGURATIONMANAGER_H
