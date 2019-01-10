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
    if(m_dataStore!=nullptr)
    {
        delete m_dataStore;
        m_dataStore= nullptr;
    }
    m_commObj->deleteLater();
    if(m_userManager!=nullptr)
    {
        m_userManager->deleteLater();
        m_userManager = nullptr;
    }

    if(m_speechObj!=nullptr)
    {
        m_speechObj->stopSpeech();
        m_speechObj->deleteLater();
        m_speechObj = nullptr;
    }

    if(m_udpObj!=nullptr)
    {
        m_udpObj->deleteLater();
        m_udpObj = nullptr;
    }

}

DataStore *Controller::getDataStore()
{
    return  m_dataStore;
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
    return m_userManager;
}

UserManager::UserRight Controller::getUserRight()
{
    if(m_userManager!=nullptr)
    {
        return m_userManager->userRight();
    }
    else
    {

        return UserManager::Employee;
    }
}

SpeechObj *Controller::getSpeechObj()
{
    return m_speechObj;
}

UdpObj *Controller::getUdpObj()
{
    return m_udpObj;
}

Controller::Controller()
{
    m_dataStore = new DataStore;
    m_commObj = new SerialLink();
    m_userManager = new UserManager;
    m_speechObj = new SpeechObj;
    m_udpObj = new UdpObj;
  //  m_udpObj->bindToHost("127.0.0.1",8080);

}


