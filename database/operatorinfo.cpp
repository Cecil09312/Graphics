#include "operatorinfo.h"
#include <QCoreApplication>
#include "control/usermanager.h"
#include "control/controller.h"

OperatorInfo::OperatorInfo(QObject *parent)
    : QObject(parent)
{
    m_sqlManager = new SqliteManager;
    m_operatorInfoDbPath = QCoreApplication::applicationDirPath()+"/operatorInfo.db";
    m_sqlManager->setDataBase("QSQLITE","operatorInfo", "",
                              "","",m_operatorInfoDbPath,6666);
    m_sqlManager->open();
    if(m_sqlManager->isOpen())
    {
        QDateTime curDateTime = QDateTime::currentDateTime();
        curDateTime = curDateTime.addYears(-3);
        QString curDateTimeStr = curDateTime.toString("yyyy/MM/dd hh:mm:ss");
        QStringList maintenanceList;
        maintenanceList << "设备编码 text primary key not null"<<"维保时间 text not null"<<"状态现象 text"<<"维保方法 text"<<"内容描述 text"<<"维保员 text"<<"楼层 text" <<"部位 text"<<"系统 text" <<"建筑名称 text"<<"值班人员 text" <<"操作人员 text";
        m_maintInfoTableSize= maintenanceList.size();
        QStringList tableList = m_sqlManager->getTables();
        if(!tableList.contains("operator"))
        {
            m_sqlManager->executeQuery("create table operator(用户名 text,用户权限 text,事件 text,结果 text,时间 text)");
        }
        else
        {
            m_sqlManager->executeQuery(QString("delete from operator where 时间 <='%1'").arg(curDateTimeStr));

        }

        if(!tableList.contains("maintenance"))
        {
            m_sqlManager->executeQuery(QString("create table maintenance(%1)").arg(maintenanceList.join(",")));
        }
        else
        {
            m_sqlManager->executeQuery(QString("delete from maintenance where 维保时间<='%1'").arg(curDateTimeStr));
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

void OperatorInfo::saveMaintInfo()
{
    int hashSize = m_maintInfoValueHash.size();
    if(hashSize>=m_maintInfoTableSize && hashSize%m_maintInfoTableSize==0)
    {
        QStringList keyList=  QStringList(m_maintInfoValueHash.keys()) ;
        QStringList valueList = QStringList(m_maintInfoValueHash.values());
        QString queryStr = QString("insert into maintenance (%1) values (").arg(keyList.join(","));
        for(int i=0;i<m_maintInfoTableSize;i++)
        {
            queryStr+="'";
            queryStr+=valueList.at(i);
            queryStr+="'";
            if(i<m_maintInfoTableSize-1)
            {
                queryStr+=",";
            }
        }
        queryStr+=")";
        m_sqlManager->executeQuery(queryStr);
    }
}

void OperatorInfo::setMaintInfoValue(const QString &key, const QString &value)
{
    m_maintInfoValueHash[key] = value;
}

void OperatorInfo::insertEvent(const QString &eventName,const QString &result)
{
    QString userName=  Controller::instance()->getUserManager()->userName();
    UserManager::UserRight userRight = Controller::instance()->getUserRight();
    QString userRightStr;
    QString currentTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    switch (userRight)
    {
    case UserManager::User:
        userRightStr = "普通用户";
        break;
    case UserManager::Administrator:
        userRightStr = "管理员";
        break;
    case UserManager::Super:
        userRightStr = "超级用户";
        break;

//    default:

//        break;
    }
    m_sqlManager->executeQuery(QString("insert into operator values ('%1','%2','%3','%4','%5')").arg(userName).arg(userRightStr).arg(eventName).arg(result).arg(currentTime));
}
