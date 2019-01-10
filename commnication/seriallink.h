#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <QSerialPortInfo>
#include "abstractconfiguration.h"
#include "abstractlink.h"
#include "serialconfiguration.h"

class SerialLink : public AbstractLink
{
    Q_OBJECT
public:
    explicit SerialLink(QObject *parent = nullptr);
    ~SerialLink();
    static QList<QSerialPortInfo> getSerialPortInfo();
    static QList<QString> portName();
    static QList<qint32> baudRates();
    Q_INVOKABLE int portNameNum();
    Q_INVOKABLE int baudRatesNum();
    Q_INVOKABLE QString portNameValue(int pos);
    Q_INVOKABLE quint32 baudRatesValue(int pos);

public slots:
    void readData();
    void sendData(const QByteArray &array);
    void setConfiguration();
    void connectLink();
    void disconnectLink();

private :
    QSerialPort *m_serialPort;
    QThread *m_thread;
    qint32 m_baud;
    QSerialPort::DataBits m_dataBits;
    QSerialPort::StopBits m_stopBits;
    QString m_portName;
    QSerialPort::FlowControl m_flowControl;
    QSerialPort::Parity m_parity;
    bool m_isOpen;
    QSharedPointer<AbstractConfiguration>m_configuration;
};

#endif // COMMOBJ_H
