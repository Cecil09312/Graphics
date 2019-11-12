#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <QObject>
#include <QSerialPort>
#include <QThread>

#include "abstractconfiguration.h"
#include "abstractlink.h"
#include "serialconfiguration.h"
#include <QMutex>
#include "dataStore/abstractdataprotocol.h"
#include "dataStore/serialdataprotocol.h"
class SerialLink : public AbstractLink
{
    Q_OBJECT
public:
    explicit SerialLink(QObject *parent = nullptr);
    ~SerialLink();

public slots:
    void readData();
    void setConfiguration();
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
    QMutex m_mutex;
    Configuration m_serialConfiguration;
    QList<QByteArray>m_sendArrayList;
};

#endif // COMMOBJ_H
