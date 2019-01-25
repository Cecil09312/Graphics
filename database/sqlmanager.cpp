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


void SqlManager::setDataBase(const QString &driver, const QString &conectionName, const QString &host,
                             const QString &user, const QString &password, const QString &dataBase, const int &port)
{
    QString defaultName = d->m_database.connectionName();

    if(defaultName!=conectionName && !conectionName.isEmpty())
    {
        d->m_database.removeDatabase(conectionName);
        d->m_database = QSqlDatabase::addDatabase(driver,conectionName);
    }
    else if(defaultName==conectionName && defaultName.isEmpty())
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

        query.prepare(sqlStr);
        foreach (QVariant value, valueList)
        {
            QList<QVariant> currentList= value.toList();
            query.addBindValue(currentList);
        }
        success= query.execBatch();
        d->m_database.commit();
        query.finish();

    });
    future.waitForFinished();


    return success;
}

QStringList SqlManager::executeQuery(const QString &sql)
{
    QStringList valueList;

    QFuture<void> future = QtConcurrent::run([&]
    {
        QSqlQuery query(d->m_database);
        d->m_database.transaction();
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
    return d->m_database.open();

}

bool SqlManager::isOpen() const
{
    return d->m_database.isOpen();
}

void SqlManager::close()
{
    d->m_database.close();
}

QSqlDatabase &SqlManager::getDatabase()
{
    return d->m_database;
}
