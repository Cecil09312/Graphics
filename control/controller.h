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
#include "commnication/udpobj.h"
class Controller:public QObject
{
    Q_OBJECT
public:

    static Controller *instance();
    ~Controller();
    DataStore * getDataStore();
    Q_INVOKABLE QString fileNameFromQml(const QString &name);
    SerialLink *getCommObj();
    void setSysArchitePlanView(SysArchitePlanView*sysArchitePlanView);
    SysArchitePlanView *getSysArchitePlanView() const;
    void setArchitePlanView(ArchitePlanView *architePlanView);
    ArchitePlanView *getArchitePlanView() const;
    UserManager *getUserManager() const;
    UserManager::UserRight getUserRight();
    SpeechObj *getSpeechObj();
    UdpObj *getUdpObj();
public:
    class AutoDelete
    {
    public:
        ~AutoDelete()
        {
            if(m_controller!=nullptr)
            {
                delete m_controller;
                m_controller = nullptr;
            }
        }
    };

private:
    Controller();

private:
    static Controller*m_controller;
    DataStore *m_dataStore;
    SerialLink *m_commObj;
    SysArchitePlanView *m_sysArthitePlanView;
    ArchitePlanView *m_architePlanView;
    UserManager *m_userManager;
    SpeechObj *m_speechObj;
    UdpObj *m_udpObj;

};

#endif // CONTROLLER_H



