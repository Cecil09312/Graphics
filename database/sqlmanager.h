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
    virtual QStringList getDatabases() = 0;
    virtual QStringList getTables(QString dataBase) = 0;
    void setDataBase(const QString &driver,const QString &conectionName, const QString &host,
                     const QString &user,const QString &password,const QString &dataBase,const int &port);
    static SqlManager * fromDriverName(const QString &driver);
    bool insertBatch(const QString &sql, const QList<QVariant> &valueList);
    QStringList executeQuery(const QString &sql);
    bool open();
    bool isOpen() const;
signals:

public slots:

protected:
    class SqlManagerPrivate;
    SqlManagerPrivate *d;
};

#endif // DATABASE_H
