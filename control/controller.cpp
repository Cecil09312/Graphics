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
    m_speechObj->deleteLater();
    m_tcpObj.clear();
    m_IndicatorObj.clear();
    m_operatorInfo->deleteLater();
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
    if(!QDir::isAbsolutePath(fileName))
    {
        fileName = "/"+fileName;
    }

#endif
    return fileName;
}

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
    return m_speechObj;
}

AbstractLink *Controller::getTcpObj()
{
    return m_tcpObj.data();
}

AbstractLink *Controller::getIndicatorObj()
{
    return m_IndicatorObj.data();
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

ConfigurationManager *Controller::getIndicatorConfigurationManager()
{
    return m_indicatorConfigurationManager.data();
}

SqlManager *Controller::getMySqlManager()
{
    return m_mysqlManager;
}

OperatorInfo *Controller::getOperatorInfo()
{
    return m_operatorInfo;
}

TransportInfo *Controller::getTransportInfo()
{
    return m_transportInfo.data();
}

Controller::Controller()
{
    m_commObj = QSharedPointer<AbstractLink>(new SerialLink(),&QObject::deleteLater);
    m_tcpObj = QSharedPointer<AbstractLink>(new TcpLink(),&QObject::deleteLater);
    m_IndicatorObj = QSharedPointer<AbstractLink>(new IndicatorLightCom,&QObject::deleteLater);
    m_userManager =new UserManager(this);
    m_speechObj = new SpeechObj();
    m_serialConfigurationManager =QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new SerialConfiguration),this));
    m_tcpConfigurationManager = QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new TcpConfiguration),this));
    m_ftpConfigurationManager = QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new FtpConfiguration),this));
    m_indicatorConfigurationManager = QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new IndicatorLightConfiguration),this));
    m_operatorInfo = new OperatorInfo;
    m_transportInfo = QSharedPointer<TransportInfo>(new TransportInfo,&QObject::deleteLater);
    m_mysqlManager = SqlManager::fromDriver("QMYSQL");
}


