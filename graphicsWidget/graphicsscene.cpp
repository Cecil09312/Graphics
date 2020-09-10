#include "graphicsscene.h"
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "graphicsWidget/graphicsitem.h"
#include "control/controller.h"
#include "dataStore/datastore.h"
#include <QQuickItem>

GraphicsScene::GraphicsScene(QObject *parent):
    QGraphicsScene(parent)
{

}

GraphicsScene::~GraphicsScene()
{

}

GraphicsItem *GraphicsScene::addGraphicsItem(qreal ax, qreal ay)
{
    return addGraphicsItem(QPointF(ax,ay));
}

GraphicsItem *GraphicsScene::addGraphicsItem(const QPointF &pointF)
{
    GraphicsItem *item= new GraphicsItem(this);
    item->setPos(pointF);
    this->addItem(item);
    int itemIconIndex = ItemIconInfoToJson::currentIconIndex();
    item->setInfoFromIconIndex(itemIconIndex);
    m_itemList.push_back(item);
    emit createItem(item);
    return item;
}

void GraphicsScene::removeGraphicsItem(qreal ax, qreal ay)
{
    removeGraphicsItem(QPointF(ax,ay));
}

void GraphicsScene::removeGraphicsItem(const QPointF &pointF)
{
    QGraphicsItem*currentItem= itemAt(pointF,QTransform());
    removeGraphicsItem(currentItem);
}

void GraphicsScene::removeGraphicsItem(QGraphicsItem *currentItem)
{
    if(currentItem!=nullptr)
    {
        if(m_itemList.contains(currentItem))
        {
            GraphicsItem *graphicsItem = dynamic_cast<GraphicsItem *>(currentItem);
            if(graphicsItem!=nullptr)
            {
                if(graphicsItem->currentState()=="OK"||graphicsItem->currentState().isEmpty())
                {

                    m_itemList.removeOne(currentItem);
                    removeItem(currentItem);
                    Controller::instance()->getDataStore()->deleteTypeItem(currentItem);
                }

            }

        }
    }
}


QPointF GraphicsScene::currentScenePos()
{
    return m_currentPointF;
}

void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(Controller::instance()->getUserRight()==UserManager::Super)
        {
            if(ArchitePlanView::itemLimit())
            {
                if(ItemIconInfoToJson::currentIconIndex()>=0)
                {
                    GraphicsItem *item= addGraphicsItem(event->scenePos().x(),event->scenePos().y());
                    emit addOneItem(item);
                }
            }

        }
    }
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        m_currentPointF =event->scenePos();
    }

    QGraphicsScene::mousePressEvent(event);
}



void GraphicsScene::setItemColor(QColor color)
{
    GraphicsItem *currentItem=dynamic_cast<GraphicsItem *> (itemAt(m_currentPointF,QTransform()));
    if(currentItem!=nullptr)
    {
        currentItem->setColor(color);
    }
    update();
}

void GraphicsScene::setItemSize(qreal size)
{
    Controller::instance()->getDataStore()->iconSize() = size;
    GraphicsItem *currentItem=dynamic_cast<GraphicsItem *> (itemAt(m_currentPointF,QTransform()));
    if(currentItem!=nullptr)
    {
        currentItem->setRadius(size);
        update();
    }

}


void GraphicsScene::setItemIcon(QString iconName)
{
    GraphicsItem *currentItem=dynamic_cast<GraphicsItem *> (itemAt(m_currentPointF,QTransform()));
    if(currentItem!=nullptr)
    {
        currentItem->setIconName(Controller::instance()->fileNameFromQml(iconName));
    }
    //update();
}

void GraphicsScene::setItemInfoFromType(const QString &type, const QString &info)
{
    GraphicsItem *currentItem=dynamic_cast<GraphicsItem *> (itemAt(m_currentPointF,QTransform()));
    if(currentItem!=nullptr)
    {
        if(!m_oldItemInfoHash.contains(currentItem))
        {
             m_oldItemInfoHash[currentItem]=QString("%1,%2,%3,%4,%5").arg(currentItem->extNum()).arg(currentItem->loopNum())
                     .arg(currentItem->addrNum()).arg(currentItem->networkNum()).arg(currentItem->powerAddr());
        }

        if(type=="currentState")
        {
            //currentItem->currentState() = info;
            currentItem->setAlarmState(type,info);
        }
        else if(type=="equipmentModel")
        {
            currentItem->equipmentModel() = info;
        }
        else if(type=="extNum")
        {
            Controller::instance()->getDataStore()->extNum()=info;
            currentItem->extNum() = info;
        }
        else if(type=="loopNum")
        {
            Controller::instance()->getDataStore()->loopNum()=info;
            currentItem->loopNum() = info;
        }
        else if(type =="addrNum")
        {
            currentItem->addrNum() = info;
            Controller::instance()->getDataStore()->setItemNum(info.toInt());
            currentItem->update();

        }

        else if(type =="networkNum")
        {
            Controller::instance()->getDataStore()->networkNum()=info;
            currentItem->networkNum()= info;
        }
        else if(type =="powerAddr")
        {
            Controller::instance()->getDataStore()->powerAddr()=info;
            currentItem->powerAddr()= info;
        }
        else if(type =="deviceNum")
        {
            currentItem->deviceNum() = info;
            currentItem->update();
            // update();
        }

        else if(type =="sysOfDevice")
        {
            Controller::instance()->getDataStore()->sysName() = info;
            currentItem->sysOfDevice() = info;
        }
        else if(type =="manufacturers")
        {
            currentItem->manufacturers() = info;

        }
        else if(type =="buildingName")
        {
            currentItem->buildingName() = info;
        }
        else if(type =="floorOfDevice")
        {
            currentItem->floorOfDevice() = info;
        }

        else if(type == "deviceLocation")
        {
            currentItem->deviceLocation() = info;
        }

        else if(type == "deviceInstallTime")
        {
            currentItem->deviceInstallTime() = info;
        }


        else if(type == "periodOfValidity")
        {
            currentItem->setPeriodOfValidity(info);

        }
        else if(type == "operator")
        {
            Controller::instance()->getDataStore()->oneOperator() = info;
            currentItem->deviceOperator() = info;
        }

        else if(type == "channelNum")
        {
            Controller::instance()->getDataStore()->channelNum() = info.toInt();
            currentItem->setChannelNum(info.toInt());
        }
        else if(type == "analogType")
        {
            Controller::instance()->getDataStore()->analogValue() = info;
            currentItem->analogType()= info;
        }
        if(!m_settingItemList.contains(currentItem))
        {
            m_settingItemList.push_back(currentItem);
        }


    }
    else
    {
        if(type=="extNum")
        {
            Controller::instance()->getDataStore()->extNum()=info;
        }
        else if(type=="loopNum")
        {
            Controller::instance()->getDataStore()->loopNum()=info;
        }
        else if(type =="addrNum")
        {
            Controller::instance()->getDataStore()->setItemNum(info.toInt()-1);
        }
        else if(type =="networkNum")
        {
            Controller::instance()->getDataStore()->networkNum()=info;
        }
        else if(type =="powerAddr")
        {
            Controller::instance()->getDataStore()->powerAddr()=info;
        }
        else if(type == "operator")
        {
            Controller::instance()->getDataStore()->oneOperator() = info;
        }

        else if(type == "channelNum")
        {
            Controller::instance()->getDataStore()->channelNum() = info.toInt();
        }
        else if(type == "analogType")
        {
            Controller::instance()->getDataStore()->analogValue() = info;
        }
        else if(type =="sysOfDevice")
        {
            Controller::instance()->getDataStore()->sysName() = info;
        }

    }

}


void GraphicsScene::setItemsIcon(int index, QString iconName)
{
    QList<QGraphicsItem*>itemList =  getItemList();
    foreach (QGraphicsItem*item, itemList)
    {
        GraphicsItem*graphicsItem = dynamic_cast<GraphicsItem*>(item);
        if(graphicsItem!=nullptr)
        {
            int iconIndex= ItemIconInfoToJson::iconIndex(graphicsItem->iconName());
            if(iconIndex==index)
            {
                graphicsItem->setIconName(iconName);
            }
        }
    }
}

void GraphicsScene::setItemsEquipmentModel(int index, QString device)
{
    QList<QGraphicsItem*>itemList =  getItemList();
    foreach (QGraphicsItem*item, itemList)
    {
        GraphicsItem*graphicsItem = dynamic_cast<GraphicsItem*>(item);
        if(graphicsItem!=nullptr)
        {
            int iconIndex= ItemIconInfoToJson::iconIndex(graphicsItem->iconName());
            if(iconIndex==index)
            {
                graphicsItem->equipmentModel()=device;
                graphicsItem->update();
            }
        }
    }
}

void GraphicsScene::setItemsManufacturers(int index, QString manufacturers)
{
    QList<QGraphicsItem*>itemList =  getItemList();
    foreach (QGraphicsItem*item, itemList)
    {
        GraphicsItem*graphicsItem = dynamic_cast<GraphicsItem*>(item);
        if(graphicsItem!=nullptr)
        {
            int iconIndex= ItemIconInfoToJson::iconIndex(graphicsItem->iconName());
            if(iconIndex==index)
            {
                graphicsItem->manufacturers()=manufacturers;
                graphicsItem->update();
            }
        }
    }
}

void GraphicsScene::setItemsPeriodOfValidity(int index, QString periodOfValidity)
{
    QList<QGraphicsItem*>itemList =  getItemList();
    foreach (QGraphicsItem*item, itemList)
    {
        GraphicsItem*graphicsItem = dynamic_cast<GraphicsItem*>(item);
        if(graphicsItem!=nullptr)
        {
            int iconIndex= ItemIconInfoToJson::iconIndex(graphicsItem->iconName());
            if(iconIndex==index)
            {
                graphicsItem->periodOfValidity()=periodOfValidity;
                graphicsItem->update();
            }
        }
    }
}

void GraphicsScene::setDeviceInstallTime(int index, QString installTime)
{
    QList<QGraphicsItem*>itemList =  getItemList();
    foreach (QGraphicsItem*item, itemList)
    {
        GraphicsItem*graphicsItem = dynamic_cast<GraphicsItem*>(item);
        if(graphicsItem!=nullptr)
        {

            int iconIndex= ItemIconInfoToJson::iconIndex(graphicsItem->iconName());
            if(iconIndex==index)
            {
                graphicsItem->deviceInstallTime()=installTime;
                //graphicsItem->update();
            }

        }
    }
}

QList<QGraphicsItem *>& GraphicsScene::getItemList()
{
    return m_itemList;
}

QGraphicsItem *GraphicsScene::getItem(int pos) const
{
    if(m_itemList.size()>pos)
    {
        return m_itemList.at(pos);
    }
    else
    {
        return nullptr;
    }
}

void GraphicsScene::setItemInfo(GraphicsItem *item, const QHash<QString, QVariant> &itemHash)
{

    if(item!=nullptr)
    {
        QString pos= itemHash["pos"].toString();
        ItemInfo itemInfo;
        itemInfo.m_extNum= itemHash["extNum"].toString();
        Controller::instance()->getDataStore()->extNum() = itemInfo.m_extNum;
        itemInfo.m_loopNum= itemHash["loopNum"].toString();
        Controller::instance()->getDataStore()->loopNum() = itemInfo.m_loopNum;
        itemInfo.m_addrNum= itemHash["addrNum"].toString();
        itemInfo.m_networkNum= itemHash["networkNum"].toString();
        Controller::instance()->getDataStore()->networkNum()=itemInfo.m_networkNum;
        Controller::instance()->getDataStore()->powerAddr() = itemHash["powerAddr"].toString();

        itemInfo.m_deviceNum= itemHash["deviceNum"].toString();
        itemInfo.m_equipmentModel= itemHash["equipmentModel"].toString();
        //itemInfo.m_currentState= itemHash["currentState"].toString();
        itemInfo.m_sysOfDevice= itemHash["sysOfDevice"].toString();
        itemInfo.m_deviceLocation=itemHash["deviceLocation"].toString();
        itemInfo.m_buildingName= itemHash["buildingName"].toString();
        itemInfo.m_floorOfDevice= itemHash["floorOfDevice"].toString();
        itemInfo.m_manufacturers= itemHash["manufacturers"].toString();
        itemInfo.m_periodOfValidity = itemHash["periodOfValidity"].toString();

        item->setItemInfo(itemInfo);
        item->powerAddr() = itemHash["powerAddr"].toString();
        item->setIconName(itemHash["iconName"].toString());
        item->setRadius(itemHash["size"].toDouble());
        item->setPos(pos.section(",",0,0).toDouble(),pos.section(",",1,1).toDouble());
        item->currentState() = itemHash["currentState"].toString();
        addItem(item);
        m_itemList.push_back(item);
    }
}

bool GraphicsScene::isHavingAlarms()
{
    bool isHavingAlarm = false;
    foreach (QGraphicsItem*item, m_itemList)
    {
        GraphicsItem * graphicsItem = dynamic_cast<GraphicsItem *>(item);
        if(graphicsItem!=nullptr)
        {
            if(graphicsItem->currentState()!="OK")
            {
                isHavingAlarm = true;
                break;
            }
        }
    }
    return isHavingAlarm;
}

void GraphicsScene::checkSettingItem()
{
    foreach(GraphicsItem*curItem,m_settingItemList)
    {
        emit itemSetting(curItem);
    }
    m_settingItemList.clear();
}

QString GraphicsScene::itemOldInfo(GraphicsItem *item)
{
    return  m_oldItemInfoHash.value(item);
}

void GraphicsScene::clearItemOldInfo()
{
    m_oldItemInfoHash.clear();
}







