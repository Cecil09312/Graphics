#include "jsonedit.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QFile>
#include <QMutex>
#include<QMutexLocker>
#include <QFuture>
#include <QtConcurrent>

JsonEdit *JsonEdit:: m_jsonEdit = nullptr;
JsonEdit::AutoDelete autoDelete;
JsonEdit::JsonEdit(QObject *parent) : QObject(parent)
{
    m_rootList = QList<QVariant>();
}

JsonEdit *JsonEdit::instance()
{
    if(m_jsonEdit==nullptr)
    {
        QMutex mutex;
        mutex.lock();
        if(m_jsonEdit==nullptr)
        {
            JsonEdit *jsonEdit = new JsonEdit;
            m_jsonEdit = jsonEdit;
        }
        mutex.unlock();
    }

     return m_jsonEdit;
}

void JsonEdit::insertRoot(const QVariant &root)
{
    m_rootList.push_back(root);
}



void JsonEdit::setRootValue(int parentIndex, const QString &name, const QVariant &value)
{
    if(m_rootList.size()>parentIndex)
    {
        QHash<QString,QVariant> parentHash = m_rootList.at(parentIndex).toHash();
        parentHash[name] = value;
        m_rootList.replace(parentIndex,parentHash);
    }
}

void JsonEdit::setRootName(int parentIndex, const QString &name)
{
    setRootValue(parentIndex,"name",name);
}

void JsonEdit::setRootImage(int parentIndex, const QVariant &value)
{
    setRootValue(parentIndex,"image",value);
}

void JsonEdit::insertChild(int parentIndex, const QVariant &child)
{
    if(m_rootList.size()>parentIndex)
    {
        QHash<QString,QVariant> parentHash = m_rootList.at(parentIndex).toHash();
        QList<QVariant>childList=parentHash["child"].toList();
        childList.push_back(child);
        parentHash["child"] = childList;
        m_rootList.replace(parentIndex,parentHash);
    }

}

void JsonEdit::setChildName(int parentIndex, int childIndex, const QString &name)
{
    setChildValue(parentIndex, childIndex,"name",name);
}


void JsonEdit::setChildImage(int parentIndex, int childIndex,const QVariant &value)
{
    setChildValue(parentIndex, childIndex,"image",value);
}

void JsonEdit::setChildValue(int parentIndex, int childIndex, const QString &name, const QVariant &value)
{
    if(m_rootList.size()>parentIndex)
    {
        QHash<QString,QVariant> parentHash= m_rootList.at(parentIndex).toHash();
        QList<QVariant>childList=parentHash["child"].toList();
        if(childList.size()>childIndex)
        {
            QHash<QString,QVariant> childHash=  childList.at(childIndex).toHash();
            childHash[name] = value;
            childList.replace(childIndex,childHash);
            parentHash["child"] = childList;
            m_rootList.replace(parentIndex,parentHash);
        }
    }
}

QVariant JsonEdit::getChild(int parentIndex)
{
    if(m_rootList.size()>parentIndex)
    {
        return m_rootList.at(parentIndex);
    }
    else
    {
        return QVariant();
    }
}

QList<QVariant>JsonEdit::getRoot() const
{
    return m_rootList;
}

void JsonEdit::writeFile(const QString &fileName)
{
    QmlForJson qmlForJson;
    qmlForJson.writeFile(getRoot(),fileName);
    m_rootList.clear();
}

QVariant JsonEdit::readFile(const QString &fileName)
{
    QmlForJson qmlForJson;
    return qmlForJson.readFile(fileName);
}

void JsonEdit::fileClear()
{
    m_rootList.clear();
}


