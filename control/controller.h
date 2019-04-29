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
#include "communication/modbusmanager.h"
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
class Controller:public QObject
{
    Q_OBJECT
public:

    static Controller *instance();
    ~Controller();
    Q_INVOKABLE QString fileNameFromQml(const QString &name);
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
    AbstractLink *getUdpObj();
    AbstractLink *getTcpObj();
    AbstractLink *getIndicatorObj();
    ConfigurationManager *getSerialConfigurationManager();
    ConfigurationManager *getFtpConfigurationManager();
    ConfigurationManager *getTcpConfigurationManager();
    ConfigurationManager *getIndicatorConfigurationManager();
    SqlManager *getMySqlManager();
    OperatorInfo *getOperatorInfo();
    TransportInfo *getTransportInfo();
private:
    Controller();
private:
    //static Controller*m_controller;
    static QSharedPointer<Controller>m_controller;
    QSharedPointer<AbstractLink>m_commObj;
    QSharedPointer<AbstractLink>m_tcpObj;
    QSharedPointer<AbstractLink>m_IndicatorObj;
    SysArchitePlanView *m_sysArthitePlanView;
    ArchitePlanView *m_architePlanView;
    UserManager*m_userManager;
    CrtWidget *m_crtWidget;

    SpeechObj*m_speechObj;
    AbstractLink*m_udpObj;
    QSharedPointer<ConfigurationManager> m_serialConfigurationManager;
    QSharedPointer<ConfigurationManager> m_tcpConfigurationManager;
    QSharedPointer<ConfigurationManager> m_udpConfigurationManager;
    QSharedPointer<ConfigurationManager> m_ftpConfigurationManager;
     QSharedPointer<ConfigurationManager> m_indicatorConfigurationManager;
    OperatorInfo*m_operatorInfo;
    QSharedPointer<TransportInfo>m_transportInfo;
    SqlManager *m_mysqlManager;
    // QSharedPointer<ModbusManager>m_modbusManager;

};

#endif // CONTROLLER_H



