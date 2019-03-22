#include "operatorinfo.h"
#include <QCoreApplication>
#include "control/usermanager.h"
#include "control/controller.h"
OperatorInfo::OperatorInfo(QObject *parent) : QObject(parent)
{
    m_sqlManager = SqlManager::fromDriver("QSQLITE");
    m_operatorInfoDbPath = QCoreApplication::applicationDirPath()+"/operatorInfo.db";
    m_sqlManager->setDataBase("QSQLITE","operatorInfo", "",
                              "","",m_operatorInfoDbPath,6666);
    m_sqlManager->open();
    if(m_sqlManager->isOpen())
    {
        QStringList tableList = m_sqlManager->getTables(m_operatorInfoDbPath);
        if(!tableList.contains("operator"))
        {
            m_sqlManager->executeQuery("create table operator(用户名 text,用户权限 text,事件 text,时间 text)");
        }

        if(!tableList.contains("maintenance"))
        {
            m_sqlManager->executeQuery("create table operator(设备编码 text,维保时间 text,维保说明 text,状态现象 text,解决方法 text,内容描述 text,维保员 text ,楼层 text ,部位 text,系统 text ,建筑名称 text,值班人员 text ,操作人员 text)");
        }
    }
}

OperatorInfo::~OperatorInfo()
{
    m_sqlManager->close();
    m_sqlManager->deleteLater();
}

QString OperatorInfo::operatorInfoDbPath()
{
    return m_operatorInfoDbPath;
}

void OperatorInfo::insertEvent(const QString &eventName)
{
    QString userName=  Controller::instance()->getUserManager()->userName();
    UserManager::UserRight userRight = Controller::instance()->getUserRight();
    QString userRightStr;
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    switch (userRight) {
    case UserManager::Employee:
        userRightStr = "Employee";
        break;
    case UserManager::Engineer:
        userRightStr = "Engineer";
        break;
    case UserManager::Super:
        userRightStr = "Super";
        break;
    default:
        break;
    }
    m_sqlManager->executeQuery(QString("insert into operator values ('%1','%2','%3','%4')").arg(userName).arg(userRightStr).arg(eventName).arg(currentTime));
}
