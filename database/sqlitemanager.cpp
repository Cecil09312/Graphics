#include "sqlitemanager.h"
#include <QDebug>
#include "control/controller.h"
SqliteManager::SqliteManager(QObject *parent)
    :SqlManager(parent),
      m_startRunning(false),
      m_isAnalog(false)
{


    qRegisterMetaType<QSqlQuery>("const QSqlQuery &");
    qRegisterMetaType<QString>("const QString &");
    m_selectAlarmInfoList <<"网络号"<< "分机号"<<"回路号"<<"地址号"<<"电源地址"<< "设备"
                         <<"事件类型"<<"时间"<<"系统"<< "建筑名称"<<"楼层"<<"位置"<<"备注";
    m_thread = new QThread;
    m_timer = new QTimer;
    this->moveToThread(m_thread);
    m_timer->moveToThread(m_thread);
    m_thread->start();
    connect(this,&SqliteManager::startProcessData,this,[=](const QString &sql)
    {
        //QMutexLocker lock(&m_mutex);
        m_sqlList.push_back(sql);
        if(m_isAnalog)
        {
            processDb(m_sqlList);
            emit finishUpdateData();
            m_thread->msleep(10);
        }
        else
        {
            if(!m_timer->isActive())
            {
                m_timer->start(500);
            }
        }



    });
    connect(m_timer,&QTimer::timeout,this,[=]()
    {
       // QMutex mutex;
        QMutexLocker lock(&m_mutex);
        if(!m_sqlList.isEmpty())
        {
            processDb(m_sqlList);

            QSqlQuery selectQuery(getDb());
            if(getDb().isOpen())
            {
                selectQuery.exec(QString("select %1 from AlarmInfo  where 备注 != '%2' order by rowid desc limit 256").arg(m_selectAlarmInfoList.join(",")).arg("OK"));
            }
            emit selectData(selectQuery);

        }
        else
        {

            m_timer->stop();
            //emit finishUpdateData();
        }

        m_thread->msleep(20);

        //        m_thread->msleep(10);
    });
    // connect(m_customTimer,&QTimer::timeout,this,&SqliteManager::finishedProcessData);
}

SqliteManager::~SqliteManager()
{
    if(m_thread!=nullptr)
    {
        m_thread->quit();
        m_thread->deleteLater();
    }
    if(m_timer!=nullptr)
    {
        m_timer->stop();
        m_timer->deleteLater();
    }
}

QStringList SqliteManager::getDatabases()
{
    QStringList list;
    return list;
}

QStringList SqliteManager::executeQuery(const QString &sql)
{
    emit startProcessData(sql);
    return QStringList();
}

void SqliteManager::quitThread()
{
    m_startRunning = false;
    m_thread->quit();
    //    wait();
}

void SqliteManager::processAnalogData(bool isAnalog)
{
    m_isAnalog = isAnalog;
}

//void SqliteManager::run()
//{
//   QSqlQuery query(getDb());
//    while (m_startRunning)
//    {
//        if(m_sqlList.size()>0)
//        {
//            QMutexLocker lock(&m_mutex);

//            if(getDb().isOpen())
//            {
//               // QSqlQuery query(getDb());

//                bool isTransaction= getDb().transaction();
//                if(isTransaction)
//                {
//                    foreach (const QString &sql, m_sqlList)
//                    {
//                        query.prepare(sql);
//                        bool isSuccess=query.exec();
//                        if(isSuccess)
//                        {
//                            m_sqlList.removeOne(sql);
//                        }
//                        msleep(10);

//                    }
//                    getDb().commit();
//                    query.finish();
//                }


//            }
//        }
//        else
//        {
//            m_startRunning = false;
//        }

//      emit finishedProcessData();
//      msleep(150);

//    }
//}


