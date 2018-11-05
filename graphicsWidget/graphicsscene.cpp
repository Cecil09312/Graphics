#include "graphicsscene.h"
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "graphicsWidget/graphicsitem.h"

QMap<QString,QList<QGraphicsItem*> > GraphicsScene::s_typeItemMap = QMap<QString,QList<QGraphicsItem*> >();
GraphicsScene::GraphicsScene(QObject *parent):
    QGraphicsScene(parent)
{
    m_menu = new QMenu;
    m_deleteAction = new QAction(tr("删除"),m_menu);
    m_editAction = new QAction(tr("编辑"),m_menu);
    m_clearAction = new QAction(tr("清空"),m_menu);
    m_deleteSelectedAction = new QAction(tr("删除选中"),m_menu);
    m_closeAction= new QAction(tr("关闭"),m_menu);
    m_menu->addAction(m_deleteAction);
    m_menu->addAction(m_editAction);
    m_menu->addAction(m_clearAction);
    m_menu->addAction(m_deleteSelectedAction);
    m_menu->addAction(m_closeAction);
    connect(m_deleteAction,&QAction::triggered,this,[=]()
    {
        removeGraphicsItem(m_currentPointF);
    });

    connect(m_editAction,&QAction::triggered,this,[=]()
    {

    });
    connect(m_clearAction,&QAction::triggered,this,[=]()
    {
        foreach (QGraphicsItem*item, m_itemList)
        {
            removeItem(item);
        }
        m_itemList.clear();
        s_typeItemMap.clear();
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
                QList<QString>keyList= s_typeItemMap.keys();
                foreach (const QString &key, keyList)
                {
                    QList<QGraphicsItem*> itemValueList = s_typeItemMap[key];
                    foreach (QGraphicsItem*valueItem, itemValueList)
                    {
                        if(valueItem==item)
                        {
                            s_typeItemMap[key].removeOne(item);
                        }
                    }
                }
            }

        }
    });
    connect(m_closeAction,&QAction::triggered,this,[=]()
    {
        m_menu->close();
    });
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
    s_typeItemMap[tr("火警")].push_back(item);
}

void GraphicsScene::removeGraphicsItem(qreal ax, qreal ay)
{
    removeGraphicsItem(QPointF(ax,ay));
}

void GraphicsScene::removeGraphicsItem(const QPointF &pointF)
{
    QList<QGraphicsItem*>graphicsItemList= items(pointF,Qt::IntersectsItemShape, Qt::DescendingOrder, QTransform());
    for(int i=0;i<graphicsItemList.size();i++)
    {
        for(int j=0;j<m_itemList.size();j++)
        {
            QGraphicsItem* item = graphicsItemList.at(i);
            if(item==m_itemList.at(j))
            {
                removeItem(graphicsItemList.at(i));
                m_itemList.removeAt(j);
                s_typeItemMap[tr("火警")].removeOne(item);
                break;
            }

        }
    }
}

void GraphicsScene::showMenu(const QPoint &point)
{
    m_menu->exec(point);
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
        QGraphicsScene::mousePressEvent(event);
    }

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

QList<QGraphicsItem *> GraphicsScene::getTypeItemList(const QString &type)
{
    return s_typeItemMap[type];
}





