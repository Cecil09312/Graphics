#ifndef QMLTABLEMODEL_H
#define QMLTABLEMODEL_H

#include <QSqlTableModel>
#include <QQmlParserStatus>
#include "database/sqlitemanager.h"
#include "database/sqlmanager.h"
#include "print/print.h"

class QmlTableModel : public QSqlQueryModel
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> roleNameList READ roleNameList WRITE setRoleNameList)
    Q_PROPERTY(QString dbDriver READ dbDriver WRITE setDbDriver)
    Q_PROPERTY(QString dbConnectionName READ dbConnectionName WRITE setDbConnectionName)
    Q_PROPERTY(QString dbHost READ dbHost WRITE setDbHost)
    Q_PROPERTY(QString dbUser READ dbUser WRITE setDbUser)
    Q_PROPERTY(QString dbPassword READ dbPassword WRITE setDbPassword)
    Q_PROPERTY(QString dbName READ dbName WRITE setDbName)
    Q_PROPERTY(int dbPort READ dbPort WRITE setDbPort)
    Q_PROPERTY(QList<QString> titleList READ titleList WRITE setTitleList)

public:
    explicit QmlTableModel(QObject *parent = nullptr);
    ~QmlTableModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    Q_INVOKABLE void sqlCommit(const QString &sqlStr);
    QList<QString> roleNameList();
    void setRoleNameList(const QList <QString> &roleNamesList);
    void setTitleList(const QList <QString> &titleList);
    QList<QString> titleList();
    QString &dbDriver();
    void setDbDriver(const QString &driver);
    QString &dbConnectionName();
    void setDbConnectionName(const QString&name);
    QString &dbHost();
    void setDbHost(const QString&host);
    QString& dbUser();
    void setDbUser(const QString&user);
    QString& dbPassword();
    void setDbPassword(const QString&password);
    QString& dbName();
    void setDbName(const QString &name);
    int dbPort();
    void setDbPort(int port);
    Q_INVOKABLE bool dbOpen();
    Q_INVOKABLE void setDbOpen(bool isOpen);
    Q_INVOKABLE void saveToPdf();
    Q_INVOKABLE void startPrint();
    Q_INVOKABLE void printPreview();
    QList<QVariant>getValues();
    Q_INVOKABLE QString getValue(int row,const QString&roleName);
private:
    QHash<int, QByteArray>m_roleHash;
    QList<QString> m_roleNameList;
    QList<QString> m_titleList;
    QString m_sqlTable;
    SqlManager *m_sqlManager;

    QString m_dbDriver;
    QString m_dbConnectionName;
    QString m_dbHost;
    QString m_dbUser;
    QString m_dbPassword;
    QString m_dbName;
    int m_dbPort;
    bool m_dbOpen;
    Print *m_print;

};

#endif // QMLTABLEMODEL_H
