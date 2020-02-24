#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QMutex>
#include <QDebug>
#include "dataStore/datastore.h"
#include <QTextToSpeech>
#include "communication/seriallink.h"
#include "architePlan/sysarchiteplanview.h"
#include "architePlan/architeplanview.h"
#include "usermanager.h"
#include "speech/speechobj.h"
#include "communication/udplink.h"
#include "communication/configurationmanager.h"
#include "communication/tcpconfiguration.h"
#include "communication/udpconfiguration.h"
#include "graphicsWidget/graphicsitem.h"
#include "graphicsWidget/graphicsitem.h"
#include "jsonEdit/qmlforjson.h"
#include "communication/ftpconfiguration.h"
#include "crtwidget.h"
#include "database/operatorinfo.h"
#include "jsonEdit/transportinfo.h"
#include "communication/indicatorlightconfiguration.h"
#include "communication/indicatorlightcom.h"
#include "database/mysqlmanager.h"
#include <QFileInfo>
#include "LogMsg/logmsg.h"
#include "LogMsg/debuglogmsg.h"

class Controller:public QObject
{
    Q_OBJECT
public:
    static Controller *instance();
    ~Controller();
    Q_INVOKABLE QString fileNameFromQml(const QString &name);
    Q_INVOKABLE QString getFileNameFromUrl(const QString &url, bool isHasSuffix=false);
    Q_INVOKABLE bool hideOnLinux();

    AbstractLink *getCommObj();
    void setSysArchitePlanView(SysArchitePlanView*sysArchitePlanView);
    SysArchitePlanView *getSysArchitePlanView() const;
    void setArchitePlanView(ArchitePlanView *architePlanView);
    void setCrtWidget(CrtWidget *widget);
    CrtWidget *getCrtWidget();
    ArchitePlanView *getArchitePlanView() const;
    UserManager *getUserManager() const;
    UserManager::UserRight getUserRight();
    SpeechObj *getSpeechObj();
    AbstractLink *getTcpObj();
    AbstractLink *getIndicatorObj();
    ConfigurationManager *getSerialConfigurationManager();
    ConfigurationManager *getFtpConfigurationManager();
    ConfigurationManager *getTcpConfigurationManager();
    ConfigurationManager *getIndicatorConfigurationManager();
    SqlManager *getMySqlManager();
    OperatorInfo *getOperatorInfo();
    TransportInfo *getTransportInfo();
    LogMsg *getLogMsg();
    void delayMs(int ms);//非阻塞延时

private:
    Controller();
private:
    static QSharedPointer<Controller>m_controller;
    QSharedPointer<AbstractLink>m_commObj;
    QSharedPointer<AbstractLink>m_tcpObj;
    QSharedPointer<AbstractLink>m_IndicatorObj;
    SysArchitePlanView *m_sysArthitePlanView;
    ArchitePlanView *m_architePlanView;
    UserManager*m_userManager;
    CrtWidget *m_crtWidget;
    SpeechObj*m_speechObj;
    QSharedPointer<ConfigurationManager> m_serialConfigurationManager;
    QSharedPointer<ConfigurationManager> m_tcpConfigurationManager;
    QSharedPointer<ConfigurationManager> m_ftpConfigurationManager;
    QSharedPointer<ConfigurationManager> m_indicatorConfigurationManager;
    QSharedPointer<OperatorInfo>m_operatorInfo;
    QSharedPointer<TransportInfo>m_transportInfo;
    SqlManager *m_mysqlManager;
    QSharedPointer<LogMsg>m_logMsg;

};

#endif // CONTROLLER_H



