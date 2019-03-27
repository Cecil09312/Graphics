#ifndef MYSQLMANAGER_H
#define MYSQLMANAGER_H
#include "sqlmanager.h"

class MySqlManager : public SqlManager
{
public:
    MySqlManager();
    ~MySqlManager();
    QStringList getDatabases();
    int tableColumns(const QString &tableName);
};

#endif // MYSQLMANAGER_H
