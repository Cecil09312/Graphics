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

void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button()==Qt::LeftButton)
    {
        GraphicsItem *rectItem= new GraphicsItem(this);
        rectItem->setPos(event->scenePos().x(),event->scenePos().y());
        this->addItem(rectItem);
        m_itemList.push_back(rectItem);
    }

}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        QList<QGraphicsItem*>graphicsItemList= items(event->scenePos().x(),event->scenePos().y(),10,10,Qt::IntersectsItemShape, Qt::DescendingOrder, QTransform());
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
    else
    {
        QGraphicsScene::mousePressEvent(event);
    }

}





