#include "datastore.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "architePlan/architeplanview.h"
QHash<QString,QList<QGraphicsItem*> >DataStore::m_typeItemHash=QHash<QString,QList<QGraphicsItem*> >();
QHash<QString,QList<DataInfo*> >DataStore::m_typeNoItemHash = QHash<QString,QList<DataInfo*> >();
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

bool DataStore::deleteType(const QString &type, const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum)
{
    static bool isExist = false;
    QList<DataInfo*>  dataInfoList= m_typeNoItemHash[type];
    foreach (DataInfo*curDataInfo, dataInfoList)
    {
        if(curDataInfo!=nullptr)
        {
            if((curDataInfo->m_addrNum==addrNum) && (curDataInfo->m_extNum==extNum) &&(curDataInfo->m_loopNum == loopNum) && (curDataInfo->m_networkNum==networkNum))
            {
                m_typeNoItemHash[type].removeOne(curDataInfo);
                delete curDataInfo;
                curDataInfo = nullptr;
                isExist = true;
                break;
            }
        }
    }
    return isExist;
}

void DataStore::deleteTypeItem(const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum)
{
  QList<QString>typeList=  m_typeNoItemHash.keys();
  foreach (QString type, typeList)
  {

      if(deleteType(type,extNum,loopNum,addrNum,networkNum))
      {
          break;
      }
  }

}

void DataStore::insertTypeItem(const QString &type, const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum)
{
    if(!haveTypeItem(type,extNum,addrNum,loopNum,networkNum))
    {
        DataInfo *dataInfo = new DataInfo;
        dataInfo->m_extNum = extNum;
        dataInfo->m_addrNum = addrNum;
        dataInfo->m_loopNum = loopNum;
        dataInfo->m_networkNum = networkNum;
        m_typeNoItemHash[type].push_back(dataInfo);
    }

}

void DataStore::clearTypeItem()
{
    m_typeItemHash.clear();

    foreach (QList<DataInfo*>dataInfoList, m_typeNoItemHash.values())
    {
        foreach (DataInfo*dataInfo, dataInfoList)
        {
            delete dataInfo;
            dataInfo= nullptr;
        }
    }
    m_typeNoItemHash.clear();

}

int DataStore::numOfTypeItem(const QString &type)
{
    return m_typeItemHash[type].size()+m_typeNoItemHash[type].size();
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

bool DataStore::haveTypeItem(const QString &type,const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum)
{
    bool isOk= false;
    QList<DataInfo *> dataInfoList = m_typeNoItemHash[type];
    foreach(DataInfo *dataInfo,dataInfoList)
    {
        if((dataInfo->m_addrNum==addrNum) &&(dataInfo->m_extNum==extNum )&& (dataInfo->m_loopNum==loopNum )&& (dataInfo->m_networkNum==networkNum))
        {
            isOk = true;
            break;
        }
    }
    return  isOk;
}

QHash<QString, QList<DataInfo *> > &DataStore::getTypeNoItemHash()
{
    return m_typeNoItemHash;
}

QString DataStore::getTypeNoItemKey(DataInfo *dataInfo)
{
   QList<QString>keyList= m_typeNoItemHash.keys();
   static QString keyStr = "";
   foreach (QString key, keyList)
   {
       QList<DataInfo *>dataInfoList= m_typeNoItemHash.value(key);
       if(dataInfoList.contains(dataInfo))
       {
           keyStr=key;
           break;
       }
   }
   return keyStr;
}

void DataStore::deleteDataInfo(DataInfo *dataInfo)
{
  QList<QString>  keyList=m_typeNoItemHash.keys();
  foreach (QString key, keyList) {
      if(m_typeNoItemHash.value(key).contains(dataInfo))
      {
          m_typeNoItemHash[key].removeOne(dataInfo);
      }
  }
}


