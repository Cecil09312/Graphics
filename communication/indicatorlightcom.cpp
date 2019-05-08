#include "indicatorlightcom.h"
#include "indicatorlightconfiguration.h"
IndicatorLightCom::IndicatorLightCom()
{
    m_thread = new QThread();
    m_serialPort = new QSerialPort();
    m_indicatorConfiguration = Configuration(new IndicatorLightConfiguration);
    m_serialPort->moveToThread(m_thread);
    moveToThread(m_thread);
    m_thread->start();
    qRegisterMetaType<QList<QString> >("QList<QString>");
    qRegisterMetaType<QList<qint32> >("QList <qint32>");

    connect(m_serialPort,&QSerialPort::readyRead,this,&IndicatorLightCom::readData);
    connect(this,&IndicatorLightCom::writeData,this,[=](const QByteArray &array)
    {
        m_serialPort->write(array);
    });
    connect(m_serialPort,&QSerialPort::errorOccurred,this,[=](QSerialPort::SerialPortError/* error*/)
    {
        emit errorInfo(m_serialPort->errorString());
    });



    connect(this,&IndicatorLightCom::startConnect,this,[=]()
    {
        QHash <QString,QVariant>valueHash=  m_indicatorConfiguration.data()->getConfiguration().toHash();
        QString portName = valueHash["portName"].toString();
        if(m_serialPort->portName()!=portName)
        {
            m_serialPort->close();
            m_serialPort->setPortName(portName);
        }
        if(!m_serialPort->isOpen())
        {
            m_isOpen= m_serialPort->open(QIODevice::ReadWrite);
            if(m_isOpen)
            {
                setConfiguration();
            }
        }
        else
        {
            m_isOpen = true;
            setConfiguration();
        }
        emit isConnected(m_isOpen);

    });

    connect(this,&IndicatorLightCom::stopConnect,this,[=]()
    {
        m_serialPort->close();
        m_isOpen = false;
        emit isConnected(m_isOpen);
    });

}

IndicatorLightCom::~IndicatorLightCom()
{
    disconnectLink();
    m_serialPort->deleteLater();
    m_thread->quit();
    m_thread->deleteLater();
}

void IndicatorLightCom::readData()
{
    QByteArray array = m_serialPort->readAll();
    emit getData(array);
}


void IndicatorLightCom::setConfiguration()
{
    QHash <QString,QVariant>valueHash=  m_indicatorConfiguration.data()->getConfiguration().toHash();
    m_serialPort->setBaudRate(valueHash["baudRate"].toInt());
    m_serialPort->setDataBits(QSerialPort::DataBits(valueHash["dataBits"].toInt()));
    m_serialPort->setStopBits(QSerialPort::StopBits(valueHash["stopBits"].toInt()));
    QString parityStr = valueHash["parity"].toString();
    if(parityStr=="无校验")
    {
        m_serialPort->setParity(QSerialPort::NoParity);
    }
    else if(parityStr=="奇校验")
    {
        m_serialPort->setParity(QSerialPort::OddParity);
    }
    else if(parityStr=="偶校验")
    {
        m_serialPort->setParity(QSerialPort::EvenParity);
    }
    //m_serialPort->setPortName(valueHash["portName"].toString());
    QString flowControl= valueHash["flowControl"].toString();
    if(flowControl=="无")
    {
        m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    }
}
