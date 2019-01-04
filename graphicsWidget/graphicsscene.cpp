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
    connect(m_deleteAction,&QAction::triggered,this,[=]()
    {
        removeGraphicsItem(m_currentPointF);
    });

    connect(m_editAction,&QAction::triggered,this,[=]()
    {
        GraphicsItem *currentItem= dynamic_cast<GraphicsItem *> (itemAt(m_currentPointF,QTransform()));
        Q_ASSERT(m_itemSettingObj);
        QVariant currentIndex;
        QMetaObject::invokeMethod(m_itemSettingObj,"currentIconIndex",Q_RETURN_ARG(QVariant,currentIndex));
        if(currentItem!=nullptr)
        {


            QMetaObject::invokeMethod(m_itemSettingObj,"setDeviceNum",Q_ARG(QVariant,currentItem->deviceNum()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setAlarmType",Q_ARG(QVariant,currentItem->alarmType()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setItemSize",Q_ARG(QVariant,currentItem->radius()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setExtNum",Q_ARG(QVariant,currentItem->extNum()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setLoopNum",Q_ARG(QVariant,currentItem->loopNum()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setAddrNum",Q_ARG(QVariant,currentItem->addrNum()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setEquipmentModel",Q_ARG(QVariant,currentItem->equipmentModel()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setSysOfDevice",Q_ARG(QVariant,currentItem->sysOfDevice()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setProtectedArea",Q_ARG(QVariant,currentItem->protectedAreaName()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setBuildingName",Q_ARG(QVariant,currentItem->buildingName()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setFloorOfDevice",Q_ARG(QVariant,currentItem->floorOfDevice()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setDeviceLocation",Q_ARG(QVariant,currentItem->deviceLocation()));
            QMetaObject::invokeMethod(m_itemSettingObj,"setOperatorOnDuty",Q_ARG(QVariant,currentItem->operatorDuty()));

        }
        else
        {

            QMetaObject::invokeMethod(m_itemSettingObj,"clearItemInfo");
        }
        m_itemSettingView->show();
    });
    connect(m_clearAction,&QAction::triggered,this,[=]()
    {
        foreach (QGraphicsItem*item, m_itemList)
        {
            removeItem(item);
        }
        m_itemList.clear();
        Controller::instance()->getDataStore()->clearTypeItem();
    });
    connect(m_deleteSelectedAction,&QAction::triggered,this,[=]()
    {
        QList<QGraphicsItem*>itemList =selectedItems();
        foreach (QGraphicsItem*item,itemList)
        {
            if(item!=nullptr)
            {
                removeItem(item);
                m_itemList.removeOne(item);
                Controller::instance()->getDataStore()->deleteTypeItem(item);
            }
        }
    });
    connect(m_closeAction,&QAction::triggered,this,[=]()
    {
        m_graphicsItemSettingMenu->close();
    });

    connect(m_modeActionGroup,&QActionGroup::triggered,this,[=](QAction *action)
    {
        QList<QGraphicsView*>viewList = this->views();
        foreach (QGraphicsView*view, viewList) {
            if(view!=nullptr)
            {
                if(action==m_handDragAction)
                {
                    view->setDragMode(QGraphicsView::ScrollHandDrag);
                }
                else
                {
                    view->setDragMode(QGraphicsView::RubberBandDrag);
                }
            }
        }
    });

    Q_ASSERT(m_itemSettingObj);
    connect(m_itemSettingObj,SIGNAL(setSize(qreal)),this,SLOT(setItemSize(qreal)));
    connect(m_itemSettingObj,SIGNAL(setIcon(QString)),this,SLOT(setItemIcon(QString)));
    connect(m_itemSettingObj,SIGNAL(setItemInfo(QString,QString)),this,SLOT(setItemInfoFromType(const QString , const QString &)));

}

GraphicsScene::~GraphicsScene()
{
    delete m_graphicsItemSettingMenu;
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
    //Controller::instance()->getDataStore()->insertTypeItem(tr("火警"),item);
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
            m_itemList.removeOne(currentItem);
            removeItem(currentItem);
            Controller::instance()->getDataStore()->deleteTypeItem(currentItem);
        }

    }
}

void GraphicsScene::showMenu(const QPoint &point)
{
    if(Controller::instance()->getUserRight()!=UserManager::Super)
    {
        m_deleteAction->setEnabled(false);
        m_editAction->setEnabled(false);
        m_clearAction->setEnabled(false);
        m_deleteSelectedAction->setEnabled(false);
    }
    else
    {
        if(!m_itemList.isEmpty())
        {
            if(m_itemList.contains(itemAt(m_currentPointF,QTransform())))
            {
                m_deleteAction->setEnabled(true);
            }
            else
            {
                m_deleteAction->setEnabled(false);
            }
            m_clearAction->setEnabled(true);
            m_deleteSelectedAction->setEnabled(true);
        }
        else
        {
            m_deleteAction->setEnabled(false);
            m_clearAction->setEnabled(false);
            m_deleteSelectedAction->setEnabled(false);
        }
        m_editAction->setEnabled(true);
    }
    m_graphicsItemSettingMenu->exec(point);
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
        if(type=="alarmType")
        {
            currentItem->getItemInfo().m_alarmType = info;
        }
        else if(type=="equipmentModel")
        {
            currentItem->getItemInfo().m_equipmentModel = info;

        }
        else if(type=="extNum")
        {
            currentItem->getItemInfo().m_extNum = info;

        }

        else if(type=="loopNum")
        {
            currentItem->getItemInfo().m_loopNum = info;

        }
        else if(type =="addrNum")
        {
            currentItem->getItemInfo().m_addrNum = info;
        }
        else if(type =="deviceNum")
        {
            currentItem->getItemInfo().m_deviceNum = info;
            update();
        }

        else if(type =="sysOfDevice")
        {
            currentItem->getItemInfo().m_sysOfDevice = info;

        }

        else if(type =="protectedArea")
        {
            currentItem->getItemInfo().m_protectedAreaName = info;

        }
        else if(type =="buildingName")
        {
            currentItem->getItemInfo().m_buildingName = info;
        }
        else if(type =="floorOfDevice")
        {
            currentItem->getItemInfo().m_floorOfDevice = info;
        }
        else if(type == "deviceLocation")
        {
            currentItem->getItemInfo().m_deviceLocation = info;
        }

        else if(type == "operatorOnDuty")
        {
            currentItem->getItemInfo().m_operatorOnDuty = info;

        }
    }

}


void GraphicsScene::init()
{
    m_graphicsItemSettingMenu = new QMenu;
    QMenu *modeSelectMenu = new QMenu("模式选择",m_graphicsItemSettingMenu);
    m_deleteAction = new QAction(tr("删除"),m_graphicsItemSettingMenu);
    m_editAction = new QAction(tr("编辑"),m_graphicsItemSettingMenu);
    m_clearAction = new QAction(tr("清空"),m_graphicsItemSettingMenu);
    m_deleteSelectedAction = new QAction(tr("删除选中"),m_graphicsItemSettingMenu);
    m_closeAction= new QAction(tr("关闭"),m_graphicsItemSettingMenu);
    m_handDragAction = new QAction(tr("手动拖拽模式"),modeSelectMenu);
    m_rubberBandDragAction = new QAction(tr("橡皮筋模式"),modeSelectMenu);
    m_modeActionGroup = new QActionGroup(this);
    modeSelectMenu->addAction(m_handDragAction);
    modeSelectMenu->addAction(m_rubberBandDragAction);
    m_modeActionGroup->addAction(m_rubberBandDragAction);
    m_modeActionGroup->addAction(m_handDragAction);
    m_modeActionGroup->setExclusive(true);
    m_handDragAction->setCheckable(true);
    m_rubberBandDragAction->setCheckable(true);
    m_handDragAction->setChecked(true);
    m_graphicsItemSettingMenu->addMenu(modeSelectMenu);
    m_itemSettingView = new QQuickView;
    m_itemSettingView->setSource(QUrl("qrc:/qml/itemSetting/GraphicsItemEditor.qml"));
    m_itemSettingObj= m_itemSettingView->rootObject();
    m_graphicsItemSettingMenu->addAction(m_deleteAction);
    m_graphicsItemSettingMenu->addAction(m_editAction);
    m_graphicsItemSettingMenu->addAction(m_clearAction);
    m_graphicsItemSettingMenu->addAction(m_deleteSelectedAction);
    m_graphicsItemSettingMenu->addAction(m_closeAction);

}



QList<QGraphicsItem *> GraphicsScene::getItemList() const
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
        itemInfo.m_alarmType= itemHash["alarmType"].toString();
        itemInfo.m_deviceNum= itemHash["deviceNum"].toString();
        itemInfo.m_equipmentModel= itemHash["equipmentModel"].toString();
        itemInfo.m_currentAlarmState= itemHash["currentAlarmState"].toString();
        itemInfo.m_alarmTime= itemHash["alarmTime"].toString();
        itemInfo.m_alarmReceiveTime= itemHash["alarmReceiveTime"].toString();
        itemInfo.m_alarmReplyTime= itemHash["alarmReplyTime"].toString();
        itemInfo.m_sysOfDevice= itemHash["sysOfDevice"].toString();
        itemInfo.m_protectedAreaName= itemHash["protectedAreaName"].toString();
        itemInfo.m_deviceLocation=itemHash["deviceLocation"].toString();
        itemInfo.m_buildingName= itemHash["buildingName"].toString();
        itemInfo.m_floorOfDevice= itemHash["floorOfDevice"].toString();
        itemInfo.m_operatorOnDuty= itemHash["operatorOnDuty"].toString();
        item->setItemInfo(itemInfo);
        item->setIconName(itemHash["iconName"].toString());
        item->setRadius(itemHash["size"].toDouble());
        item->setPos(pos.section(",",0,0).toDouble(),pos.section(",",1,1).toDouble());
        addItem(item);
        m_itemList.push_back(item);
    }
}







