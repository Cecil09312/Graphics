#ifndef INDICATORLIGHTCOM_H
#define INDICATORLIGHTCOM_H
#include "abstractlink.h"
#include <QSerialPort>
class IndicatorLightCom : public AbstractLink
{
public:
    IndicatorLightCom();
    ~IndicatorLightCom();
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
   Configuration m_indicatorConfiguration;
};

#endif // INDICATORLIGHTCOM_H
