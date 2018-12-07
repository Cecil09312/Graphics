#include "usermanager.h"
#include <QCoreApplication>
#include <QDebug>
UserManager::UserManager(QObject *parent)
    : QObject(parent),
    m_userRight(UserManager::Employee),
    m_userName(tr("员工"))
{
    m_sqliteManager = SqlManager::fromDriver("SQLITE");

    if(m_sqliteManager!=nullptr)
    {
        QString dbName = QCoreApplication::applicationDirPath()+"/userSetting.db";
        m_sqliteManager->setDataBase("QSQLITE","userRight","","","",dbName,1000);
        m_sqliteManager->open();
        if(m_sqliteManager->isOpen())
        {
            QStringList tableNameList = m_sqliteManager->getTables(dbName);
            if(tableNameList.isEmpty())
            {
                m_sqliteManager->executeQuery("create table UserInfo(userRight text primary key not null, userName text not null,password text not null);");
                m_sqliteManager->executeQuery("insert into UserInfo values('Employee','员工','1234')");
                m_sqliteManager->executeQuery("insert into UserInfo values('Enginee','工程人员','enginee')");
                m_sqliteManager->executeQuery("insert into UserInfo values('Super','超级用户','super')");
            }
            else
            {
                QStringList valueList=  m_sqliteManager->executeQuery("select password from UserInfo where userRight='Employee'");
                if(valueList.size()>0)
                {
                    m_password = valueList.at(0);

                }
            }
        }

    }
}

 QString UserManager::password(const UserManager::UserRight &right)
{
    if(m_sqliteManager!=nullptr)
    {
        QStringList valueList;

        switch (right)
        {
        case UserManager::Super:
            valueList= m_sqliteManager->executeQuery("select password from UserInfo where userRight='Super'");
            break;
        case UserManager::Engineer:
            valueList= m_sqliteManager->executeQuery("select password from UserInfo where userRight='Enginee'");
            break;
        case UserManager::Employee:
            valueList= m_sqliteManager->executeQuery("select password from UserInfo where userRight='Employee'");
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

void UserManager::setPassword(const UserManager::UserRight &right, const QString &userPassword)
{
    if(m_sqliteManager!=nullptr)
    {
        switch (right)
        {
        case UserManager::Super:
            m_sqliteManager->executeQuery(QString("update UserInfo set password = '%1' where userRight = 'Super'").arg(userPassword));
            break;
        case UserManager::Engineer:
            m_sqliteManager->executeQuery(QString("update UserInfo set password = '%1' where userRight = 'Enginee'").arg(userPassword));
            break;
        case UserManager::Employee:
            m_sqliteManager->executeQuery(QString("update UserInfo set password = '%1' where userRight = 'Employee'").arg(userPassword));
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

void UserManager::setUserName(const UserManager::UserRight &right, const QString &name)
{
    if(m_sqliteManager!=nullptr)
    {
        switch (right)
        {
        case UserManager::Super:
            m_sqliteManager->executeQuery(QString("update UserInfo set userName = '%1' where userRight = 'Super'").arg(name));
            break;
        case UserManager::Engineer:
            m_sqliteManager->executeQuery(QString("update UserInfo set userName = '%1' where userRight = 'Enginee'").arg(name));
            break;
        case UserManager::Employee:
            m_sqliteManager->executeQuery(QString("update UserInfo set userName = '%1' where userRight = 'Employee'").arg(name));
            break;
        default:
            break;
        }
    }

}

UserManager::UserRight UserManager::userRight()
{
    return m_userRight;
}

void UserManager::setUserRight(const UserManager::UserRight &right)
{
    m_userRight = right;
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




