#include "qmltablemodel.h"
#include <QDebug>
#include <QSqlRecord>
#include <QStringList>
#include <QDebug>

QmlTableModel::QmlTableModel(QObject *parent)
    : QSqlQueryModel(parent),
      m_sqlManager(nullptr)
{
    m_print = new Print(this);
}

QmlTableModel::~QmlTableModel()
{
    if(m_sqlManager!=nullptr)
    {
        m_sqlManager->close();
        m_sqlManager->deleteLater();
        m_sqlManager = nullptr;
    }

}


QVariant QmlTableModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole)
    {
        return QSqlQueryModel::data(index, role);
    }
    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

QHash<int, QByteArray> QmlTableModel::roleNames() const
{
    return m_roleHash;
}

void QmlTableModel::sqlCommit(const QString &sqlStr)
{
    if(m_sqlManager!=nullptr)
    {
        setQuery(sqlStr,m_sqlManager->getDatabase());
    }
    submit();
}


QList<QString> QmlTableModel::roleNameList()
{
    return m_roleNameList;
}

void QmlTableModel::setRoleNameList(const QList<QString> &roleNamesList)
{
    m_roleNameList = roleNamesList;
    for(int i=0;i<m_roleNameList.size();i++)
    {
        m_roleHash[Qt::UserRole+i] = m_roleNameList.at(i).toLocal8Bit();
    }
}

void QmlTableModel::setTitleList(const QList<QString> &titleList)
{
    m_titleList = titleList;
}

QList<QString> QmlTableModel::titleList()
{
    return m_titleList;
}


QString &QmlTableModel::dbDriver()
{
    return m_dbDriver;
}

void QmlTableModel::setDbDriver(const QString &driver)
{

    m_dbDriver = driver;

    if(m_sqlManager==nullptr)
    {
        if(m_dbDriver.contains("SQLITE",Qt::CaseInsensitive))
        {
           m_sqlManager = new SqliteManager;
        }
    }
}

QString &QmlTableModel::dbConnectionName()
{
    return m_dbConnectionName;
}

void QmlTableModel::setDbConnectionName(const QString &name)
{

    m_dbConnectionName= name;

}

QString &QmlTableModel::dbHost()
{
    return m_dbHost;
}

void QmlTableModel::setDbHost(const QString &host)
{
    m_dbHost = host;
}

QString& QmlTableModel::dbUser()
{
    return m_dbUser;
}

void QmlTableModel::setDbUser(const QString &user)
{
    m_dbUser = user;
}

QString &QmlTableModel::dbPassword()
{
    return m_dbPassword;
}

void QmlTableModel::setDbPassword(const QString &password)
{
    m_dbPassword = password;
}

QString &QmlTableModel::dbName()
{
    return m_dbName;
}

void QmlTableModel::setDbName(const QString &name)
{
    m_dbName = name;
}

int QmlTableModel::dbPort()
{
    return m_dbPort;
}

void QmlTableModel::setDbPort(int port)
{
    m_dbPort = port;
}

bool QmlTableModel::dbOpen()
{
    if(m_sqlManager!=nullptr)
    {
        return m_sqlManager->isOpen();
    }

    else
    {
        return false;
    }
}

void QmlTableModel::setDbOpen(bool isOpen)
{
    if(m_sqlManager!=nullptr)
    {
        m_sqlManager->setDataBase(m_dbDriver,m_dbConnectionName,m_dbHost,m_dbUser,m_dbPassword,m_dbName,m_dbPort);
        if(isOpen)
        {
            m_sqlManager->open();
        }
        else
        {
            m_sqlManager->close();
        }
    }
}

void QmlTableModel::saveToPdf()
{

    m_print->saveToPdf(titleList(),getValues());
}

void QmlTableModel::startPrint()
{
    m_print->startPrint(titleList(),getValues());
}

void QmlTableModel::printPreview()
{
    m_print->printPreview(titleList(),getValues());
}

QList<QVariant> QmlTableModel::getValues()
{
    QList<QVariant>valueList;
    for(int i=0;i<rowCount();i++)
    {
        QSqlRecord sqlRecord = record(i);
        for(int j=0;j<sqlRecord.count();j++)
        {
            valueList.push_back(sqlRecord.value(j));
        }
    }
    return valueList;
}

QString QmlTableModel::getValue(int row, const QString &roleName)
{
    if(row<rowCount())
    {
        QSqlRecord sqlRecord = record(row);
        int index =m_roleHash.key(roleName.toLocal8Bit())-Qt::UserRole;
        if(index <sqlRecord.count())
        {
            return sqlRecord.value(index).toString();
        }
        else
        {
            return QString();
        }
    }
    else
    {
        return QString();
    }

}




