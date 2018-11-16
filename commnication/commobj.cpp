#include "commobj.h"
#include <QtAlgorithms>
#include <QDebug>
CommObj::CommObj(QObject *parent) : QObject(parent)
{
    m_thread = new QThread(this);
    m_serialPort = new QSerialPort();
    m_serialPort->moveToThread(m_thread);
    moveToThread(m_thread);
    m_thread->start();
    qRegisterMetaType<QList<QString> >("QList<QString>");
    qRegisterMetaType<QList<qint32> >("QList <qint32>");
    connect(m_serialPort,&QSerialPort::readyRead,this,&CommObj::readData);
    connect(this,&CommObj::send,this,[=](const QByteArray &array)
    {
        m_serialPort->write(array);
    });
    connect(m_serialPort,&QSerialPort::errorOccurred,this,[=](QSerialPort::SerialPortError/* error*/)
    {
        emit errorInfo(m_serialPort->errorString());
        qDebug() << m_serialPort->errorString();
    });

    connect(this,&CommObj::serialSetting,this,[=]()
    {
        m_serialPort->setBaudRate(m_baud);
        m_serialPort->setDataBits(m_dataBits);
        m_serialPort->setStopBits(m_stopBits);
        m_serialPort->setParity(m_parity);
        m_serialPort->setPortName(m_portName);
        m_serialPort->setFlowControl(m_flowControl);
    });

    connect(this,&CommObj::openSerialPort,this,[=]()
    {
       m_isOpen= m_serialPort->open(QIODevice::ReadWrite);
       // m_serialPort->isOpen();
    });

    connect(this,&CommObj::closeSerialPort,this,[=](){
        m_serialPort->close();
        m_isOpen = false;
    });
}

CommObj::~CommObj()
{
    close();
    m_thread->quit();
    m_thread->wait();
    m_serialPort->deleteLater();
}

QList<QSerialPortInfo> CommObj::getSerialPortInfo()
{
    return QSerialPortInfo::availablePorts();
}

QList<QString> CommObj::portName()
{
    QList <QSerialPortInfo> portInfoList = getSerialPortInfo();
    static  QList <QString> portNameList;
    foreach (QSerialPortInfo portInfo, portInfoList)
    {
        portNameList.push_back(portInfo.portName());
    }
    return portNameList;

}

QList<qint32> CommObj::baudRates()
{
    static QList<qint32>baudRateList= QSerialPortInfo::standardBaudRates();
    std::sort(baudRateList.begin(),baudRateList.end(),std::greater<qint32>());
    //qSort(baudRateList.begin(),baudRateList.end(),qGreater<qint32>());
    return baudRateList ;
}

int CommObj::portNameNum()
{
    return CommObj::portName().size();
}

int CommObj::baudRatesNum()
{
    return CommObj::baudRates().size();
}

QString CommObj::portNameValue(int pos)
{
    if(pos<portNameNum()&&pos>=0)
    {
        return CommObj::portName().at(pos);
    }
    else
    {
        return "";
    }
}

quint32 CommObj::baudRatesValue(int pos)
{
    if(pos<baudRatesNum()&&pos>=0)
    {
        return CommObj::baudRates().at(pos);
    }
    else
    {
        return 0;
    }
}

void CommObj::setBaudRade(qint32 baud)
{
    m_baud = baud;
    emit serialSetting();
}

void CommObj::setDataBits(QSerialPort::DataBits dataBits)
{
    m_dataBits = dataBits;
    emit serialSetting();
}

void CommObj::setStopBits(QSerialPort::StopBits stopBits)
{
    m_stopBits = stopBits;
    emit serialSetting();
}

void CommObj::setPortName(const QString &portName)
{
    m_portName = portName;
    emit serialSetting();
}

void CommObj::setFlowContral(QSerialPort::FlowControl flowControl)
{
    m_flowControl = flowControl;
    emit serialSetting();
}

void CommObj::setParity(QSerialPort::Parity parity)
{
    m_parity = parity;
    emit serialSetting();
}

void CommObj::open()
{
    emit openSerialPort();
}

void CommObj::close()
{
    emit closeSerialPort();
}

bool CommObj::isOpen()
{
    return m_isOpen;
}

void CommObj::readData()
{
    QByteArray array = m_serialPort->readAll();
}

void CommObj::sendData(const QByteArray &array)
{
    emit send(array);
}
