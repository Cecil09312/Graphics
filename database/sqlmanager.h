#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QtConcurrent>
#include <QFuture>

class SqlManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName)
    Q_PROPERTY(QString userName READ userName WRITE setUserName)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(QString databaseName READ databaseName WRITE setDatabaseName)
    Q_PROPERTY(int port READ port WRITE setPort)
    Q_PROPERTY(QString connectionName READ connectionName)
public:
    explicit SqlManager(QObject *parent = nullptr);
    virtual ~SqlManager();
    virtual QStringList getDatabases();
    virtual void quitThread();
    QStringList getTables();
    Q_INVOKABLE  void setDataBase(const QString &driver, const QString &connectionName, const QString &host,
                                  const QString &user, const QString &password, const QString &dataBase, const int &port);
    bool insertBatch(const QString &tableName, const QList<QVariant> &valueList);
    virtual QStringList executeQuery(const QString &sql);
    static SqlManager *fromDriver(const QString &driver);
    Q_INVOKABLE bool open();
    Q_INVOKABLE bool isOpen() const;
    Q_INVOKABLE void close();
    Q_INVOKABLE bool tableIsExist(const QString &tableName);
    QSqlDatabase &getDatabase();
    virtual int tableColumns(const QString &tableName);
    QString dbName() const;
    QString hostName();
    void setHostName(const QString& host);
    QString userName();
    void setUserName(const QString& user);
    QString password();
    void setPassword(const QString &passwordStr);
    QString databaseName();
    void setDatabaseName(const QString &name);
    int port();
    void setPort(int p);
    QString connectionName();
    //QList<QString>sqlStrInList(const QString &sql);
    void processDb(QList<QString> &sqlList);
    QSqlDatabase &getDb();

signals:
   void dataCommitSuccess(bool isSuccessful);
   void dbConnected(bool isConnected);

public slots:

protected:
    class SqlManagerPrivate;
    SqlManagerPrivate *d;
    //QSqlQuery *m_query;


};

#endif // DATABASE_H
