#include "datastore.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "architePlan/architeplanview.h"
DataStore::DataStore(QObject *parnet)
    :QObject(parnet),
      m_priority(100)
{
    reInit();
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
     updatePriority();
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
                    if(m_typeItemHash.value(type).isEmpty())
                    {
                        m_typeItemHash.remove(type);
                    }
                    updatePriority();
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
        if(m_typeItemHash.value(type).isEmpty())
        {
            m_typeItemHash.remove(type);
        }
         updatePriority();
    }
}

void DataStore::deleteTypeItem(const QString &type, int pos)
{

    if(m_typeItemHash[type].size()>pos)
    {
        m_typeItemHash[type].removeAt(pos);
        if(m_typeItemHash.value(type).isEmpty())
        {
            m_typeItemHash.remove(type);
        }
        updatePriority();
    }

}

void DataStore::insertTypeItem(const QString &type, QGraphicsItem *item)
{
    if(item!=nullptr)
    {
        if(!m_typeItemHash.value(type).contains(item))
        {
            m_typeItemHash[type].push_back(item);
            if(m_priority>m_priorityHash.value(type))
            {
                m_priority = m_priorityHash.value(type);
            }

        }
    }
}

void DataStore::deleteType(const QString &type, const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum,const QString &powerAddr)
{

    QString curStr=QString("%1,%2,%3,%4,%5").arg(extNum).arg(loopNum).arg(addrNum).arg(networkNum).arg(powerAddr);
    if(m_typeNoItemHash.value(type).contains(curStr))
    {
        m_typeNoItemHash[type].removeOne(curStr);
        m_typeNoItemTimeHash.remove(curStr);
        if(m_typeNoItemHash.value(type).isEmpty())
        {
            m_typeNoItemHash.remove(type);
        }
        updatePriority();
    }

}

void DataStore::deleteTypeItem(const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum)
{
    QList<QString>typeList=  m_typeNoItemHash.keys();
    foreach (QString type, typeList)
    {
        deleteType(type,extNum,loopNum,addrNum,networkNum);
    }
    updatePriority();

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
                m_typeNoItemTimeHash.remove(value);
                if(m_typeNoItemHash.value(type).isEmpty())
                {
                    m_typeNoItemHash.remove(type);
                }
            }
        }

    }
    updatePriority();
}

void DataStore::insertTypeNoItem(const QString &type, const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum,const QString &powerAddr,const QString&timeStr)
{
    QString curStr=QString("%1,%2,%3,%4,%5").arg(extNum).arg(loopNum).arg(addrNum).arg(networkNum).arg(powerAddr);
    m_typeNoItemTimeHash[curStr] = timeStr;

    if(!m_typeNoItemHash.value(type).contains(curStr))
    {
        m_typeNoItemHash[type].push_back(curStr);
        if(m_priority>m_priorityHash.value(type))
        {
            m_priority = m_priorityHash.value(type);
        }
    }

}

void DataStore::clearTypeItem()
{
    m_typeItemHash.clear();
    m_typeNoItemHash.clear();
    m_typeNoItemTimeHash.clear();
    m_priority=100;

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

bool DataStore::haveTypeItem(const QString &type,const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum,const QString &powerAddr)
{
    QList<QString> dataInfoList = m_typeNoItemHash.value(type);
    QString curStr=QString("%1,%2,%3,%4,%5").arg(extNum).arg(loopNum).arg(addrNum).arg(networkNum).arg(powerAddr);
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

int DataStore::indexOfTypeFromItem(const QString &alarmType, GraphicsItem *item)
{
    return m_typeItemHash.value(alarmType).indexOf(item);
}

bool DataStore::containsAlarms(const QString&alarm)
{
    if(getTypeItemList(alarm).size()>0)
    {
        return  true;
    }
    else
    {
        return  false;
    }
}

bool DataStore::containsAlarmsNoItem(const QString &alarm)
{

    if(m_typeNoItemHash.value(alarm).size()>0)
    {
        return true;
    }
    else
    {
        return  false;
    }
}

int DataStore::itemNum()
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

    emit curLoopAddr(m_loopNum,QString::number(s_itemNum));
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

void DataStore::reInit()
{
    s_itemNum=0;
    m_loopNum="0";
    m_extNum="0";
    m_networkNum="0";
    m_powerAddr ="0";
    m_sysName = "";
    m_channelNum=0;
    m_analogValue="无";
    m_iconSize=30;
    m_operator="";
    s_itemNumTemp =0;
    setPriority();

}

void DataStore::clearStoreAlarm()
{
    QList< QList<QGraphicsItem *> >list= m_typeItemHash.values();
    foreach (QList<QGraphicsItem *>itemList, list)
    {
        foreach (QGraphicsItem *currentItem, itemList)
        {
            GraphicsItem *item = dynamic_cast<GraphicsItem *>(currentItem);
            if(item!=nullptr)
            {
                item->stopAnimations();

                disconnect(item,&GraphicsItem::moveToPos,nullptr,nullptr);
                //disconnect(item,&GraphicsItem::sizeChanged,nullptr,nullptr);
                item->clearAllAlarm();
            }
        }
    }
    clearTypeItem();
}



int &DataStore::priority()
{
    return m_priority;
}

QString DataStore::priorityType(int prioityValue)
{
    return   m_priorityHash.key(prioityValue);
}

QString DataStore::currentPriorityType()
{
    return  m_priorityHash.key(m_priority);
}


void DataStore::updatePriority()
{


    if(m_typeItemHash.isEmpty()&&m_typeNoItemHash.isEmpty())
    {
        m_priority=100;
    }
    else
    {

        if(!m_typeItemHash.value(tr("火警")).isEmpty()||!m_typeNoItemHash.value(tr("火警")).isEmpty())
        {
            m_priority =1;
        }
        else if(!m_typeItemHash.value(tr("监管")).isEmpty()||!m_typeNoItemHash.value(tr("监管")).isEmpty())
        {
             m_priority =2;
        }
        else if(!m_typeItemHash.value(tr("启动")).isEmpty()||!m_typeNoItemHash.value(tr("启动")).isEmpty())
        {
            m_priority =3;
        }
        else if(!m_typeItemHash.value(tr("反馈")).isEmpty()||!m_typeNoItemHash.value(tr("反馈")).isEmpty())
        {
            m_priority =4;
        }
        else if(!m_typeItemHash.value(tr("故障")).isEmpty()||!m_typeNoItemHash.value(tr("故障")).isEmpty())
        {
            m_priority =5;
        }

        else if(!m_typeItemHash.value(tr("屏蔽")).isEmpty()||!m_typeNoItemHash.value(tr("屏蔽")).isEmpty())
        {
            m_priority =6;
        }
    }


    // qDebug() <<"1111" << m_typeItemHash<<m_priority;
}

QString DataStore::getTypeNoItemTime(const QString &str)
{
    return  m_typeNoItemTimeHash.value(str);
}

void DataStore::setPriority()
{
    m_priorityHash.clear();
    m_priorityHash[tr("火警")]=1;
    m_priorityHash[tr("监管")]=2;
    m_priorityHash[tr("启动")]=3;
    m_priorityHash[tr("反馈")]=4;
    m_priorityHash[tr("故障")]=5;
    m_priorityHash[tr("屏蔽")]=6;

}


