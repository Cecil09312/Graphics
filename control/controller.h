#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QMutex>
#include <QDebug>
#include "dataStore/datastore.h"
class Controller
{
public:
    static Controller *instance();
    ~Controller();
    DataStore * getDataStore();

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
               // qDebug() << "Delete:" << "m_controller";
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



