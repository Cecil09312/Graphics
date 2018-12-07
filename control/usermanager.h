#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include "database/sqlitemanager.h"
#include "database/sqlmanager.h"
class UserManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(UserRight)
public:
    enum UserRight
    {
        Super =1,
        Engineer,
        Employee
    };
    explicit UserManager(QObject *parent = nullptr);
    Q_INVOKABLE  QString password(const UserRight& right);
    Q_INVOKABLE  void  setPassword(const UserRight& right,const QString &userPassword);
    Q_INVOKABLE  QString userName();
    Q_INVOKABLE  void setUserName(const UserRight& right,const QString &name);
    Q_INVOKABLE  UserRight userRight();
    Q_INVOKABLE  void setUserRight(const UserRight& right);
    ~UserManager();

signals:

public slots:
private:

private:
    UserRight m_userRight;
    QString m_userName;
    QString m_password;
    SqlManager *m_sqliteManager;
};

#endif // USERMANAGER_H
