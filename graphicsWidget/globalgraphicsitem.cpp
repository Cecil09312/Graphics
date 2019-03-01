#include "globalgraphicsitem.h"
#include <QSvgRenderer>
#include "control/controller.h"
#include <QDebug>
GlobalGraphicsItem::GlobalGraphicsItem(GlobalGraphicsScene *scene)
    :m_radius(12.0)
{
    m_scene = scene;
    m_iconName = ":/images/fireAlarm.png";
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlags(ItemIsMovable|ItemIsSelectable);
}

GlobalGraphicsItem::~GlobalGraphicsItem()
{

}

void GlobalGraphicsItem::setIconName(const QString &name)
{
    m_iconName = name;
}

QString GlobalGraphicsItem::iconName()
{
    return m_iconName;
}

qreal GlobalGraphicsItem::itemSize()
{
    return m_radius;
}

void GlobalGraphicsItem::setItemSize(qreal radius)
{
    m_radius = radius;
    m_scene->update();
}

void GlobalGraphicsItem::setHoverText(const QString &hoverText)
{
    m_hoverText = hoverText;
}

QString GlobalGraphicsItem::buildName()
{
    return m_buildName;
}

void GlobalGraphicsItem::setBuildName(const QString &name)
{
    m_buildName = name;
}

QRectF GlobalGraphicsItem::boundingRect() const
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

void GlobalGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if(m_iconName.endsWith(".svg"))
    {
        QSvgRenderer renderer(m_iconName);
        renderer.render(painter,QRectF(-m_radius,-m_radius,m_radius*2,m_radius*2));
        //painter->drawText(QRect(-m_radius,-m_radius,m_radius,m_radius),m_itemInfo.m_deviceNum);
    }
    else
    {
        if(m_radius>0)
        {
            if(!QPixmap(m_iconName).isNull())
            {
                painter->drawPixmap(-m_radius,-m_radius,m_radius*2,m_radius*2,QPixmap(m_iconName));
            }
            //painter->drawText(QRect(-m_radius,-m_radius,m_radius,m_radius),m_itemInfo.m_deviceNum);
        }
        else
        {
            if(!QPixmap(m_iconName).isNull())
            {
                painter->drawPixmap(-12,-12,12,12,QPixmap(m_iconName));
            }
           // painter->drawText(QRectF(-12,-12,12,12),m_itemInfo.m_deviceNum);
        }


    }

    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::black, 0, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(m_radius*2, m_radius*2, -m_radius*2, -m_radius*2));
    }
}

void GlobalGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(Controller::instance()->getUserRight()==UserManager::Super)
    {
        setPos(event->scenePos());
        m_scene->update();
    }
}

void GlobalGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setToolTip(m_hoverText);
    event->accept();
}
