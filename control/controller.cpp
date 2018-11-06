#include "controller.h"
Controller*Controller::m_controller = nullptr;
Controller::AutoDelete controlAutoDelete;
Controller *Controller::instance()
{
    if(m_controller==nullptr)
    {
        QMutex mutex;
        mutex.lock();
        if(m_controller==nullptr)
        {
            Controller *controller = new Controller;
            m_controller = controller;
        }

        mutex.unlock();
    }
    return m_controller;
}

Controller::~Controller()
{
    if(m_dataStore!=nullptr)
    {
        delete m_dataStore;
        m_dataStore= nullptr;
       // qDebug() << "Delete:" << "m_dataStore";
    }

}

DataStore *Controller::getDataStore()
{
    return  m_dataStore;
}

Controller::Controller()
{
   m_dataStore = new DataStore;
}
