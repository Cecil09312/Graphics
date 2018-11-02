#include "graphicsscene.h"
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "graphicsWidget/graphicsitem.h"


GraphicsScene::GraphicsScene(QObject *parent):
    QGraphicsScene(parent)
{

}

void GraphicsScene::addGraphicsItem(qreal ax, qreal ay)
{
    addGraphicsItem(QPointF(ax,ay));
}

void GraphicsScene::addGraphicsItem(const QPointF &pointF)
{
    GraphicsItem *rectItem= new GraphicsItem(this);
    rectItem->setPos(pointF);
    this->addItem(rectItem);
    m_itemList.push_back(rectItem);
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
            if(graphicsItemList.at(i)==m_itemList.at(j))
            {
                removeItem(graphicsItemList.at(i));
                m_itemList.removeAt(j);
                break;
            }

        }
    }
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
        removeGraphicsItem(event->scenePos().x(),event->scenePos().y());

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





