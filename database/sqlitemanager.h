#ifndef SQLITEMANAGER_H
#define SQLITEMANAGER_H
#include "sqlmanager.h"

class SqliteManager : public SqlManager
{
    Q_OBJECT
public:
    SqliteManager(QObject *parent = nullptr);
    QStringList getDatabases();
};

#endif // SQLITEMANAGER_H
