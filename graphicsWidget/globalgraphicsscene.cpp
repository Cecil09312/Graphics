#include "globalgraphicsscene.h"
#include "control/controller.h"
#include "globalgraphicsitem.h"
#include <QDebug>
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

    connect(m_editItemAction,&QAction::triggered,this,&GlobalGraphicsScene::editItem);
    connect(m_goToAchitePlanAction,&QAction::triggered,this,[=]()
    {
        QGraphicsItem *graphicsItem =  itemAt(m_pointF,QTransform());
        GlobalGraphicsItem *item = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
        emit goToArchitePlan(item);
    });
    connect(m_clearItemAction,&QAction::triggered,this,&GlobalGraphicsScene::clearItem);

}

GlobalGraphicsScene::~GlobalGraphicsScene()
{
    m_menu->close();
    delete m_menu;
}

void GlobalGraphicsScene::showMenu(const QPoint &point)
{
    if(Controller::instance()->getUserRight()!=UserManager::Super)
    {
        m_removeItemAction->setEnabled(false);
        m_editItemAction->setEnabled(false);
        m_goToAchitePlanAction->setEnabled(false);
    }
    else
    {
        m_removeItemAction->setEnabled(true);
        m_editItemAction->setEnabled(true);
        m_goToAchitePlanAction->setEnabled(true);
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
