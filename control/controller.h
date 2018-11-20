#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QMutex>
#include <QDebug>
#include "dataStore/datastore.h"
#include <QTextToSpeech>
#include "commnication/commobj.h"
#include "architePlan/sysarchiteplanview.h"
#include "architePlan/architeplanview.h"

class Controller:public QObject
{
    Q_OBJECT
public:
    static Controller *instance();
    ~Controller();
    DataStore * getDataStore();
    QString fileNameFromQml(const QString &name);
    CommObj *getCommObj();
    void setSysArchitePlanView(SysArchitePlanView*sysArchitePlanView);
    SysArchitePlanView *getSysArchitePlanView();
    void setArchitePlanView(ArchitePlanView *architePlanView);
    ArchitePlanView *getArchitePlanView();
// Q_INVOKABLE QList<QString>portNameList();
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

};

#endif // CONTROLLER_H



