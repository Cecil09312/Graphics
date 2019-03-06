#include "globalgraphicsscene.h"
#include "control/controller.h"
#include "globalgraphicsitem.h"
#include <QDebug>
#include <QQmlContext>
#include "control/controller.h"
#include <QQuickItem>

GlobalGraphicsScene::GlobalGraphicsScene(QObject *parent):
    QGraphicsScene(parent),
    m_num(0)
{
    m_menu = new QMenu();
    m_removeItemAction = new QAction(tr("删除"),m_menu);
    m_editItemAction = new QAction(tr("编辑"),m_menu);
    m_removeSelectItemAction = new QAction(tr("删除选中"),m_menu);
    m_goToAchitePlanAction = new QAction(tr("转到建筑平面"),m_menu);
    m_clearItemAction = new QAction(tr("清空"),m_menu);

    m_globalItemSettingView = new QQuickView;
    m_globalItemSettingView->setSource(QUrl("qrc:/qml/itemSetting/GlobalItemSetting.qml"));
    m_globalItemSettingView->rootContext()->setContextProperty("GlobalItemSettingView",this);

    m_globalItemObj = m_globalItemSettingView->rootObject();

    m_menu->addAction(m_removeItemAction);
    m_menu->addAction(m_removeSelectItemAction);
    m_menu->addAction(m_clearItemAction);
    m_menu->addAction(m_editItemAction);
    m_menu->addAction(m_goToAchitePlanAction);

    connect(m_removeItemAction,&QAction::triggered,this,[=]()
    {
        QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
        GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
        if(item!=nullptr)
        {
            removeItem(graphicsItem);
            emit deleteGlobalItem(item);
        }
    });

    connect(m_removeSelectItemAction,&QAction::triggered,this,[=]()
    {
        QList<QGraphicsItem*>itemList =selectedItems();

        foreach (QGraphicsItem*currentItem, itemList)
        {
            GlobalGraphicsItem *item=  dynamic_cast<GlobalGraphicsItem *>(currentItem);
            if(item!=nullptr)
            {
                removeItem(item);
                emit deleteGlobalItem(item);
            }
        }
    });

    connect(m_editItemAction,&QAction::triggered,this,[=]()
    {
        m_globalItemSettingView->show();
        Q_ASSERT(m_globalItemObj);
        QMetaObject::invokeMethod(m_globalItemObj,"setBuileName",Q_ARG(QVariant,currentBuildName()));
        QMetaObject::invokeMethod(m_globalItemObj,"setGlobalItemValue",Q_ARG(QVariant,currentItemSize()));
        QMetaObject::invokeMethod(m_globalItemObj,"setGlobalIcon",Q_ARG(QVariant,currentItemIcon()));
    });
    connect(m_goToAchitePlanAction,&QAction::triggered,this,[=]()
    {
        QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
        GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
        emit goToArchitePlan(item);
    });
    connect(m_clearItemAction,&QAction::triggered,this,&GlobalGraphicsScene::clearItem);
    Q_ASSERT(m_globalItemObj);
    connect(m_globalItemObj,SIGNAL(setItemValue(qreal)),this,SLOT(setCurrentItemSize(qreal)));



}

GlobalGraphicsScene::~GlobalGraphicsScene()
{
    m_menu->close();
    delete m_menu;
    delete m_globalItemSettingView;
}

void GlobalGraphicsScene::showMenu(const QPoint &point)
{
    if(Controller::instance()->getUserRight()!=UserManager::Super)
    {
        m_removeItemAction->setEnabled(false);
        m_editItemAction->setEnabled(false);
        m_removeSelectItemAction->setEnabled(false);
        m_clearItemAction->setEnabled(false);

    }
    else
    {
        m_removeItemAction->setEnabled(true);
        m_editItemAction->setEnabled(true);
        m_removeSelectItemAction->setEnabled(true);
        m_clearItemAction->setEnabled(true);

    }
    m_menu->exec(point);
}

GlobalGraphicsItem * GlobalGraphicsScene::addGlobalGraphicsItem(QPointF point)
{
    GlobalGraphicsItem *item = new GlobalGraphicsItem(this);
    m_num++;
    item->setBuildName(QString(tr("%1号楼")).arg(m_num));
    item->setPos(point);
    addItem(item);
    return item;
}

void GlobalGraphicsScene::clearGraphicsItem()
{
    QList<QGraphicsItem*> graphicsItemList= items();
    foreach (QGraphicsItem*item, graphicsItemList)
    {
        GlobalGraphicsItem *currentItem = dynamic_cast<GlobalGraphicsItem *>(item);
        if(currentItem!=nullptr)
        {
            removeItem(currentItem);
        }
    }
}

QString GlobalGraphicsScene::currentBuildName()
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        return item->buildName();
    }
    else
    {
        return "";
    }
}

void GlobalGraphicsScene::setCurrentBuildName(const QString &name)
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        item->setBuildName(name);
        emit setBuildingName(item,name);
    }
}

qreal GlobalGraphicsScene::currentItemSize()
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        return item->itemSize();
    }
    else
    {
        return 0.0;
    }
}

void GlobalGraphicsScene::setCurrentItemSize(qreal size)
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        item->setItemSize(size);
        update();
    }
}

QString GlobalGraphicsScene::currentItemIcon()
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        return item->iconName();
    }
    else
    {
        return "";
    }
}

void GlobalGraphicsScene::setCurrentItemIcon(const QString &icon)
{
    QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
    GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
    if(item!=nullptr)
    {
        QString currentIconName=  Controller::instance()->fileNameFromQml(icon);
        item->setIconName(currentIconName);
        update();
    }
}


void GlobalGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(Controller::instance()->getUserRight()==UserManager::Super)
        {
            GlobalGraphicsItem*item=  addGlobalGraphicsItem(event->scenePos());
            emit addGlobalItem(item);
        }
    }
}

void GlobalGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        m_pointF =event->scenePos();
    }
    QGraphicsScene::mousePressEvent(event);
}
