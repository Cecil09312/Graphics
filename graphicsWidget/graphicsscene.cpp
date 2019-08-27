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
    init();
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
            if(ItemIconInfoToJson::currentIconIndex()>=0)
                addGraphicsItem(event->scenePos().x(),event->scenePos().y());
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
    GraphicsItem *currentItem=dynamic_cast<GraphicsItem *> (itemAt(m_currentPointF,QTransform()));
    if(currentItem!=nullptr)
    {
        currentItem->setRadius(size);
    }
    update();
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
            currentItem->extNum() = info;
        }
        else if(type=="loopNum")
        {
            currentItem->loopNum() = info;
        }
        else if(type =="addrNum")
        {
            currentItem->addrNum() = info;
        }

        else if(type =="networkNum")
        {
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

        else if(type == "periodOfValidity")
        {
            currentItem->setPeriodOfValidity(info);

        }
        else if(type == "operator")
        {
            currentItem->deviceOperator() = info;
        }

        else if(type == "channelNum")
        {
            currentItem->setChannelNum(info.toInt());
        }
        else if(type == "analogType")
        {
            currentItem->analogType()= info;
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
            if(graphicsItem->iconIndex()==index)
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
            if(graphicsItem->iconIndex()==index)
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
            if(graphicsItem->iconIndex()==index)
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
            if(graphicsItem->iconIndex()==index)
            {
                graphicsItem->periodOfValidity()=periodOfValidity;
                graphicsItem->update();
            }
        }
    }
}

//void GraphicsScene::restoreAlarm(QString extNum, QString loopNum, QString addrNum, QString networkNum)
//{
//    ArchitePlanView*architePlanView=   Controller::instance()->getArchitePlanView();
//    if(architePlanView!=nullptr)
//    {
//        architePlanView->eliminateAlarm(extNum,loopNum,addrNum,networkNum);
//    }
//}

void GraphicsScene::init()
{
//    m_graphicsItemSettingMenu = new QMenu;
//    QMenu *modeSelectMenu = new QMenu("模式选择",m_graphicsItemSettingMenu);
//    m_deleteAction = new QAction(tr("删除"),m_graphicsItemSettingMenu);
//    m_editAction = new QAction(tr("编辑"),m_graphicsItemSettingMenu);
//    m_clearAction = new QAction(tr("清空"),m_graphicsItemSettingMenu);
//    m_deleteSelectedAction = new QAction(tr("删除选中"),m_graphicsItemSettingMenu);
//    m_closeAction= new QAction(tr("关闭"),m_graphicsItemSettingMenu);
//    m_analogAlarmAction = new QAction(tr("报警模拟"),m_graphicsItemSettingMenu);
//    m_maintenanceAction = new QAction(tr("设备维保"),m_graphicsItemSettingMenu);
//    m_handDragAction = new QAction(tr("手动拖拽模式"),modeSelectMenu);
//    m_rubberBandDragAction = new QAction(tr("橡皮筋模式"),modeSelectMenu);
//    m_modeActionGroup = new QActionGroup(this);
//    modeSelectMenu->addAction(m_handDragAction);
//    modeSelectMenu->addAction(m_rubberBandDragAction);
//    m_modeActionGroup->addAction(m_rubberBandDragAction);
//    m_modeActionGroup->addAction(m_handDragAction);

//    m_modeActionGroup->setExclusive(true);
//    m_handDragAction->setCheckable(true);
//    m_rubberBandDragAction->setCheckable(true);
//    m_handDragAction->setChecked(true);
//    m_graphicsItemSettingMenu->addMenu(modeSelectMenu);
//    m_itemSettingView = new QQuickView;
//    m_itemSettingView->setSource(QUrl("qrc:/qml/itemSetting/GraphicsItemEditor.qml"));
//    m_itemSettingView->setTitle(tr("设备信息设置界面"));
//    m_analogAlarmView = new QQuickView;
//    m_analogAlarmView->setSource(QUrl("qrc:/qml/itemSetting/AnalogAlarmItem.qml"));
//    m_analogAlarmView->setTitle(tr("报警模拟界面"));
//    //m_maintenanceView = new QQuickView;
//    //m_maintenanceView->setSource(QUrl("qrc:/qml/infoSetting/MaintenanceInfo.qml"));
//   // m_maintenanceView->setTitle(tr("设备维保"));
//    m_itemSettingObj= m_itemSettingView->rootObject();

//    m_graphicsItemSettingMenu->addAction(m_editAction);
//    m_graphicsItemSettingMenu->addAction(m_analogAlarmAction);
//   // m_graphicsItemSettingMenu->addAction(m_maintenanceAction);
//    m_graphicsItemSettingMenu->addAction(m_deleteAction);
//    m_graphicsItemSettingMenu->addAction(m_deleteSelectedAction);
//    m_graphicsItemSettingMenu->addAction(m_clearAction);
//    m_graphicsItemSettingMenu->addAction(m_closeAction);

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
        itemInfo.m_loopNum= itemHash["loopNum"].toString();
        itemInfo.m_addrNum= itemHash["addrNum"].toString();
        itemInfo.m_networkNum= itemHash["networkNum"].toString();
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







