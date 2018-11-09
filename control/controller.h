#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QMutex>
#include <QDebug>
#include "dataStore/datastore.h"
#include <QTextToSpeech>
class Controller
{
public:
    static Controller *instance();
    ~Controller();
    DataStore * getDataStore();
    QString fileNameFromQml(const QString &name);

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

};

#endif // CONTROLLER_H



