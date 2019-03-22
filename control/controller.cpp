#include "controller.h"

QSharedPointer<Controller>Controller::m_controller = QSharedPointer<Controller>(nullptr);
Controller *Controller::instance()
{
    if(m_controller.isNull())
    {
        m_controller = QSharedPointer<Controller>(new Controller);
    }
    return m_controller.data();
}

Controller::~Controller()
{
    m_commObj.clear();
    m_serialConfigurationManager.data()->saveConfiguration();
    m_tcpConfigurationManager.data()->saveConfiguration();
    m_udpConfigurationManager.data()->saveConfiguration();
    m_speechObj.clear();
    m_udpObj->deleteLater();
    m_operatorInfo->deleteLater();
    //m_userManager.clear();
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

void Controller::setCrtWidget(CrtWidget *widget)
{
    m_crtWidget = widget;
}

CrtWidget *Controller::getCrtWidget()
{
    return m_crtWidget;
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
    return m_udpObj;
}


ConfigurationManager *Controller::getSerialConfigurationManager()
{
    return m_serialConfigurationManager.data();
}

ConfigurationManager *Controller::getFtpConfigurationManager()
{
    return m_ftpConfigurationManager.data();
}

ConfigurationManager *Controller::getTcpConfigurationManager()
{
    return m_tcpConfigurationManager.data();
}

OperatorInfo *Controller::getOperatorInfo()
{
    return m_operatorInfo;
}

Controller::Controller()
{
    m_commObj = QSharedPointer<AbstractLink>(new SerialLink(),&QObject::deleteLater);
    m_userManager =new UserManager(this);
    m_speechObj = QSharedPointer<SpeechObj>(new SpeechObj/*,&QObject::deleteLater*/);
    m_udpObj =new UdpLink;
    m_serialConfigurationManager =QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new SerialConfiguration),this)) ;
    m_tcpConfigurationManager = QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new TcpConfiguration),this)) ;
    m_udpConfigurationManager = QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new UdpConfiguration),this)) ;
    m_ftpConfigurationManager = QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new FtpConfiguration),this));
    m_operatorInfo = new OperatorInfo;
    // m_modbusManager = QSharedPointer<ModbusManager>(new ModbusManager(Configuration(new TcpConfiguration)),&QObject::deleteLater);
   // m_udpObj->connectLink();
    // m_modbusManager.data()->connectDevice(ModbusManager::Connected);
}


