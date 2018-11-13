#include "sqlitemanager.h"

SqliteManager::SqliteManager(QObject *parent)
    :SqlManager(parent)
{

}

QStringList SqliteManager::getDatabases()
{
    QStringList list;
    return list;
}

QStringList SqliteManager::getTables(QString /*dataBase*/)
{
    QStringList list;
    list= executeQuery( "SELECT name FROM sqlite_master WHERE type='table'");
    return list;
}
