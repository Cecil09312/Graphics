#include "seriallink.h"
#include <QtAlgorithms>
#include <QDebug>
SerialLink::SerialLink( QObject *parent)
    : AbstractLink(parent)
{
    m_thread = new QThread();
    m_serialPort = new QSerialPort();
    m_serialConfiguration = Configuration(new SerialConfiguration);
    m_serialPort->moveToThread(m_thread);
    moveToThread(m_thread);
    m_thread->start();
    qRegisterMetaType<QList<QString> >("QList<QString>");
    qRegisterMetaType<QList<qint32> >("QList <qint32>");

    connect(m_serialPort,&QSerialPort::readyRead,this,&SerialLink::readData);
    connect(this,&SerialLink::writeData,this,[=](const QByteArray &array)
    {
        if(m_serialPort->isOpen())
        {
           m_serialPort->write(array);
        }
    });



    connect(this,&SerialLink::startConnect,this,[=]()
    {
        QHash <QString,QVariant>valueHash=  m_serialConfiguration.data()->getConfiguration().toHash();
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



    connect(this,&SerialLink::stopConnect,this,[=]()
    {
        m_serialPort->close();
        m_isOpen = false;
        emit isConnected(m_isOpen);
    });

//    connect(m_serialPort,QOverload<QSerialPort::SerialPortError>::of(&QSerialPort::error),this,[=](QSerialPort::SerialPortError serialPortError)
//    {
//        Q_UNUSED(serialPortError);
//        emit errorInfo(m_serialPort->errorString());
//        disconnectLink();
//    });

}

SerialLink::~SerialLink()
{
    disconnectLink();
    m_serialPort->deleteLater();
    m_thread->quit();
    m_thread->deleteLater();
}

void SerialLink::readData()
{
    QByteArray array = m_serialPort->readAll();
    emit getData(array);
}


void SerialLink::setConfiguration()
{
    QHash <QString,QVariant>valueHash=  m_serialConfiguration.data()->getConfiguration().toHash();
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



