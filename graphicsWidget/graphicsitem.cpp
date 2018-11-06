#include "graphicsitem.h"
#include <QDebug>
#include "graphicsWidget/graphicsscene.h"
#include <QStyleOptionGraphicsItem>
GraphicsItem::GraphicsItem(GraphicsScene *scene, QObject *parent):
    QObject(parent),
    m_radius(12.0)

{
    m_graphicsScene = scene;
    setFlags(ItemIsMovable|ItemIsSelectable);
    m_color = QColor(Qt::green);
    setProperty("color",m_color);
    setProperty("scale",m_radius);
    m_colorAnimation = new QPropertyAnimation(this,"color");
    m_colorAnimation->setStartValue(QColor(Qt::black));
    m_colorAnimation->setEndValue(QColor(Qt::red));
    m_colorAnimation->setDuration(1000);
    m_scaleAnimation = new QPropertyAnimation(this,"scale");
    m_scaleAnimation->setStartValue(0.5);
    m_scaleAnimation->setEndValue(1.2);
    m_scaleAnimation->setDuration(1000);

    m_parallelAnimGroup = new QParallelAnimationGroup(this);
    m_parallelAnimGroup->addAnimation(m_colorAnimation);
    m_parallelAnimGroup->addAnimation(m_scaleAnimation);
    m_parallelAnimGroup->setLoopCount(-1);


   // m_parallelAnimGroup->setDirection(1000);
   m_parallelAnimGroup->start();
    connect(m_colorAnimation,&QPropertyAnimation::valueChanged,this,[=](const QVariant &/*value*/)
    {
        QColor color =qvariant_cast<QColor> (m_colorAnimation->currentValue());
        m_color = color;
        if(m_graphicsScene)
        {
            m_graphicsScene->update();
        }
    });

    connect(m_scaleAnimation,&QPropertyAnimation::valueChanged,this,[=](const QVariant &/*value*/)
    {
        qreal scale =qvariant_cast<qreal> (m_scaleAnimation->currentValue());
        setScale(scale);
        if(m_graphicsScene)
        {
            m_graphicsScene->update();
        }
    });

    setAcceptHoverEvents(true);
    m_itemText = QString("%1").arg(qrand()%256);

}

GraphicsItem::~GraphicsItem()
{
    if(m_parallelAnimGroup)
    {
        m_parallelAnimGroup->stop();
    }
}

QRectF GraphicsItem::boundingRect() const
{
    qreal penWidth = 1;
    if(m_radius>0)
    {
        return QRectF(-m_radius - penWidth / 2, -m_radius - penWidth / 2,
                      m_radius*2 + penWidth, m_radius*2 + penWidth);
    }
    else
    {

        return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                      20 + penWidth, 20 + penWidth);
    }

}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*option, QWidget */*widget*/)
{
    painter->setBrush(m_color);
//     QPen pen;
//     pen.setWidth(0);
//     pen.setColor(Qt::red);
//     painter->setPen(pen);
    if(m_radius>0)
    {
        //pen.setColor(Qt::red);
        painter->drawEllipse(QPointF(0,0),m_radius,m_radius);
        painter->drawText(QPointF(-m_radius-5,-m_radius-5),m_itemText);

    }
    else
    {
        painter->drawEllipse(QPointF(0,0),12,12);
        painter->drawText(QPointF(-17,-17),m_itemText);
    }

   if (option->state & QStyle::State_Selected)
   {
       painter->setPen(QPen(Qt::black, 0, Qt::DashLine));
       painter->setBrush(Qt::NoBrush);
       painter->drawRect(boundingRect().adjusted(m_radius*2, m_radius*2, -m_radius*2, -m_radius*2));

   }
       //   qt_graphicsItem_highlightSelected(this, painter, option);
    m_graphicsScene->update();

   // painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
}

void GraphicsItem::setColor(const QColor &color)
{
    m_color = color;
}

void GraphicsItem::startAnimation()
{
    m_parallelAnimGroup->start();
}

void GraphicsItem::stopAnimation()
{
    m_parallelAnimGroup->stop();
}

void GraphicsItem::setAnimationDuration(int duration)
{
    m_colorAnimation->setDuration(duration);
    m_scaleAnimation->setDuration(duration);
}

void GraphicsItem::setAnimationLoopCount(int count)
{
    m_parallelAnimGroup->setLoopCount(count);
}

QPointF GraphicsItem::graphicsItemPos() const
{
    return  this->pos();
}

void GraphicsItem::setColorStartValue(const QVariant &value)
{
    m_colorAnimation->setStartValue(value);
}

void GraphicsItem::setColorEndValue(const QVariant &value)
{
    m_colorAnimation->setEndValue(value);
}

void GraphicsItem::setScaleStartValue(const QVariant &value)
{
    m_scaleAnimation->setStartValue(value);
}

void GraphicsItem::setScaleEndValue(const QVariant &value)
{
    m_scaleAnimation->setEndValue(value);
}



QColor GraphicsItem::color() const
{
    return m_color;
}

qreal GraphicsItem::radius() const
{
    return m_radius;
}

void GraphicsItem::setRadius(qreal radius)
{
    m_radius = radius;
}

QString GraphicsItem::hoverText() const
{
    return m_hoverText;
}

void GraphicsItem::setHoverText(const QString &hoverText)
{
    m_hoverText = hoverText;
}

QString GraphicsItem::itemText() const
{
    return m_itemText;
}

void GraphicsItem::setItemText(const QString &itemText)
{
    m_itemText = itemText;
}

QColor GraphicsItem::itemTextColor() const
{
    return m_itemTextColor;
}

void GraphicsItem::setItemTextColor(const QColor &color)
{
    m_itemTextColor = color;
}


void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setPos(event->scenePos());
    m_graphicsScene->update();
}

void GraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hoverText = QString("x:%1,y:%2").arg(event->scenePos().rx()).arg(event->scenePos().ry());
    setToolTip(m_hoverText);
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
