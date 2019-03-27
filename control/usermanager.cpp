#include "usermanager.h"
#include <QCoreApplication>
#include <QDebug>
UserManager::UserManager(QObject *parent)
    : QObject(parent),
      m_userRight(UserManager::Employee),
      m_userName(tr("employee"))
{
    m_sqliteManager = SqlManager::fromDriver("SQLITE");
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
                m_sqliteManager->executeQuery("insert into UserInfo values('Employee','employee','1234')");
                m_sqliteManager->executeQuery("insert into UserInfo values('Super','super','super')");
            }
            else
            {
                QStringList valueList=  m_sqliteManager->executeQuery("select password from UserInfo where userRight='Employee' and userName='employee'");
                if(valueList.size()>0)
                {
                    m_password = valueList.at(0);
                }
            }
        }

    }
}

QString UserManager::password(const UserManager::UserRight &right,const QString &userName)
{
    if(m_sqliteManager!=nullptr)
    {
        QStringList valueList;

        switch (right)
        {
        case UserManager::Super:
            valueList= m_sqliteManager->executeQuery(QString("select password from UserInfo where userRight='Super' and userName = '%1'").arg(userName));
            break;
        case UserManager::Engineer:
            valueList= m_sqliteManager->executeQuery(QString("select password from UserInfo where userRight='Engineer' and userName = '%1'").arg(userName));
            break;
        case UserManager::Employee:
            valueList= m_sqliteManager->executeQuery(QString("select password from UserInfo where userRight='Employee' and userName = '%1'").arg(userName));
            break;
        default:
            break;
        }
        if(valueList.size()>0)
        {
            m_password = valueList.at(0);
        }
    }
    return m_password;
}

void UserManager::setPassword(const UserManager::UserRight &right, const QString &userPassword,const QString &userName)
{
    if(m_sqliteManager!=nullptr)
    {
        switch (right)
        {
        case UserManager::Super:
            m_sqliteManager->executeQuery(QString("update UserInfo set password = '%1' where userRight = 'Super' and userName = '%2'").arg(userPassword).arg(userName));
            break;
        case UserManager::Engineer:
            m_sqliteManager->executeQuery(QString("update UserInfo set password = '%1' where userRight = 'Engineer' and userName = '%2'").arg(userPassword).arg(userName));
            break;
        case UserManager::Employee:
            m_sqliteManager->executeQuery(QString("update UserInfo set password = '%1' where userRight = 'Employee' and userName = '%2'").arg(userPassword).arg(userName));
            break;
        default:
            break;
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
        case UserManager::Engineer:
            m_sqliteManager->executeQuery(QString("insert into UserInfo values ('Engineer','%1','%2')").arg(userName).arg(password));
            break;
        case UserManager::Employee:
            m_sqliteManager->executeQuery(QString("insert into UserInfo values ('Employee','%1','%2')").arg(userName).arg(password));
            break;
        default:
            break;
        }
    }
}

UserManager::~UserManager()
{
    if(m_sqliteManager!=nullptr)
    {
        m_sqliteManager->close();
        m_sqliteManager->deleteLater();
        m_sqliteManager = nullptr;
    }
}




