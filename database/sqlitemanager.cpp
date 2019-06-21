#include "sqlitemanager.h"
#include <QDebug>
SqliteManager::SqliteManager(QObject *parent)
    :SqlManager(parent)
{

}

QStringList SqliteManager::getDatabases()
{
    QStringList list;
    return list;
}


