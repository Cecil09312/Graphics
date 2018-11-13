#include "sqlmanager.h"
#include <QVariant>
#include "sqlitemanager.h"
#include <QDebug>
#include <QSqlError>
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
    d->m_database = QSqlDatabase::addDatabase(driver,conectionName);
    d->m_database.setHostName(host);
    d->m_database.setUserName(user);
    d->m_database.setPassword(password);
    d->m_database.setDatabaseName(dataBase);
    d->m_database.setPort(port);
}

SqlManager *SqlManager::fromDriverName(const QString &driver)
{

    static SqlManager * result = nullptr;
    const QString driv = driver.toUpper();
    if(result==nullptr)
    {
        if(driv.contains("SQLITE"))
            result = new SqliteManager();
    }
    return result;
}

bool SqlManager::insertBatch(const QString &sql, const QList<QVariant> &valueList)
{
    bool success = false;
    QSqlQuery query(d->m_database);
    query.prepare(sql);
    foreach (QVariant value, valueList)
    {
        QList<QVariant> currentList= value.toList();
        query.addBindValue(currentList);
    }
    success= query.execBatch();
    query.finish();
    return success;
}

QStringList SqlManager::executeQuery(const QString &sql)
{
    QStringList valueList;
    QSqlQuery query(d->m_database);
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
    query.finish();
    return valueList;
}

bool SqlManager::open()
{
    return d->m_database.open();
}

bool SqlManager::isOpen() const
{
    return d->m_database.isOpen();
}
