#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QMutex>
#include <QDebug>
#include "dataStore/datastore.h"
#include <QTextToSpeech>
#include "commnication/commobj.h"
class Controller:public QObject
{
    Q_OBJECT
public:
    static Controller *instance();
    ~Controller();
    DataStore * getDataStore();
    QString fileNameFromQml(const QString &name);
    CommObj *getCommObj();
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

};

#endif // CONTROLLER_H



