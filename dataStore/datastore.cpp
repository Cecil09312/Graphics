#include "datastore.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "architePlan/architeplanview.h"
QHash<QString,QList<QGraphicsItem*> >DataStore::m_typeItemHash=QHash<QString,QList<QGraphicsItem*> >();
DataStore::DataStore()
{

}

DataStore::~DataStore()
{

}

QHash<QString, QList<QGraphicsItem *> > &DataStore::getTypeItemHash()
{
    return m_typeItemHash;
}

QList<QGraphicsItem *> &DataStore::getTypeItemList(const QString &type)
{
    return m_typeItemHash[type];
}

QGraphicsItem *DataStore::getTypeItem(const QString &type, int pos)
{
    QList<QGraphicsItem *> itemList = m_typeItemHash[type];
    if(itemList.size()>pos)
    {
        return itemList.at(pos);
    }
    else
    {
        return nullptr;
    }
}

void DataStore::deleteTypeItems(const QString &type)
{
    m_typeItemHash.remove(type);
}

void DataStore::deleteTypeItem(QGraphicsItem *item)
{
    QList<QString> keyList= m_typeItemHash.keys();
    foreach (const QString &type, keyList)
    {
        QList<QGraphicsItem *> itemList=  m_typeItemHash[type];
        foreach (QGraphicsItem *currentItem, itemList)
        {
            if(currentItem==item)
            {
                m_typeItemHash[type].removeOne(item);
                return;
            }
        }
    }
}

void DataStore::deleteTypeItem(const QString &type, QGraphicsItem *item)
{
    m_typeItemHash[type].removeOne(item);
}

void DataStore::deleteTypeItem(const QString &type, int pos)
{
    if(m_typeItemHash[type].size()>pos)
        m_typeItemHash[type].removeAt(pos);
}

void DataStore::insertTypeItem(const QString &type, QGraphicsItem *item)
{
    m_typeItemHash[type].push_back(item);
}

void DataStore::clearTypeItem()
{
    m_typeItemHash.clear();
}

int DataStore::numOfTypeItem(const QString &type)
{
    return m_typeItemHash[type].size();
}

GraphicsView *DataStore::itemDisplayView(GraphicsItem *item)
{
    GraphicsView *currentView = nullptr;
    QList<GraphicsView*> viewList= ArchitePlanView::getWidgetMap().values();
    foreach (GraphicsView*view, viewList)
    {
        if(view!=nullptr)
        {
            QList<QGraphicsItem *>itemList=  view->getItemList();
            if(itemList.contains(item))
            {
                currentView = view;
                break;
            }
        }
    }
    return currentView;
}
