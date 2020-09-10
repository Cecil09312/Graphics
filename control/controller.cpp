#include "controller.h"
#include <QElapsedTimer>
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
    m_userManager->deleteLater();

    //m_speechObj.clear();
    //m_operatorInfo->deleteLater();
    m_operatorInfo.clear();
    //m_logMsg.clear();
    m_drawImageThread.clear();


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
    QStringList fileDirList=   fileName.split("\\");

    if(!fileDirList.isEmpty())
    {
        fileName=QString(fileDirList.join("/"));
    }

    if(fileName.startsWith("qrc"))
    {
        fileName.remove("qrc");
    }
#ifdef Q_OS_LINUX
    if(!QDir::isAbsolutePath(fileName))
    {
        fileName = "/"+fileName;
    }

#endif
    return fileName;
}

QString Controller::getFileNameFromUrl(const QString &url, bool isHasSuffix)
{
    QFileInfo fileInfo(url);
    if(isHasSuffix)
    {
        return  fileInfo.fileName();
    }
    else
    {

        return  fileInfo.baseName();
    }
}

bool Controller::sysOnLinux()
{
    bool isHide = false;
#ifdef Q_OS_WIN
    isHide= false;
#elif defined Q_OS_LINUX
    isHide= true;
#endif
    return isHide;
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
        return UserManager::User;
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



OperatorInfo *Controller::getOperatorInfo()
{
    return m_operatorInfo.data();
}

TransportInfo *Controller::getTransportInfo()
{
    return m_transportInfo.data();
}



//LogMsg *Controller::getLogMsg()
//{
//    return m_logMsg.data();
//}

void Controller::delayMs(int ms)
{
    QTime dieTime = QTime::currentTime().addMSecs(ms);

    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }


}

DataStore *Controller::getDataStore()
{
    return m_dataStore.data();
}

DrawImageThread *Controller::getDrawImageThread()
{
    return m_drawImageThread.data();
}


Controller::Controller()
{
    m_commObj = QSharedPointer<AbstractLink>(new SerialLink(),&QObject::deleteLater);
    m_tcpObj = QSharedPointer<AbstractLink>(new TcpLink(),&QObject::deleteLater);
   // m_IndicatorObj = QSharedPointer<AbstractLink>(new SpeechCom,&QObject::deleteLater);
    //m_logMsg = QSharedPointer<LogMsg>(new DebugLogMsg(nullptr),&QObject::deleteLater);
    m_userManager =new UserManager(this);
    m_speechObj = new SpeechObj;
    m_serialConfigurationManager =QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new SerialConfiguration)),&QObject::deleteLater);
    m_tcpConfigurationManager = QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new TcpConfiguration)),&QObject::deleteLater);
    m_ftpConfigurationManager = QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new FtpConfiguration)),&QObject::deleteLater);
    m_indicatorConfigurationManager = QSharedPointer<ConfigurationManager>(new ConfigurationManager(Configuration(new SpeechComConfiguration)),&QObject::deleteLater);
    m_operatorInfo = QSharedPointer<OperatorInfo>(new OperatorInfo,&QObject::deleteLater);
    m_transportInfo = QSharedPointer<TransportInfo>(new TransportInfo,&QObject::deleteLater);

    m_dataStore = QSharedPointer<DataStore>(new DataStore(this));
    m_drawImageThread = QSharedPointer<DrawImageThread>(new DrawImageThread,&QObject::deleteLater);





}


