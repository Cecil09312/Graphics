#include "globalgraphicsitem.h"
#include <QSvgRenderer>
#include "control/controller.h"
#include <QDebug>
GlobalGraphicsItem::GlobalGraphicsItem(GlobalGraphicsScene *scene)
    :m_radius(50.0)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlags(ItemIsMovable|ItemIsSelectable);
    m_scene = scene;
    m_iconName = ":/images/build.png";
    setProperty("size",m_radius);
    m_propertyAnimation = new QPropertyAnimation(this,"size");
    m_propertyAnimation->setDuration(1000);
    m_propertyAnimation->setStartValue(m_radius*0.5);
    m_propertyAnimation->setEndValue(1.5*m_radius);
    m_propertyAnimation->setLoopCount(-1);
    connect(m_propertyAnimation,&QPropertyAnimation::valueChanged,this,[=](const QVariant &value)
    {
        setItemSize(value.toDouble());
    });

    connect(m_propertyAnimation,&QPropertyAnimation::stateChanged,this,[=](QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
    {
        if(oldState==QAbstractAnimation::Running&&newState==QAbstractAnimation::Stopped)
        {
            setItemSize(50.0);
        }
    });

}

GlobalGraphicsItem::~GlobalGraphicsItem()
{
    startAnimal(false);
}

void GlobalGraphicsItem::setIconName(const QString &name)
{
    m_iconName = name;
    m_scene->update();
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
    m_scene->update();

}

void GlobalGraphicsItem::startAnimal(bool isStart)
{
    if(isStart)
    {
        m_propertyAnimation->start();
    }
    else
    {
        m_propertyAnimation->stop();
    }
}

bool GlobalGraphicsItem::animalIsRunning()
{
    if(m_propertyAnimation->state()==QAbstractAnimation::Running)
    {
        return true;
    }
    else
    {
        return false;
    }

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
    QFont font;
    font.setPointSize(14);
    painter->setFont(font);
    if(m_iconName.endsWith(".svg"))
    {
        QSvgRenderer renderer(m_iconName);
        renderer.render(painter,QRectF(-m_radius,-m_radius,m_radius*2,m_radius*2));
        painter->drawText(QRect(-m_radius,-m_radius,m_radius,m_radius),m_buildName);
    }
    else
    {
        if(m_radius>0)
        {
            if(!QPixmap(m_iconName).isNull())
            {
                painter->drawPixmap(-m_radius,-m_radius,m_radius*2,m_radius*2,QPixmap(m_iconName));
            }
            painter->drawText(QRect(-m_radius,-m_radius,m_radius,m_radius),m_buildName);
        }
        else
        {
            if(!QPixmap(m_iconName).isNull())
            {
                painter->drawPixmap(-50,-50,50,50,QPixmap(m_iconName));
            }
             painter->drawText(QRectF(-50,-50,50,50),m_buildName);
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
