#include "seriallink.h"
#include <QtAlgorithms>
#include <QDebug>
#include <QMutexLocker>

SerialLink::SerialLink( QObject *parent)
    : AbstractLink(parent),
     m_isOpen(false)
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
        //QMutexLocker locker(&m_mutex);

        m_sendArrayList.push_back(array);
        if(m_serialPort->isOpen())
        {
            foreach (QByteArray curArray, m_sendArrayList)
            {

                qint64 size=  m_serialPort->write(curArray);
               // qDebug() << "curArray" << curArray.toHex();
                m_serialPort->flush();
                if(size>0)
                {
                    m_sendArrayList.removeOne(curArray);
                }
                m_thread->msleep(10);
            }

        }
    });



    connect(this,&SerialLink::startConnect,this,[=]()
    {

        QHash <QString,QVariant>valueHash=  m_serialConfiguration.data()->getConfiguration().toHash();
        QString portName = valueHash.value("portName").toString();
        m_serialPort->close();//关闭以前的连接
        if(m_serialPort->portName()!=portName)
        {
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
    //QMutexLocker locker(&m_mutex);
    QByteArray array = m_serialPort->readAll();
    emit getData(array);
    m_thread->msleep(5);
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



