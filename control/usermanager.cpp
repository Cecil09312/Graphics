#include "usermanager.h"
#include <QCoreApplication>
#include <QDebug>
UserManager::UserManager(QObject *parent)
    : QObject(parent)

{
#ifdef Q_OS_LINUX
    m_userRight=UserManager::User;
    m_userName="default";
#elif defined (Q_OS_WIN)
    m_userRight=UserManager::Super;
    m_userName="super";
#endif
    m_sqliteManager = new SqlManager;
    if(m_sqliteManager!=nullptr)
    {
        QString dbName = QCoreApplication::applicationDirPath()+"/userSetting.db";
        m_sqliteManager->setDataBase("QSQLITE","userRight","","","",dbName,1000);
        m_sqliteManager->open();
        if(m_sqliteManager->isOpen())
        {
            QStringList tableNameList = m_sqliteManager->getTables();
            if(tableNameList.isEmpty())
            {
                m_sqliteManager->executeQuery("create table UserInfo(userRight text not null, userName text primary key not null,password text not null);");
                m_sqliteManager->executeQuery("insert into UserInfo values('普通用户','119','119')");
                m_sqliteManager->executeQuery("insert into UserInfo values('超级用户','super','system')");
                m_sqliteManager->executeQuery("insert into UserInfo values('管理员','admin','admin')");
            }
            else
            {
               // QStringList valueList=  m_sqliteManager->executeQuery("select password from UserInfo where userRight='普通用户'");
//                if(valueList.size()>0)
//                {
//                    m_password = valueList.at(0);
//                }
            }
        }

    }
}

QString UserManager::password(const UserManager::UserRight &right,const QString &userName)
{
    QString customPassword ="";
    if(m_sqliteManager!=nullptr)
    {
        QStringList valueList;

        switch (right)
        {
        case UserManager::Super:
            valueList= m_sqliteManager->executeQuery(QString("select password from UserInfo where userRight='超级用户' and userName = '%1'").arg(userName));
            break;
        case UserManager::Administrator:
            valueList= m_sqliteManager->executeQuery(QString("select password from UserInfo where userRight='管理员' and userName = '%1'").arg(userName));
            break;
        case UserManager::User:
            valueList= m_sqliteManager->executeQuery(QString("select password from UserInfo where userRight='普通用户' and userName = '%1'").arg(userName));
            break;
//        default:
//            break;
        }
        if(valueList.size()>0)
        {
            customPassword = valueList.at(0);
        }
    }
    return customPassword;
}

void UserManager::setPassword(const UserManager::UserRight &right, const QString &userPassword,const QString &userName)
{
    if(m_sqliteManager!=nullptr)
    {
        switch (right)
        {
        case UserManager::Super:
            m_sqliteManager->executeQuery(QString("update UserInfo set password = '%1' where userRight = '超级用户' and userName = '%2'").arg(userPassword).arg(userName));
            break;
        case UserManager::Administrator:
            m_sqliteManager->executeQuery(QString("update UserInfo set password = '%1' where userRight = '管理员' and userName = '%2'").arg(userPassword).arg(userName));
            break;
        case UserManager::User:
            m_sqliteManager->executeQuery(QString("update UserInfo set password = '%1' where userRight = '普通用户' and userName = '%2'").arg(userPassword).arg(userName));
            break;

//        default:
//            break;
        }
    }
}


QString UserManager::userName()
{
    return m_userName;
}

void UserManager::setUserName(const QString &name)
{
    m_userName = name;
    emit userNameChanged(name);
}

UserManager::UserRight UserManager::userRight()
{
    return m_userRight;
}

void UserManager::setUserRight(const UserManager::UserRight &right)
{
    m_userRight = right;
    emit userRightChanged(right);
}

void UserManager::addUser(const QString &userName, const UserManager::UserRight &right, const QString &password)
{
    if(m_sqliteManager!=nullptr)
    {
        switch (right)
        {
        case UserManager::Super:

            break;
        case UserManager::Administrator:
            m_sqliteManager->executeQuery(QString("insert into UserInfo values ('管理员','%1','%2')").arg(userName).arg(password));
            break;
        case UserManager::User:
            m_sqliteManager->executeQuery(QString("insert into UserInfo values ('普通用户','%1','%2')").arg(userName).arg(password));
            break;
//        default:
//            break;
        }
    }
}

void UserManager::removeUser(const QString &userName, const UserManager::UserRight &right, const QString &password)
{
    if(m_sqliteManager!=nullptr)
    {
        switch (right)
        {
        case UserManager::Super:

            break;
        case UserManager::Administrator:
            m_sqliteManager->executeQuery(QString("delete from UserInfo where userRight = '管理员' and userName='%1' and password='%2'").arg(userName).arg(password));
            break;
        case UserManager::User:
            m_sqliteManager->executeQuery(QString("delete from UserInfo where userRight = '普通用户' and userName='%1' and password='%2'").arg(userName).arg(password));
            break;
//        default:
//            break;
        }
    }
}

bool UserManager::userIsExist(const QString &userName, const UserManager::UserRight &right)
{
    //QString customPassword ="";
    QStringList valueList;
    if(m_sqliteManager!=nullptr)
    {

        switch (right)
        {
        case UserManager::Super:
            valueList= m_sqliteManager->executeQuery(QString("select password from UserInfo where userRight='超级用户' and userName = '%1'").arg(userName));
            break;
        case UserManager::Administrator:
            valueList= m_sqliteManager->executeQuery(QString("select password from UserInfo where userRight='管理员' and userName = '%1'").arg(userName));
            break;
        case UserManager::User:
            valueList= m_sqliteManager->executeQuery(QString("select password from UserInfo where userRight='普通用户' and userName = '%1'").arg(userName));
            break;
//        default:
//            break;
        }
//        if(valueList.size()>0)
//        {
//            customPassword = valueList.at(0);
//        }
    }
    return !valueList.isEmpty();
    // return customPassword;
}

void UserManager::deleteUsers(const QString &userName)
{
    m_sqliteManager->executeQuery(QString("delete from UserInfo where userRight != '超级用户' and userName='%1'").arg(userName));
}

UserManager::~UserManager()
{
    if(m_sqliteManager!=nullptr)
    {
        m_sqliteManager->close();
        m_sqliteManager->deleteLater();
       // m_sqliteManager = nullptr;
    }
}



QVariantList UserManager::selectUsers()
{
    return  m_sqliteManager->queryFromSql(QString("select userName from UserInfo where userRight!='超级用户'"));
}






