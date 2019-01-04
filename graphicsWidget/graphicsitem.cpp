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
    m_radius(12.0)

{
    m_graphicsScene = scene;
    m_itemInfo.m_alarmType = tr("火警");
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlags(ItemIsMovable|ItemIsSelectable);
    // m_color = QColor(Qt::red);
    m_colorEffect = new QGraphicsColorizeEffect(this);
    setGraphicsEffect(m_colorEffect);
    m_colorEffect->setStrength(0.0);
    setProperty("color",m_color);
    setProperty("scale",m_radius);
    m_colorAnimation = new QPropertyAnimation(this,"color");
    m_colorAnimation->setStartValue(QColor(Qt::transparent));
    m_colorAnimation->setEndValue(QColor(Qt::red));
    m_colorAnimation->setDuration(1000);
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

    m_itemTextFont.setPointSize(6);
    m_itemTextFont.setFamily("Times New Roman");
    setAcceptHoverEvents(true);
    m_itemInfo.m_deviceNum = QString("%1").arg(m_num++);
    int itemIconIndex = ItemIconInfoToJson::currentIconIndex();
    QHash<QString,QVariant>itemIconInfoHash = m_itemIconInfoToJson.getIconInfoHash();
    if(itemIconInfoHash.size()>0)
    {
        QHash<QString,QVariant> deviceNameHash=  itemIconInfoHash[QString("%1").arg(itemIconIndex)].toHash();
        m_itemInfo.m_equipmentModel= deviceNameHash["deviceName"].toString();
    }
    else
    {
        m_itemInfo.m_equipmentModel ="";
        m_itemIconInfoToJson.setCurrentIconIndex(-1);
    }
    if(itemIconIndex>=0)
    {
        QString currentIconName = ItemIconInfoToJson::getIconName(itemIconIndex);
      // qDebug() <<"currentIconName"<< currentIconName;
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

    // m_parallelAnimGroup->setDirection(1000);
    // m_parallelAnimGroup->start();
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

    painter->setFont(m_itemTextFont);
    if(m_iconName.endsWith(".svg"))
    {
        QSvgRenderer renderer(m_iconName);
        renderer.render(painter,QRectF(-m_radius,-m_radius,m_radius*2,m_radius*2));
        // painter->drawRect(QRect(-m_radius*2,-m_radius*2,30,30));
        painter->drawText(QRect(-m_radius,-m_radius,m_radius,m_radius),m_itemInfo.m_deviceNum);
    }
    else
    {
        if(m_radius>0)
        {
            //            pen.setColor(Qt::red);
            //            painter->setPen(pen);
            if(!QPixmap(m_iconName).isNull())
            {
                painter->drawPixmap(-m_radius,-m_radius,m_radius*2,m_radius*2,QPixmap(m_iconName));

            }

            painter->drawText(QRect(-m_radius,-m_radius,m_radius,m_radius),m_itemInfo.m_deviceNum);
        }
        else
        {
            if(!QPixmap(m_iconName).isNull())
            {
                painter->drawPixmap(-12,-12,12,12,QPixmap(m_iconName));
            }

            painter->drawText(QRectF(-12,-12,12,12),m_itemInfo.m_deviceNum);
        }


    }

    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::black, 0, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(m_radius*2, m_radius*2, -m_radius*2, -m_radius*2));
    }
    //   qt_graphicsItem_highlightSelected(this, painter, option);
    //m_graphicsScene->update();

    // painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
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

void GraphicsItem::restoreSize()
{
    QTransform currentTransform = transform();
    qreal xScale = currentTransform.m11();
    qreal yScale = currentTransform.m22();
    if(xScale*yScale>0)
    {
        setTransform(currentTransform.scale(1/xScale,1/yScale));
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
    itemHash["alarmType"] = m_itemInfo.m_alarmType;
    itemHash["deviceNum"] = m_itemInfo.m_deviceNum;
    itemHash["equipmentModel"] = m_itemInfo.m_equipmentModel;
    itemHash["currentAlarmState"] = m_itemInfo.m_currentAlarmState;
    itemHash["alarmTime"] = m_itemInfo.m_alarmTime;
    itemHash["alarmReceiveTime"] = m_itemInfo.m_alarmReceiveTime;
    itemHash["alarmReplyTime"] = m_itemInfo.m_alarmReplyTime;
    itemHash["sysOfDevice"] = m_itemInfo.m_sysOfDevice;
    itemHash["protectedAreaName"] = m_itemInfo.m_protectedAreaName;
    itemHash["deviceLocation"] = m_itemInfo.m_deviceLocation;
    itemHash["buildingName"] = m_itemInfo.m_buildingName;
    itemHash["floorOfDevice"] = m_itemInfo.m_floorOfDevice;
    itemHash["operatorOnDuty"] = m_itemInfo.m_operatorOnDuty;
    itemHash["iconName"] = m_iconName;
    itemHash["size"] = m_radius;
    itemHash["pos"] = QString("%1,%2").arg(scenePos().x()).arg(scenePos().y());
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

QString GraphicsItem::extNum()
{
    return m_itemInfo.m_extNum;
}

QString GraphicsItem::loopNum()
{
    return m_itemInfo.m_loopNum;
}

QString GraphicsItem::addrNum()
{
    return m_itemInfo.m_addrNum;
}

QString GraphicsItem::alarmType()
{
    return m_itemInfo.m_alarmType;
}

QString GraphicsItem::deviceNum()
{
    return m_itemInfo.m_deviceNum;
}

QString GraphicsItem::equipmentModel()
{
    qDebug() << m_itemInfo.m_equipmentModel;
    return m_itemInfo.m_equipmentModel;
}

QString GraphicsItem::sysOfDevice()
{
    return m_itemInfo.m_sysOfDevice;
}

QString GraphicsItem::protectedAreaName()
{
    return m_itemInfo.m_protectedAreaName;
}

QString GraphicsItem::buildingName()
{
    return m_itemInfo.m_buildingName;
}

QString GraphicsItem::floorOfDevice()
{
    return m_itemInfo.m_floorOfDevice;
}

QString GraphicsItem::deviceLocation()
{
    return m_itemInfo.m_deviceLocation;
}

QString GraphicsItem::operatorDuty()
{
    return m_itemInfo.m_operatorOnDuty;
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
                                "报警类型:%4\n"
                                "设备产品编码:%5\n"
                                "设备设施型号:%6\n"
                                "报警当前状态:%7\n"
                                "报警时间:%8\n"
                                "报警收到时间:%9\n"
                                "报警恢复正常时间:%10\n"
                                "设备所属系统:%11\n"
                                "总保护区域名称:%12\n"
                                "建筑设施名称:%13\n"
                                "设施所在楼层:%14\n"
                                "设施所在位置:%15\n"
                                "值班人员:%16").arg(m_itemInfo.m_extNum).arg(m_itemInfo.m_loopNum).arg(m_itemInfo.m_addrNum)
            .arg(m_itemInfo.m_alarmType).arg(m_itemInfo.m_deviceNum).arg(m_itemInfo.m_equipmentModel)
            .arg(m_itemInfo.m_currentAlarmState).arg(m_itemInfo.m_alarmTime).arg(m_itemInfo.m_alarmReceiveTime)
            .arg(m_itemInfo.m_alarmReplyTime).arg(m_itemInfo.m_sysOfDevice).arg(m_itemInfo.m_protectedAreaName).arg(m_itemInfo.m_buildingName)
            .arg(m_itemInfo.m_floorOfDevice).arg(m_itemInfo.m_deviceLocation).arg(m_itemInfo.m_operatorOnDuty);
    setHoverText(hoverText);
}




//void GraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
//{

//}
