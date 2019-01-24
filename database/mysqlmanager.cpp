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

QStringList MySqlManager::getTables(QString dataBase)
{
    return executeQuery(QString("select table_name from information_schema.tables where table_schema='%1' and table_type='base table'").arg(dataBase));
}
