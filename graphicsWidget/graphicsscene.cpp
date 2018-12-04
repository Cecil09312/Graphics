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
       if(currentItem!=nullptr)
       {

        QMetaObject::invokeMethod(m_itemSettingObj,"setSelectColor",Q_ARG(QVariant,currentItem->color()));
        QMetaObject::invokeMethod(m_itemSettingObj,"setIconName",Q_ARG(QVariant,currentItem->iconName()));
        QMetaObject::invokeMethod(m_itemSettingObj,"setItemText",Q_ARG(QVariant,currentItem->itemText()));
        QMetaObject::invokeMethod(m_itemSettingObj,"setItemType",Q_ARG(QVariant,currentItem->typeName()));
        QMetaObject::invokeMethod(m_itemSettingObj,"setItemSize",Q_ARG(QVariant,currentItem->radius()));
        QMetaObject::invokeMethod(m_itemSettingObj,"setUseIcon",Q_ARG(QVariant,currentItem->isUseIcon()));
        QMetaObject::invokeMethod(m_itemSettingObj,"setItemGeoInfo",Q_ARG(QVariant,currentItem->geoInfo()));

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
        QList<QGraphicsItem*>itemList =m_itemList;
        foreach (QGraphicsItem*item,itemList)
        {
            if(item->isSelected())
            {
                removeItem(item);
                m_itemList.removeOne(item);
                Controller::instance()->getDataStore()->deleteTypeItem(item);
            }

        }
    });
    connect(m_closeAction,&QAction::triggered,this,[=]()
    {
        m_menu->close();
    });

    Q_ASSERT(m_itemSettingObj);
    connect(m_itemSettingObj,SIGNAL(selectColor(QColor)),this,SLOT(setItemColor(QColor)));
    connect(m_itemSettingObj,SIGNAL(setSize(qreal)),this,SLOT(setItemSize(qreal)));
    connect(m_itemSettingObj,SIGNAL(setText(QString)),this,SLOT(setItemText(QString)));
    connect(m_itemSettingObj,SIGNAL(setGeoInfo(QString)),this,SLOT(setItemGeoInfo(QString)));
    connect(m_itemSettingObj,SIGNAL(setTypeName(QString)),this,SLOT(setItemTypeName(QString)));
    connect(m_itemSettingObj,SIGNAL(setIcon(QString)),this,SLOT(setItemIcon(QString)));
    connect(m_itemSettingObj,SIGNAL(setIsUseIcon(bool)),this,SLOT(setItemUseIcon(bool)));
}

GraphicsScene::~GraphicsScene()
{
    delete m_menu;
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
        m_itemList.removeOne(currentItem);
        removeItem(currentItem);
        Controller::instance()->getDataStore()->deleteTypeItem(currentItem);
    }
}

void GraphicsScene::showMenu(const QPoint &point)
{
    m_menu->exec(point);
}

QPointF GraphicsScene::currentScenePos()
{
    return m_currentPointF;
}

void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        addGraphicsItem(event->scenePos().x(),event->scenePos().y());
    }
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button()==Qt::RightButton)
    {
        m_currentPointF =event->scenePos();
    }
    else
    {

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

void GraphicsScene::setItemText(QString txt)
{
    GraphicsItem *currentItem=dynamic_cast<GraphicsItem *> (itemAt(m_currentPointF,QTransform()));
    if(currentItem!=nullptr)
    {
        currentItem->setItemText(txt);
    }
    update();
}

void GraphicsScene::setItemGeoInfo(QString geoInfo)
{
    GraphicsItem *currentItem=dynamic_cast<GraphicsItem *> (itemAt(m_currentPointF,QTransform()));
    if(currentItem!=nullptr)
    {
        currentItem->setGeoInfo(geoInfo);
    }
    update();
}

void GraphicsScene::setItemTypeName(QString name)
{
    GraphicsItem *currentItem=dynamic_cast<GraphicsItem *> (itemAt(m_currentPointF,QTransform()));
    if(currentItem!=nullptr)
    {
        currentItem->setTypeName(name);
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

void GraphicsScene::setItemUseIcon(bool isUseIcon)
{
    GraphicsItem *currentItem=dynamic_cast<GraphicsItem *> (itemAt(m_currentPointF,QTransform()));
    if(currentItem!=nullptr)
    {
        currentItem->setIsUseIcon(isUseIcon);
    }
    update();
}

void GraphicsScene::init()
{
    m_menu = new QMenu;
    m_deleteAction = new QAction(tr("删除"),m_menu);
    m_editAction = new QAction(tr("编辑"),m_menu);
    m_clearAction = new QAction(tr("清空"),m_menu);
    m_deleteSelectedAction = new QAction(tr("删除选中"),m_menu);
    m_closeAction= new QAction(tr("关闭"),m_menu);
    m_itemSettingView = new QQuickView;
    m_itemSettingView->setSource(QUrl("qrc:/qml/GraphicsItemEditor.qml"));
    m_itemSettingObj= m_itemSettingView->rootObject();
    m_menu->addAction(m_deleteAction);
    m_menu->addAction(m_editAction);
    m_menu->addAction(m_clearAction);
    m_menu->addAction(m_deleteSelectedAction);
    m_menu->addAction(m_closeAction);
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
        item->setItemText(itemHash["text"].toString());
        item->setIsUseIcon(itemHash["isUseIcon"].toBool());
        item->setTypeName(itemHash["typeName"].toString());
        item->setColor(QColor(itemHash["color"].toString()));
        item->setIconName(itemHash["iconName"].toString());
        item->setRadius(itemHash["size"].toDouble());
        item->setGeoInfo(itemHash["geoInfo"].toString());
        item->setHoverText(itemHash["hoverText"].toString());
        item->setPos(pos.section(",",0,0).toDouble(),pos.section(",",1,1).toDouble());
        addItem(item);
        m_itemList.push_back(item);
    }
}







