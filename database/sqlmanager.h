#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlRecord>

class SqlManager : public QObject
{
    Q_OBJECT
public:
    explicit SqlManager(QObject *parent = nullptr);
    virtual ~SqlManager();
    virtual QStringList getDatabases()=0;
    QStringList getTables();
    void setDataBase(const QString &driver,const QString &conectionName, const QString &host,
                     const QString &user,const QString &password,const QString &dataBase,const int &port);
    bool insertBatch(const QString &tableName, const QList<QVariant> &valueList);
    QStringList executeQuery(const QString &sql);
    static SqlManager *fromDriver(const QString &driver);
    bool open();
    bool isOpen() const;
    void close();
    QSqlDatabase &getDatabase();
    virtual int tableColumns(const QString &tableName);
    QString dbName() const;

signals:

public slots:

protected:
    class SqlManagerPrivate;
    SqlManagerPrivate *d;
};

#endif // DATABASE_H
