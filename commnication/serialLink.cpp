#include "seriallink.h"
#include <QtAlgorithms>
#include <QDebug>
SerialLink::SerialLink( QObject *parent)
    : AbstractLink(parent)
{
    m_thread = new QThread();
    m_serialPort = new QSerialPort();
    m_configuration = QSharedPointer<AbstractConfiguration>(new SerialConfiguration);
    m_serialPort->moveToThread(m_thread);
    moveToThread(m_thread);
    m_thread->start();
    qRegisterMetaType<QList<QString> >("QList<QString>");
    qRegisterMetaType<QList<qint32> >("QList <qint32>");
    connect(m_serialPort,&QSerialPort::readyRead,this,&SerialLink::readData);
    connect(this,&SerialLink::writeData,this,[=](const QByteArray &array)
    {
        m_serialPort->write(array);
    });
    connect(m_serialPort,&QSerialPort::errorOccurred,this,[=](QSerialPort::SerialPortError/* error*/)
    {
        emit errorInfo(m_serialPort->errorString());
        qDebug() << m_serialPort->errorString();
    });

//    connect(this,&SerialLink::serialSetting,this,[=]()
//    {

//    });

    connect(this,&SerialLink::startConnect,this,[=]()
    {
       m_isOpen= m_serialPort->open(QIODevice::ReadWrite);
       // m_serialPort->isOpen();
    });

    connect(this,&SerialLink::stopConnect,this,[=](){
        m_serialPort->close();
        m_isOpen = false;
    });
}

SerialLink::~SerialLink()
{
    disconnectLink();
    m_serialPort->deleteLater();
    m_thread->quit();
    m_thread->deleteLater();

}

QList<QSerialPortInfo> SerialLink::getSerialPortInfo()
{
    return QSerialPortInfo::availablePorts();
}

QList<QString> SerialLink::portName()
{
    QList <QSerialPortInfo> portInfoList = getSerialPortInfo();
    static  QList <QString> portNameList;
    foreach (QSerialPortInfo portInfo, portInfoList)
    {
        portNameList.push_back(portInfo.portName());
    }
    return portNameList;

}

QList<qint32> SerialLink::baudRates()
{
    static QList<qint32>baudRateList= QSerialPortInfo::standardBaudRates();
    std::sort(baudRateList.begin(),baudRateList.end(),std::greater<qint32>());
    //qSort(baudRateList.begin(),baudRateList.end(),qGreater<qint32>());
    return baudRateList ;
}

int SerialLink::portNameNum()
{
    return SerialLink::portName().size();
}

int SerialLink::baudRatesNum()
{
    return SerialLink::baudRates().size();
}

QString SerialLink::portNameValue(int pos)
{
    if(pos<portNameNum()&&pos>=0)
    {
        return SerialLink::portName().at(pos);
    }
    else
    {
        return "";
    }
}

quint32 SerialLink::baudRatesValue(int pos)
{
    if(pos<baudRatesNum()&&pos>=0)
    {
        return SerialLink::baudRates().at(pos);
    }
    else
    {
        return 0;
    }
}





void SerialLink::readData()
{
    QByteArray array = m_serialPort->readAll();
    emit getData(array);
}

void SerialLink::sendData(const QByteArray &array)
{
    emit writeData(array);
}

void SerialLink::setConfiguration()
{
  QList <QVariant>serialInfoList=  m_configuration.data()->getConfiguration();
  foreach (QVariant value, serialInfoList) {
      QHash<QString,QVariant>valueHash = value.toHash();
      QList<QString> keyList=valueHash.keys();
      if(keyList.contains("baudRate"))
      {
          m_serialPort->setBaudRate(valueHash["baudRate"].toInt());
      }
      else if(keyList.contains("dataBits"))
      {
          m_serialPort->setDataBits(QSerialPort::DataBits(valueHash["dataBits"].toInt()));
      }

      else if(keyList.contains("stopBits"))
      {
          m_serialPort->setStopBits(QSerialPort::StopBits(valueHash["stopBits"].toInt()));
      }

      else if(keyList.contains("parity"))
      {
          m_serialPort->setParity(QSerialPort::Parity(valueHash["parity"].toInt()));
      }

      else if(keyList.contains("portName"))
      {
          m_serialPort->setPortName(valueHash["portName"].toString());
      }
      else if(keyList.contains("flowControl"))
      {
          m_serialPort->setFlowControl(QSerialPort::FlowControl(valueHash["flowControl"].toInt()));
      }
  }
}


void SerialLink::connectLink()
{
    emit startConnect();
}

void SerialLink::disconnectLink()
{
    emit stopConnect();
}
