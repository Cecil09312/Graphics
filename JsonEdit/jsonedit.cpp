#include "jsonedit.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QFile>
#include <QMutex>
#include<QMutexLocker>

JsonEdit *JsonEdit:: m_jsonEdit = nullptr;
JsonEdit::AutoDelete autoDelete;
JsonEdit::JsonEdit(QObject *parent) : QObject(parent)
{
    // m_childMap["child"] = "";

}

JsonEdit *JsonEdit::instance()
{
    if(m_jsonEdit==nullptr)
    {
        QMutex mutex;
        QMutexLocker mutexLocker(&mutex);
        if(m_jsonEdit==nullptr)
        {
            JsonEdit *jsonEdit = new JsonEdit;
            m_jsonEdit = jsonEdit;

        }
        return m_jsonEdit;
    }
}

void JsonEdit::insertRoot(const QVariant &root)
{
    m_rootList.push_back(root);
}



void JsonEdit::setRootValue(int parentIndex, const QString &name, const QVariant &value)
{
    if(m_rootList.size()>parentIndex)
    {
        QMap<QString,QVariant> parentMap = m_rootList.at(parentIndex).toMap();
        parentMap[name] = value;
        m_rootList.replace(parentIndex,parentMap);
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
        QMap<QString,QVariant> parentMap = m_rootList.at(parentIndex).toMap();
        QList<QVariant>childList=parentMap["child"].toList();
        childList.push_back(child);
        parentMap["child"] = childList;
        m_rootList.replace(parentIndex,parentMap);
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
        QMap<QString,QVariant> parentMap = m_rootList.at(parentIndex).toMap();
        QList<QVariant>childList=parentMap["child"].toList();
        if(childList.size()>childIndex)
        {
            QMap<QString,QVariant> childMap=  childList.at(childIndex).toMap();
            childMap[name] = value;
            childList.replace(childIndex,childMap);
            parentMap["child"] = childList;
            m_rootList.replace(parentIndex,parentMap);
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

QList<QVariant> JsonEdit::getRoot() const
{
    return m_rootList;
}

void JsonEdit::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
   {
       QJsonDocument jsonDoc= QJsonDocument::fromVariant(getRoot());
       file.write(jsonDoc.toJson());
    }
    file.close();
}

QVariant JsonEdit::readFile(const QString &fileName)
{
    QFile file(fileName);
    QVariant variant;
    if(file.open(QIODevice::ReadOnly))
   {
       QByteArray byteArray = file.readAll();
       QJsonDocument jsonDoc= QJsonDocument::fromJson(byteArray);
       variant = jsonDoc.toVariant();
      // qDebug() << variant;
    }
    file.close();
    return variant;
}

void JsonEdit::fileClear()
{
    m_rootList.clear();
}


