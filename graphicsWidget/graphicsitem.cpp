#include "graphicsitem.h"
#include <QDebug>
#include "graphicsWidget/graphicsscene.h"
GraphicsItem::GraphicsItem(GraphicsScene *scene, QObject *parent):
    QObject(parent)
{
    m_graphicsScene = scene;
    setFlags(ItemIsMovable|ItemIsSelectable);
    m_color = QColor(Qt::green);
    setProperty("color",m_color);
    m_propertyAnimation = new QPropertyAnimation(this,"color");
    m_propertyAnimation->setStartValue(QColor(Qt::green));
    m_propertyAnimation->setEndValue(QColor(Qt::red));
    m_propertyAnimation->setDuration(1000);
    m_propertyAnimation->setLoopCount(-1);
   // qDebug() << m_propertyAnimation->currentValue();
   // m_propertyAnimation->start();
    connect(m_propertyAnimation,&QPropertyAnimation::valueChanged,this,[=](const QVariant &/*value*/)
    {
      QColor color =qvariant_cast<QColor> (m_propertyAnimation->currentValue());
       m_color = color;
       if(m_graphicsScene)
       {
           m_graphicsScene->update();
       }
    });

   setAcceptHoverEvents(true);

}

GraphicsItem::~GraphicsItem()
{
    if(m_propertyAnimation)
    {
        m_propertyAnimation->stop();

       m_propertyAnimation->deleteLater();
    }
}

QRectF GraphicsItem::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                  20 + penWidth, 20 + penWidth);

}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*/*option*/, QWidget */*widget*/)
{
    painter->setBrush(m_color);
    painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
}

void GraphicsItem::setColor(const QColor &color)
{
    m_color = color;
}

void GraphicsItem::startAnimation()
{
    m_propertyAnimation->start();
}

void GraphicsItem::stopAnimation()
{
    m_propertyAnimation->stop();
}

void GraphicsItem::setAnimationDuration(int duration)
{
    m_propertyAnimation->setDuration(duration);
}

void GraphicsItem::setAnimationLoopCount(int count)
{
    m_propertyAnimation->setLoopCount(count);
}

void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setPos(event->scenePos());
}

void GraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QString toolTip = QString("x:%1,y:%2").arg(event->scenePos().rx()).arg(event->scenePos().ry());
      setToolTip(toolTip);
      event->accept();
     // qDebug() << toolTip;
}


void GraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QString toolTip = QString("x:%1,y:%2").arg(event->scenePos().rx()).arg(event->scenePos().ry());
      setToolTip(toolTip);
      event->accept();

}

//void GraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
//{

//}
