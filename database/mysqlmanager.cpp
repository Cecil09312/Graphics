#include "mysqlmanager.h"

MySqlManager::MySqlManager()
{

}

MySqlManager::~MySqlManager()
{

}

QStringList MySqlManager::getDatabases()
{
   return executeQuery("show databases");
}



int MySqlManager::tableColumns(const QString &tableName)
{
    QStringList list;
    list = executeQuery(QString("select count(*) from information_schema.columns where table_schema='%1' and table_name='%2'").arg(dbName()).arg(tableName));
    if(list.size()>0)
    {
        QString column = list.at(0);
        return column.toInt();
    }
    else
    {
        return 0;
    }
}
