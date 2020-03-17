#include "datastore.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "architePlan/architeplanview.h"
QHash<QString,QList<QGraphicsItem*> >DataStore::m_typeItemHash=QHash<QString,QList<QGraphicsItem*> >();
QHash<QString,QList<QString> >DataStore::m_typeNoItemHash = QHash<QString,QList<QString> >();
int DataStore::s_itemNum=0;
QString DataStore::m_loopNum="0";
QString DataStore::m_extNum="0";
QString DataStore::m_networkNum="0";
QString DataStore::m_powerAddr ="0";
QString DataStore:: m_sysName = "";
int DataStore::m_channelNum=0;
QString DataStore::m_analogValue="无";
qreal DataStore::m_iconSize=25;
QString DataStore::m_operator="";
int DataStore::s_itemNumTemp =0;
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

QList<QGraphicsItem *> DataStore::getTypeItemList(const QString &type)
{
    return m_typeItemHash.value(type);
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
                if(m_typeItemHash.value(type).contains(item))
                {
                    m_typeItemHash[type].removeOne(item);
                }

                return;
            }
        }
    }
}

void DataStore::deleteTypeItem(const QString &type, QGraphicsItem *item)
{
    if(m_typeItemHash.value(type).contains(item))
    {
        m_typeItemHash[type].removeOne(item);
    }
}

void DataStore::deleteTypeItem(const QString &type, int pos)
{

    if(m_typeItemHash[type].size()>pos)
    {
        m_typeItemHash[type].removeAt(pos);
    }
}

void DataStore::insertTypeItem(const QString &type, QGraphicsItem *item)
{
    if(item!=nullptr)
    {
        m_typeItemHash[type].push_back(item);
    }
}

void DataStore::deleteType(const QString &type, const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum,const QString &powerAddr)
{

    QString curStr=QString("%1,%2,%3,%4,%5").arg(extNum).arg(loopNum).arg(addrNum).arg(networkNum).arg(powerAddr);
   if(m_typeNoItemHash.value(type).contains(curStr))
   {
       m_typeNoItemHash[type].removeOne(curStr);
   }
}

void DataStore::deleteTypeItem(const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum)
{
    QList<QString>typeList=  m_typeNoItemHash.keys();
    foreach (QString type, typeList)
    {
        deleteType(type,extNum,loopNum,addrNum,networkNum);
    }

}

void DataStore::deleteTypeNoItem(const QString &extNum)
{
    QList<QString>typeList=  m_typeNoItemHash.keys();
    foreach (QString type, typeList)
    {
       QList<QString>valueList= m_typeNoItemHash.value(type);
       foreach (QString value, valueList)
       {
           if(value.startsWith(extNum))
           {
               m_typeNoItemHash[type].removeOne(value);
           }
       }

    }
}

void DataStore::insertTypeNoItem(const QString &type, const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum,const QString &powerAddr)
{
    QString curStr=QString("%1,%2,%3,%4,%5").arg(extNum).arg(loopNum).arg(addrNum).arg(networkNum).arg(powerAddr);
    m_typeNoItemHash[type].push_back(curStr);
}

void DataStore::clearTypeItem()
{
    m_typeItemHash.clear();
    m_typeNoItemHash.clear();

}

int DataStore::numOfTypeItem(const QString &type)
{
    int typeItemHashSize = m_typeItemHash.value(type).size();
    if(typeItemHashSize<0)
    {
        typeItemHashSize=0;
    }
    int typeNoItemHashSize = m_typeNoItemHash.value(type).size();
    if(typeNoItemHashSize<0)
    {
        typeNoItemHashSize=0;
    }
    return typeItemHashSize+typeNoItemHashSize;
}

GraphicsView *DataStore::itemDisplayView(GraphicsItem *item)
{
    GraphicsView *currentView = nullptr;
    QList<GraphicsView*> viewList= ArchitePlanView::getWidgetHash().values();
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

bool DataStore::haveTypeItem(const QString &type,const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum)
{
    QList<QString> dataInfoList = m_typeNoItemHash.value(type);
    QString curStr=QString("%1,%2,%3,%4").arg(extNum).arg(loopNum).arg(addrNum).arg(networkNum);
    return dataInfoList.contains(curStr);
}

QHash<QString, QList<QString> > &DataStore::getTypeNoItemHash()
{
    return m_typeNoItemHash;
}





int DataStore::indexOfItem(const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum, const QString &alarmType)
{
    QList<QGraphicsItem*> itemList= m_typeItemHash.value(alarmType);
    GraphicsItem*selectItem = nullptr;

    foreach (QGraphicsItem*item, itemList) {
        GraphicsItem *curItem = dynamic_cast<GraphicsItem*>(item);
        if(curItem!=nullptr)
        {
            if(curItem->extNum()==extNum&&curItem->loopNum()==loopNum&&curItem->addrNum()==addrNum&&curItem->networkNum()==networkNum)
            {
                selectItem = curItem;
                break;
            }
        }
    }

    if(selectItem==nullptr)
    {
        return -1;
    }
    else
    {
        return itemList.indexOf(selectItem);
    }
}

int &DataStore::itemNum()
{
    //qDebug() << s_itemNum << s_itemNumTemp;
    if(s_itemNum!=s_itemNumTemp)
    {
        s_itemNum = s_itemNumTemp++;
    }
    else
    {
        s_itemNum = ++s_itemNumTemp;
    }

    return s_itemNum;
}

void DataStore::setItemNum(int num)
{
    s_itemNum = num;
    s_itemNumTemp =num;
}

QString &DataStore::loopNum()
{
    return m_loopNum;
}

QString &DataStore::extNum()
{
    return m_extNum;
}

QString &DataStore::networkNum()
{
    return m_networkNum;
}

QString &DataStore::sysName()
{
    return m_sysName;
}

int &DataStore::channelNum()
{
    return m_channelNum;
}

QString &DataStore::analogValue()
{
    return m_analogValue;
}

qreal &DataStore::iconSize()
{
    return m_iconSize;
}

QString &DataStore::oneOperator()
{
    return m_operator;
}

QString &DataStore::powerAddr()
{
    return m_powerAddr;
}


