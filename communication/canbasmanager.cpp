#include "canbasmanager.h"
#include <QCanBusDeviceInfo>
#include "canconfiguration.h"
CanbasManager::CanbasManager(QObject *parent)
    : QObject(parent),
      m_canbasDevice(QSharedPointer<QCanBusDevice>(nullptr))
{
    m_thread = new QThread;
    m_canConfiguration = Configuration(new CanConfiguration);
    this->moveToThread(m_thread);
    qRegisterMetaType<ConnectCanState>("ConnectCanState");
    qRegisterMetaType<QByteArray>("const QByteArray &");
    qRegisterMetaType<QByteArray>("QByteArray");
    qRegisterMetaType<QCanBusFrame>("const QCanBusFrame &");
    qRegisterMetaType<QCanBusDevice::CanBusDeviceState>("QCanBusDevice::CanBusDeviceState");
    setCanConfigure();
    createDevice();
}

CanbasManager::~CanbasManager()
{
    m_thread->quit();
    m_thread->deleteLater();
}

void CanbasManager::writeFrame(const QCanBusFrame &frame)
{
    emit writeData(frame);
}

void CanbasManager::connectDevice(CanbasManager::ConnectCanState state)
{
    emit connectCan(state);
}

void CanbasManager::createDevice()
{
    if(m_canbasDevice.isNull())
    {
        m_canbasDevice = QSharedPointer<QCanBusDevice>(QCanBus::instance()->createDevice(m_plugin,m_interfaceName),&QObject::deleteLater);
        Q_ASSERT(m_canbasDevice);
        Q_ASSERT(m_canbasDevice.data());
        m_canbasDevice->moveToThread(m_thread);
        m_canbasDevice.data()->moveToThread(m_thread);
        m_thread->start();
        connect(this,&CanbasManager::connectCan,this,[=](ConnectCanState state)
        {
            if(state==ConnectedCan)
            {
                if(m_canbasDevice.data()->state()!=QCanBusDevice::ConnectedState)
                {
                    m_canbasDevice.data()->connectDevice();
                }

            }
            else if(state==DisConnectedCan)
            {
                if(m_canbasDevice.data()->state()==QCanBusDevice::ConnectedState)
                {
                    m_canbasDevice.data()->disconnectDevice();
                }

            }
            emit currentState(m_canbasDevice.data()->state());
        });

        connect(m_canbasDevice.data(),&QCanBusDevice::stateChanged,this,&CanbasManager::currentState);
        connect(this,&CanbasManager::writeData,this,[=](const QCanBusFrame &frame)
        {
            m_canbasDevice.data()->writeFrame(frame);
        });
        connect(m_canbasDevice.data(),&QCanBusDevice::framesReceived,this,[=](){
            QCanBusFrame currentFrame = m_canbasDevice.data()->readFrame();
            QByteArray currentArray=  currentFrame.payload();
            emit readFrame(currentArray);
        });

        connect(m_canbasDevice.data(),&QCanBusDevice::errorOccurred,this,[=](QCanBusDevice::CanBusError error)
        {
            Q_UNUSED(error)
            emit errorHanppen(m_canbasDevice.data()->errorString());
        });
    }
}

void CanbasManager::deleteDevice()
{
    if(!m_canbasDevice.isNull())
    {
        m_canbasDevice.clear();
    }
}

void CanbasManager::setCanConfigure()
{
    QVariant canInfo =   m_canConfiguration.data()->getConfiguration();
    QHash<QString,QVariant>canValueHash = canInfo.toHash();
    m_plugin = canValueHash["plugin"].toString();
    m_interfaceName = canValueHash["interfaceName"].toString();
}
