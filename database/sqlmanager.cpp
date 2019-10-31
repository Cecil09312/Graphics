#include "sqlmanager.h"
#include <QVariant>
#include "sqlitemanager.h"
#include <QDebug>
#include <QSqlError>
#include <QtConcurrent>
#include <QFuture>
#include "mysqlmanager.h"

class SqlManager::SqlManagerPrivate
{
public:
    SqlManagerPrivate()
    {

    }
    QSqlDatabase m_database;

};
SqlManager::SqlManager(QObject *parent)
    : QObject(parent),
      d(new SqlManagerPrivate)
{

}

SqlManager::~SqlManager()
{
    if(d!=nullptr)
    {
        delete d;
        d = nullptr;
    }

}
QStringList SqlManager::getTables()
{
    return d->m_database.tables();
}


void SqlManager::setDataBase(const QString &driver, const QString &connectionName, const QString &host,
                             const QString &user, const QString &password, const QString &dataBase, const int &port)
{
    QString defaultName = d->m_database.connectionName();

    if(defaultName!=connectionName && !connectionName.isEmpty())
    {
        d->m_database.removeDatabase(connectionName);
        d->m_database = QSqlDatabase::addDatabase(driver,connectionName);
    }
    else if(defaultName==connectionName && defaultName.isEmpty())
    {
        d->m_database.removeDatabase("defaultName");
        d->m_database = QSqlDatabase::addDatabase(driver,"defaultName");
    }

    d->m_database.setHostName(host);
    d->m_database.setUserName(user);
    d->m_database.setPassword(password);
    d->m_database.setDatabaseName(dataBase);
    d->m_database.setPort(port);
}



bool SqlManager::insertBatch(const QString &tableName, const QList<QVariant> &valueList)
{
    bool success = false;

    QFuture<void> future = QtConcurrent::run([&]
    {
        if(d->m_database.isOpen())
        {
            QString sqlStr = QString("insert into %1 values(").arg(tableName);
            for(int i=0;i<valueList.size();i++)
            {
                sqlStr+="?";
                if(i<valueList.size()-1)
                {
                    sqlStr+=",";
                }
            }
            sqlStr+=")";
            QSqlQuery query(d->m_database);
            d->m_database.transaction();
            QThread::msleep(10);
            query.prepare(sqlStr);
            foreach (QVariant value, valueList)
            {
                QList<QVariant> currentList= value.toList();
                query.addBindValue(currentList);
            }
            success= query.execBatch();
            d->m_database.commit();
            query.finish();
        }

    });
    future.waitForFinished();
    return success;
}

QStringList SqlManager::executeQuery(const QString &sql)
{
    QStringList valueList;
    bool isTransaction= false;
    QFuture<void> future = QtConcurrent::run([&]
    {

        if(d->m_database.isOpen())
        {
            QSqlQuery query(d->m_database);
            isTransaction=d->m_database.transaction();
            QThread::msleep(10);
            if(isTransaction)
            {
                query.prepare(sql);
                query.exec();
                QSqlRecord record = query.record();
                while (query.next())
                {
                    for(int i=0;i<record.count();i++)
                    {
                        valueList.push_back(query.value(i).toString());
                    }
                }
                d->m_database.commit();
                query.finish();
            }
            else
            {
                QThread::msleep(200);
            }
//            else
//            {
//                close();
//            }

        }

        if(sql.startsWith("insert")||sql.startsWith("update"))
        {
            emit dataCommitSuccess(isTransaction);
        }
    });

    future.waitForFinished();


    return valueList;
}

SqlManager*SqlManager::fromDriver(const QString &driver)
{

    if(driver.contains("SQLITE",Qt::CaseInsensitive))
    {
        return new SqliteManager;
    }
    else if(driver.contains("MYSQL",Qt::CaseInsensitive))
    {
        return new MySqlManager;
    }
    else
    {
        return nullptr;
    }
}

bool SqlManager::open()
{
    bool isOpen = d->m_database.open();
    emit dbConnected(isOpen);
    return isOpen;
}

bool SqlManager::isOpen() const
{
    return d->m_database.isOpen();
}

void SqlManager::close()
{
    d->m_database.close();
    emit dbConnected(false);
}

bool SqlManager::tableIsExist(const QString &tableName)
{
    return getTables().contains(tableName);
}

QSqlDatabase &SqlManager::getDatabase()
{
    return d->m_database;
}

int SqlManager::tableColumns(const QString &/*tableName*/)
{
    return 0;
}

QString SqlManager::dbName() const
{
    return d->m_database.databaseName();
}

QString SqlManager::hostName()
{
    return d->m_database.hostName();
}

void SqlManager::setHostName(const QString &host)
{
    d->m_database.setHostName(host);
}

QString SqlManager::userName()
{
    return d->m_database.userName();
}

void SqlManager::setUserName(const QString &user)
{
    d->m_database.setUserName(user);
}

QString SqlManager::password()
{
    return d->m_database.password();
}

void SqlManager::setPassword(const QString &passwordStr)
{
    d->m_database.setPassword(passwordStr);
}

QString SqlManager::databaseName()
{
    return d->m_database.databaseName();
}

void SqlManager::setDatabaseName(const QString &name)
{
    d->m_database.setDatabaseName(name);
}

int SqlManager::port()
{
    return d->m_database.port();
}

void SqlManager::setPort(int p)
{
    d->m_database.setPort(p);
}

QString SqlManager::connectionName()
{
    return d->m_database.connectionName();
}



