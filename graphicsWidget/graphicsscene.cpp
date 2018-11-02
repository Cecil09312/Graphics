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

QList<QGraphicsItem *> GraphicsScene::getTypeItemList(const QString &type)
{
    return s_typeItemMap[type];
}





