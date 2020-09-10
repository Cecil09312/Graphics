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
        Administrator,
        User
    };
    explicit UserManager(QObject *parent = nullptr);
    Q_INVOKABLE  QString password(const UserRight& right, const QString &userName);
    Q_INVOKABLE  void  setPassword(const UserRight& right, const QString &userPassword, const QString &userName);
    Q_INVOKABLE  QString userName();
    Q_INVOKABLE  void setUserName(const QString &name);
    Q_INVOKABLE  UserRight userRight();
    Q_INVOKABLE  void setUserRight(const UserRight& right);
    Q_INVOKABLE  void addUser(const QString &userName,const UserRight& right,const QString &password);
    Q_INVOKABLE  void removeUser(const QString &userName,const UserRight& right,const QString &password);
    Q_INVOKABLE  bool userIsExist(const QString &userName,const UserRight& right);
    Q_INVOKABLE  void deleteUsers(const QString &userName);
    ~UserManager();
    Q_INVOKABLE QVariantList selectUsers();

signals:
    void userNameChanged(const QString &userName);
    void userRightChanged(const UserRight& right);

public slots:
private:

private:
    UserRight m_userRight;
    QString m_userName;
    QString m_password;
    SqlManager *m_sqliteManager;
   // QStringList m_getUserList;
    //int m_userNum;
};

#endif // USERMANAGER_H
