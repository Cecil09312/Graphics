#include "globalgraphicsitem.h"
#include <QSvgRenderer>
#include "control/controller.h"
#include <QDebug>
GlobalGraphicsItem::GlobalGraphicsItem(GlobalGraphicsScene *scene)
    :m_radius(30.0)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlags(ItemIsMovable|ItemIsSelectable);
    setAcceptHoverEvents(true);
    m_scene = scene;
    m_propertyAnimation =nullptr;
    m_iconName = ":/images/build.png";
    setProperty("size",m_radius);

    m_font.setPointSize(qFloor(m_radius/4));
    m_font.setFamily("Times New Roman");

}

GlobalGraphicsItem::~GlobalGraphicsItem()
{
    startAnimal(false);
    if(m_propertyAnimation!=nullptr)
    {
        m_propertyAnimation->deleteLater();
        m_propertyAnimation = nullptr;
    }

}

void GlobalGraphicsItem::setIconName(const QString &name)
{
    m_iconName = Controller::instance()->fileNameFromQml(name);
   // m_scene->update();
    Controller::instance()->getDrawImageThread()->drawImage(m_iconName);
    update();
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
    update();
}

void GlobalGraphicsItem::setHoverText(const QString &hoverText)
{
    m_hoverText = hoverText;
    update();
}

QString GlobalGraphicsItem::buildName()
{
    return m_buildName;
}

void GlobalGraphicsItem::setBuildName(const QString &name)
{
    m_buildName = name;
    update();
    //m_scene->update();

}

void GlobalGraphicsItem::startAnimal(bool isStart)
{

    if(m_propertyAnimation==nullptr)
    {
        m_propertyAnimation = new QPropertyAnimation(this,"size");
        m_propertyAnimation->setDuration(1200);
        m_propertyAnimation->setStartValue(0.5);
        m_propertyAnimation->setEndValue(2);
        m_propertyAnimation->setLoopCount(-1);

        connect(m_propertyAnimation,&QPropertyAnimation::valueChanged,this,[=](const QVariant &value)
        {
            if(m_scaleRunNum>=60)
            {
                m_scaleRunNum =0;
            }
            if(m_scaleRunNum%12==0)
            {
                qreal scale =qvariant_cast<qreal> (value);
                setScale(scale);
            }
            m_scaleRunNum++;
        });
    }

    if(m_propertyAnimation!=nullptr)
    {
        if(isStart)
        {
            m_propertyAnimation->start();
        }
        else
        {
            m_scaleRunNum =0;
            m_propertyAnimation->stop();
            QTransform currentTransform = transform();
            qreal xScale = currentTransform.m11();
            qreal yScale = currentTransform.m22();
            if(xScale*yScale>0)
            {
                if(qAbs(xScale-yScale)<=0.0001)
                {
                    setScale(1.0/xScale);
                }
                else
                {
                    currentTransform.scale(1.0/xScale,1.0/yScale);
                    setTransform(currentTransform);
                }
            }


        }
    }

}

bool GlobalGraphicsItem::animalIsRunning()
{
    if(m_propertyAnimation!=nullptr)
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
    else
    {
        return false;
    }


}

QString GlobalGraphicsItem::personOnDuty()
{
    return m_personOnDuty;
}

void GlobalGraphicsItem::setPersonOnDuty(const QString &name)
{
    m_personOnDuty = name;
}

QRectF GlobalGraphicsItem::boundingRect() const
{
    qreal penWidth = 1;
    if(m_radius>0)
    {
        return QRectF(-m_radius - penWidth / 2, -m_radius - penWidth / 2,
                      m_radius*2.7+ penWidth, m_radius*2 + penWidth);
    }
    else
    {

        return QRectF(-40 - penWidth / 2, -40 - penWidth / 2,
                      108+ penWidth, 80 + penWidth);
    }
}

void GlobalGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{

    QString curBuildName = m_buildName;

    m_font.setPointSize(qFloor(m_radius/4));
    painter->setFont(m_font);


    if(m_radius>0)
    {

        if(curBuildName.size()>8)
        {
            painter->drawText(QRectF(-m_radius,-m_radius,2.7*m_radius,2.7*m_radius),Qt::AlignHCenter,curBuildName);
        }
        else
        {
            painter->drawText(QRectF(-m_radius,-m_radius,2*m_radius,2*m_radius),Qt::AlignHCenter,curBuildName);
        }

        QRectF rectF = QRectF(-m_radius/2,-m_radius/2,m_radius,m_radius);
       // DrawImageThread drawImageThread;

        painter->drawImage(rectF,Controller::instance()->getDrawImageThread()->getImageFromName(m_iconName));
    }


    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::black, 0, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(0.4*m_radius,0.4*m_radius, -1.1*m_radius, -0.4*m_radius));
    }
}

void GlobalGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    UserManager::UserRight userRight =   Controller::instance()->getUserRight();
    if(userRight==UserManager::Super||userRight==UserManager::Administrator)
    {
        if(ArchitePlanView::itemLimit())
        {
            setPos(event->scenePos());
            m_scene->update();
        }

    }
}

void GlobalGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

    QString hoverText = QString(tr("建筑名称:%1")+ "\n"+ tr("值班人员:%2")+"\n"
                                +tr("大小:%3")).arg(m_buildName)
            .arg(m_personOnDuty)
            .arg(m_radius);
    setHoverText(hoverText);
    setToolTip(m_hoverText);
    event->accept();
}
