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
    m_commObj.clear();
    m_udpObj.clear();
    m_speechObj.clear();
    //m_userManager.clear();
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

AbstractLink*Controller::getCommObj()
{
    return m_commObj.data();
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
    return m_userManager;
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

AbstractLink *Controller::getUdpObj()
{
    return m_udpObj.data();
}

ConfigurationManager *Controller::getSerialConfigurationManager()
{
    return m_serialConfigurationManager.data();
}

Controller::Controller()
{
    m_dataStore =QSharedPointer<DataStore>(new DataStore);
    m_commObj = QSharedPointer<AbstractLink>(new SerialLink());
    m_userManager =new UserManager(this) ;
    m_speechObj = QSharedPointer<SpeechObj>(new SpeechObj/*,&QObject::deleteLater*/);

    m_udpObj =QSharedPointer<AbstractLink> (new UdpLink);
    m_serialConfigurationManager =QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new SerialConfiguration),this)) ;
   // m_udpObj->connectLink();

}


