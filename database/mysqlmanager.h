#ifndef MYSQLMANAGER_H
#define MYSQLMANAGER_H
#include "sqlmanager.h"

class MySqlManager : public SqlManager
{
public:
    MySqlManager();
    ~MySqlManager();
    QStringList getDatabases();
    QStringList getTables(QString dataBase);
};

#endif // MYSQLMANAGER_H
