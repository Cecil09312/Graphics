#include "controller.h"
Controller*Controller::m_controller = nullptr;
Controller::AutoDelete controlAutoDelete;
Controller *Controller::instance()
{
    if(m_controller==nullptr)
    {
        QMutex mutex;
        mutex.lock();
        if(m_controller==nullptr)
        {
            Controller *controller = new Controller;
            m_controller = controller;
        }

        mutex.unlock();
    }
    return m_controller;
}

Controller::~Controller()
{
    m_commObj->deleteLater();

    if(m_udpObj!=nullptr)
    {
        m_udpObj->deleteLater();
        m_udpObj = nullptr;
    }

    m_speechObj.clear();
}

DataStore *Controller::getDataStore()
{
    return  m_dataStore.data();
}

QString Controller::fileNameFromQml(const QString &name)
{
    QString fileName = name;

    if(fileName.startsWith("file:///"))
    {
        QList<QString> nameList=  fileName.split("file:///");
        int size = nameList.size();
        if(size>0)
        {
            fileName = nameList.at(size-1);
        }
    }
#ifdef Q_OS_LINUX
    fileName = "/"+fileName;
#endif
    return fileName;
}

//QList<QString> Controller::portNameList()
//{
//    return SerialLink::portName();
//}

SerialLink *Controller::getCommObj()
{
    return m_commObj;
}

void Controller::setSysArchitePlanView(SysArchitePlanView *sysArchitePlanView)
{
    m_sysArthitePlanView = sysArchitePlanView;
}

SysArchitePlanView *Controller::getSysArchitePlanView() const
{
    return m_sysArthitePlanView;
}

void Controller::setArchitePlanView(ArchitePlanView *architePlanView)
{
    m_architePlanView = architePlanView;
}

ArchitePlanView *Controller::getArchitePlanView() const
{
    return m_architePlanView;
}

UserManager *Controller::getUserManager() const
{
    return m_userManager.data();
}

UserManager::UserRight Controller::getUserRight()
{
    if(getUserManager()!=nullptr)
    {
        return getUserManager()->userRight();
    }
    else
    {

        return UserManager::Employee;
    }
}

SpeechObj *Controller::getSpeechObj()
{
    return m_speechObj.data();
}

UdpLink *Controller::getUdpObj()
{
    return m_udpObj;
}

ConfigurationManager *Controller::getSerialConfigurationManager()
{
    return m_serialConfigurationManager.data();
}

Controller::Controller()
{
    m_dataStore =QSharedPointer<DataStore>(new DataStore);
    m_commObj = new SerialLink();
    m_userManager =QSharedPointer<UserManager>(new UserManager(this)) ;
    m_speechObj = QSharedPointer<SpeechObj>(new SpeechObj,&QObject::deleteLater);

    m_udpObj = new UdpLink;
    m_serialConfigurationManager =QSharedPointer<ConfigurationManager>(new ConfigurationManager(QSharedPointer<AbstractConfiguration>(new SerialConfiguration),this)) ;
  // m_udpObj->bindToHost("127.0.0.1",8080);
    m_udpObj->connectLink();

}


