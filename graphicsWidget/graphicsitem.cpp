#include "graphicsitem.h"
#include <QDebug>
#include "graphicsWidget/graphicsscene.h"
#include <QStyleOptionGraphicsItem>
#include "control/controller.h"
#include "jsonEdit/itemiconinfotojson.h"
#include "control/controller.h"
#include <QSvgGenerator>
#include <QGraphicsView>
int GraphicsItem::m_num =1;
GraphicsItem::GraphicsItem(GraphicsScene *scene):
    m_radius(20.0)

{
    m_graphicsScene = scene;
    m_itemInfo.m_currentState = tr("正常");
    m_itemInfo.m_manufacturers = tr("北京利达华信电子有限公司");
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlags(ItemIsMovable|ItemIsSelectable);
    m_colorEffect = new QGraphicsColorizeEffect(this);
    setAcceptHoverEvents(true);
    setGraphicsEffect(m_colorEffect);
    m_colorEffect->setStrength(0.0);
    setProperty("color",m_color);
    setProperty("scale",m_radius);
    m_colorAnimation = new QPropertyAnimation(this,"color");
    m_colorAnimation->setStartValue(QColor(Qt::black));
    m_colorAnimation->setEndValue(QColor(Qt::red));
    m_colorAnimation->setDuration(500);
    m_colorAnimation->setLoopCount(-1);
    m_scaleAnimation = new QPropertyAnimation(this,"scale");
    m_scaleAnimation->setStartValue(0.3);
    m_scaleAnimation->setEndValue(1.2);
    m_scaleAnimation->setDuration(1000);
    m_scaleAnimation->setLoopCount(-1);


    m_parallelAnimGroup = new QParallelAnimationGroup(this);
    m_parallelAnimGroup->addAnimation(m_colorAnimation);
    m_parallelAnimGroup->addAnimation(m_scaleAnimation);
    m_parallelAnimGroup->setLoopCount(-1);

    m_itemTextFont.setPointSize(12);
    m_itemTextFont.setFamily("Times New Roman");

    m_itemInfo.m_deviceNum = QString("%1").arg(m_num++);
    int itemIconIndex = ItemIconInfoToJson::currentIconIndex();
    QHash<QString,QVariant>itemIconInfoHash = m_itemIconInfoToJson.getIconInfoHash();
    if(itemIconInfoHash.size()>0)
    {
        QHash<QString,QVariant> deviceHash=  itemIconInfoHash[QString("%1").arg(itemIconIndex)].toHash();
        m_itemInfo.m_equipmentModel= deviceHash["deviceName"].toString();
        m_itemInfo.m_manufacturers = deviceHash["manufacturers"].toString();
        m_itemInfo.m_periodOfValidity = deviceHash["periodOfvalidity"].toString();

    }
    else
    {
        m_itemInfo.m_equipmentModel ="";
        m_itemIconInfoToJson.setCurrentIconIndex(-1);
    }
    if(itemIconIndex>=0)
    {
        QString currentIconName = ItemIconInfoToJson::getIconName(itemIconIndex);
        if(!currentIconName.isEmpty())
        {
            m_iconName = Controller::instance()->fileNameFromQml(currentIconName);
        }
        else
        {
            m_itemIconInfoToJson.setCurrentIconIndex(-1);
        }

    }
    else
    {
        m_itemIconInfoToJson.setCurrentIconIndex(-1);
    }

    connect(m_colorAnimation,&QPropertyAnimation::valueChanged,this,[=](const QVariant &/*value*/)
    {
        QColor color =qvariant_cast<QColor> (m_colorAnimation->currentValue());
        m_color = color;
        m_colorEffect->setColor(m_color);

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

}

GraphicsItem::~GraphicsItem()
{
    stopAnimations();
    stopColorAnimation();
    stopScaleAnimation();
}

QRectF GraphicsItem::boundingRect() const
{
    qreal penWidth = 1;
    if(m_radius>0)
    {
        return QRectF(-m_radius - penWidth / 2, -m_radius - penWidth / 2,
                      m_radius*2.5 + penWidth, m_radius*2.5 + penWidth);
    }
    else
    {

        return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                      25 + penWidth, 25 + penWidth);
    }

}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*option, QWidget */*widget*/)
{


    painter->setFont(m_itemTextFont);
    if(m_iconName.endsWith(".svg"))
    {
        QSvgRenderer renderer(m_iconName);
        renderer.render(painter,QRectF(-m_radius/1.25,-m_radius/1.25,m_radius*2,m_radius*2));
        painter->drawText(QRect(-m_radius,-m_radius,m_radius,m_radius),m_itemInfo.m_deviceNum);
    }
    else
    {
        if(m_radius>0)
        {
            if(!QPixmap(m_iconName).isNull())
            {
                painter->drawPixmap(-m_radius/1.25,-m_radius/1.25,m_radius*2,m_radius*2,QPixmap(m_iconName));
            }
            painter->drawText(QRect(-m_radius,-m_radius,m_radius,m_radius),m_itemInfo.m_deviceNum);
        }
        else
        {
            if(!QPixmap(m_iconName).isNull())
            {
                painter->drawPixmap(-20/1.25,-20/1.25,20,20,QPixmap(m_iconName));
            }
            painter->drawText(QRectF(-20,-20,20,20),m_itemInfo.m_deviceNum);
        }
    }

    if(!m_itemInfo.m_periodOfValidity.isEmpty()&&QDate::fromString(m_itemInfo.m_periodOfValidity,"yyyy/MM/dd")<=QDate::currentDate())
    {
        painter->setPen(QPen(Qt::red));
        painter->setBrush(Qt::red);
        painter->drawEllipse(m_radius,m_radius,m_radius/4,m_radius/4);
    }

    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::black, 0, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(m_radius*2.5, m_radius*2.5, -m_radius*2.5, -m_radius*2.5));
    }
}

void GraphicsItem::setColor(const QColor &color)
{
    m_color = color;
}

void GraphicsItem::startAnimations()
{
    m_colorEffect->setStrength(1.0);
    m_parallelAnimGroup->start();
}

void GraphicsItem::stopAnimations()
{
    m_parallelAnimGroup->stop();
}

void GraphicsItem::startColorAnimation()
{
    setColorEffectStrength(1.0);
    m_colorAnimation->start();
}

void GraphicsItem::stopColorAnimation()
{
    m_colorAnimation->stop();
}

void GraphicsItem::startScaleAnimation()
{
    m_scaleAnimation->start();
}

void GraphicsItem::stopScaleAnimation()
{
    m_scaleAnimation->stop();
}

void GraphicsItem::setColorEffectStrength(qreal strength)
{
    m_colorEffect->setStrength(strength);
}

void GraphicsItem::setAnimationDuration(int duration)
{
    m_colorAnimation->setDuration(duration/2);
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

void GraphicsItem::setColorEffectValue(qreal value)
{
    m_colorEffect->setStrength(value);
}

void GraphicsItem::restoreSize()
{
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
        update();
    }
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
    update();
}

QString GraphicsItem::hoverText() const
{
    return m_hoverText;
}

void GraphicsItem::setHoverText(const QString &hoverText)
{
    m_hoverText = hoverText;
    update();
}

QColor GraphicsItem::itemTextColor() const
{
    return m_itemTextColor;
}

void GraphicsItem::setItemTextColor(const QColor &color)
{
    m_itemTextColor = color;
    update();
}

QString GraphicsItem::iconName() const
{
    return m_iconName;
}

void GraphicsItem::setIconName(const QString &iconName)
{
    m_iconName = iconName;
    update();
}

QHash<QString, QVariant> GraphicsItem::itemInfo()
{
    QHash<QString, QVariant> itemHash;
    itemHash["extNum"] = m_itemInfo.m_extNum;
    itemHash["loopNum"] = m_itemInfo.m_loopNum;
    itemHash["addrNum"] = m_itemInfo.m_addrNum;
    itemHash["deviceNum"] = m_itemInfo.m_deviceNum;
    itemHash["equipmentModel"] = m_itemInfo.m_equipmentModel;
    itemHash["currentState"] = m_itemInfo.m_currentState;
    itemHash["sysOfDevice"] = m_itemInfo.m_sysOfDevice;
    itemHash["deviceLocation"] = m_itemInfo.m_deviceLocation;
    itemHash["buildingName"] = m_itemInfo.m_buildingName;
    itemHash["floorOfDevice"] = m_itemInfo.m_floorOfDevice;
    itemHash["manufacturers"] = m_itemInfo.m_manufacturers;
    itemHash["periodOfValidity"] = m_itemInfo.m_periodOfValidity;
    itemHash["iconName"] = m_iconName;
    itemHash["size"] = m_radius;
    itemHash["pos"] = QString("%1,%2").arg(scenePos().x()).arg(scenePos().y());
    itemHash["operator"] = m_itemInfo.m_deviceOperator;
    return itemHash;
}

void GraphicsItem::setItemInfo(const ItemInfo &itemInfo)
{
    m_itemInfo = itemInfo;
}

ItemInfo &GraphicsItem::getItemInfo()
{
    return m_itemInfo;
}

void GraphicsItem::setPeriodOfValidity(const QString &period)
{
    m_itemInfo.m_periodOfValidity = period;
    update();
}

QString &GraphicsItem::extNum()
{
    return m_itemInfo.m_extNum;
}

QString &GraphicsItem::loopNum()
{
    return m_itemInfo.m_loopNum;
}

QString &GraphicsItem::addrNum()
{
    return m_itemInfo.m_addrNum;
}

QString &GraphicsItem::currentState()
{
    return m_itemInfo.m_currentState;
}

QString &GraphicsItem::deviceNum()
{
    return m_itemInfo.m_deviceNum;
}

QString &GraphicsItem::equipmentModel()
{
    return m_itemInfo.m_equipmentModel;
}

QString &GraphicsItem::sysOfDevice()
{
    return m_itemInfo.m_sysOfDevice;
}

//QString &GraphicsItem::protectedAreaName()
//{
//    return m_itemInfo.m_protectedAreaName;
//}

QString &GraphicsItem::buildingName()
{
    return m_itemInfo.m_buildingName;
}

QString &GraphicsItem::floorOfDevice()
{
    return m_itemInfo.m_floorOfDevice;
}

QString &GraphicsItem::deviceLocation()
{
    return m_itemInfo.m_deviceLocation;
}

QString &GraphicsItem::manufacturers()
{
    return m_itemInfo.m_manufacturers;
}

QString &GraphicsItem::periodOfValidity()
{
    return m_itemInfo.m_periodOfValidity;
}

QString &GraphicsItem::deviceOperator()
{
    return m_itemInfo.m_deviceOperator;
}

QString &GraphicsItem::alarmType()
{
    return m_itemInfo.m_alarmType;
}



void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(Controller::instance()->getUserRight()==UserManager::Super)
    {
        setPos(event->scenePos());
        m_graphicsScene->update();
    }

}

void GraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //Q_UNUSED(event)
    updateHoverText();
    setToolTip(m_hoverText);
    event->accept();
}

void GraphicsItem::updateHoverText()
{
    QString hoverText = QString("分机号:%1\n"
                                "回路号:%2\n"
                                "地址号:%3\n"
                                "设备编码:%4\n"
                                "设备:%5\n"
                                "报警状态:%6\n"
                                "报警时间:%7\n"
                                "系统:%8\n"
                                "建筑名称:%9\n"
                                "楼层:%10\n"
                                "位置:%11\n"
                                "制造商:%12\n"
                                "有效期:%13\n"
                                "操作员:%14").arg(m_itemInfo.m_extNum).arg(m_itemInfo.m_loopNum).arg(m_itemInfo.m_addrNum)
            .arg(m_itemInfo.m_deviceNum).arg(m_itemInfo.m_equipmentModel).arg(m_itemInfo.m_currentState)
            .arg(m_itemInfo.m_alarmTime).arg(m_itemInfo.m_sysOfDevice).arg(m_itemInfo.m_buildingName)
            .arg(m_itemInfo.m_floorOfDevice).arg(m_itemInfo.m_deviceLocation).arg(m_itemInfo.m_manufacturers)
            .arg(m_itemInfo.m_periodOfValidity).arg(m_itemInfo.m_deviceOperator);
    setHoverText(hoverText);
}

