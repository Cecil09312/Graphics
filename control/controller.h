#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QMutex>
#include <QDebug>
#include "dataStore/datastore.h"
#include <QTextToSpeech>
#include "commnication/commobj.h"
#include "architePlan/sysarchiteplanview.h"
#include "architePlan/architeplanview.h"
#include "usermanager.h"
class Controller:public QObject
{
    Q_OBJECT
public:

    static Controller *instance();
    ~Controller();
    DataStore * getDataStore();
    Q_INVOKABLE QString fileNameFromQml(const QString &name);
    CommObj *getCommObj();
    void setSysArchitePlanView(SysArchitePlanView*sysArchitePlanView);
    SysArchitePlanView *getSysArchitePlanView() const;
    void setArchitePlanView(ArchitePlanView *architePlanView);
    ArchitePlanView *getArchitePlanView() const;
    UserManager *getUserManager() const;
    UserManager::UserRight getUserRight();
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
    CommObj *m_commObj;
    SysArchitePlanView *m_sysArthitePlanView;
    ArchitePlanView *m_architePlanView;
    UserManager *m_userManager;

};

#endif // CONTROLLER_H



