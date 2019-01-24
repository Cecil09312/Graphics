#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QMutex>
#include <QDebug>
#include "dataStore/datastore.h"
#include <QTextToSpeech>
#include "commnication/SerialLink.h"
#include "architePlan/sysarchiteplanview.h"
#include "architePlan/architeplanview.h"
#include "usermanager.h"
#include "speech/speechobj.h"
#include "commnication/udplink.h"
#include "commnication/configurationmanager.h"
#include "commnication/tcpconfiguration.h"
#include "commnication/modbusmanager.h"
#include "commnication/udpconfiguration.h"
#include "graphicsWidget/graphicsitem.h"
#include "graphicsWidget/graphicsitem.h"
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
    ArchitePlanView *getArchitePlanView() const;
    UserManager *getUserManager() const;
    UserManager::UserRight getUserRight();
    //SpeechObj *getSpeechObj();
    AbstractLink *getUdpObj();
    ConfigurationManager *getSerialConfigurationManager();
private:
    Controller();
private:
    //static Controller*m_controller;
    static QSharedPointer<Controller>m_controller;
    QSharedPointer<AbstractLink>m_commObj;
    SysArchitePlanView *m_sysArthitePlanView;
    ArchitePlanView *m_architePlanView;
    UserManager*m_userManager;

   // QSharedPointer<SpeechObj>m_speechObj;
    QSharedPointer<AbstractLink>m_udpObj;
    QSharedPointer<ConfigurationManager> m_serialConfigurationManager;
    QSharedPointer<ConfigurationManager> m_tcpConfigurationManager;
    QSharedPointer<ConfigurationManager> m_udpConfigurationManager;
    QSharedPointer<ModbusManager>m_modbusManager;

};

#endif // CONTROLLER_H



