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

void GraphicsScene::addGraphicsItem(qreal ax, qreal ay)
{
    addGraphicsItem(QPointF(ax,ay));
}

void GraphicsScene::addGraphicsItem(const QPointF &pointF)
{
    GraphicsItem *item= new GraphicsItem(this);
    item->setPos(pointF);
    this->addItem(item);
    m_itemList.push_back(item);
    emit createItem(item);
}

void GraphicsScene::removeGraphicsItem(qreal ax, qreal ay)
{
    removeGraphicsItem(QPointF(ax,ay));
}

void GraphicsScene::removeGraphicsItem(const QPointF &pointF)
{
    QGraphicsItem*currentItem= itemAt(pointF,QTransform());
    if(currentItem!=nullptr)
    {
        if(m_itemList.contains(currentItem))
        {
            GraphicsItem *graphicsItem = dynamic_cast<GraphicsItem *>(currentItem);
            if(graphicsItem->currentState()==tr("正常"))
            {
                m_itemList.removeOne(currentItem);
                removeItem(currentItem);
                DataStore::deleteTypeItem(currentItem);
            }
            else
            {

                QMessageBox::warning(nullptr,tr("警告"),tr("有报警，不能被删除"));
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
                    addGraphicsItem(event->scenePos().x(),event->scenePos().y());
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
    DataStore::iconSize() = size;
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
    update();
}

void GraphicsScene::setItemInfoFromType(const QString &type, const QString &info)
{
    GraphicsItem *currentItem=dynamic_cast<GraphicsItem *> (itemAt(m_currentPointF,QTransform()));
    if(currentItem!=nullptr)
    {
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
            DataStore::extNum()=info;
            currentItem->extNum() = info;
        }
        else if(type=="loopNum")
        {
            DataStore::loopNum()=info;
            currentItem->loopNum() = info;
        }
        else if(type =="addrNum")
        {
            currentItem->addrNum() = info;
            DataStore::itemNum() = info.toInt();
        }

        else if(type =="networkNum")
        {
            DataStore::networkNum()=info;
            currentItem->networkNum()= info;
        }
        else if(type =="deviceNum")
        {
            currentItem->deviceNum() = info;
            currentItem->update();
            // update();
        }

        else if(type =="sysOfDevice")
        {
            DataStore::sysName() = info;
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
            DataStore::oneOperator() = info;
            currentItem->deviceOperator() = info;
        }

        else if(type == "channelNum")
        {
            DataStore::channelNum() = info.toInt();
            currentItem->setChannelNum(info.toInt());
        }
        else if(type == "analogType")
        {
            DataStore::analogValue() = info;
            currentItem->analogType()= info;
        }
    }
    else
    {
        if(type=="extNum")
        {
            DataStore::extNum()=info;
        }
        else if(type=="loopNum")
        {
            DataStore::loopNum()=info;
        }
        else if(type =="addrNum")
        {
            DataStore::itemNum() = info.toInt();
           // currentItem->addrNum() = info;
        }
        else if(type =="networkNum")
        {
            DataStore::networkNum()=info;
        }
        else if(type == "operator")
        {
            DataStore::oneOperator() = info;
        }

        else if(type == "channelNum")
        {
            DataStore::channelNum() = info.toInt();
        }
        else if(type == "analogType")
        {
            DataStore::analogValue() = info;
        }
        else if(type =="sysOfDevice")
        {
            DataStore::sysName() = info;
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
               // qDebug() << "****************" << iconIndex << graphicsItem->deviceInstallTime() << installTime;
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
        DataStore::extNum() = itemInfo.m_extNum;
        itemInfo.m_loopNum= itemHash["loopNum"].toString();
        DataStore::loopNum() = itemInfo.m_loopNum;
        itemInfo.m_addrNum= itemHash["addrNum"].toString();
        itemInfo.m_networkNum= itemHash["networkNum"].toString();
        DataStore::networkNum()=itemInfo.m_networkNum;
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
            if(graphicsItem->currentState()!=tr("正常"))
            {
                isHavingAlarm = true;
                break;
            }
        }
    }
    return isHavingAlarm;
}







