#include "graphicsitem.h"
#include <QDebug>
#include "graphicsWidget/graphicsscene.h"
#include <QStyleOptionGraphicsItem>
#include "control/controller.h"
#include "jsonEdit/itemiconinfotojson.h"
#include "control/controller.h"
#include <QSvgGenerator>
#include <QGraphicsView>
#include <qmath.h>
#include <QPointF>


GraphicsItem::GraphicsItem(GraphicsScene *scene):
    m_itemTextIsVisiable(true)

{

    m_scaleAnimation=nullptr;
    m_colorAnimation=nullptr;
    m_scaleRunNum =0;
    m_colorRunNum =0;
    //m_parallelAnimGroup=nullptr;
    m_radius=Controller::instance()->getDataStore()->iconSize();
    m_graphicsScene = scene;
    m_itemInfo.m_manufacturers = tr("北京利达华信电子有限公司");
    m_itemInfo.m_sysOfDevice = Controller::instance()->getDataStore()->sysName();
    m_itemInfo.m_deviceOperator = Controller::instance()->getDataStore()->oneOperator();
    m_channelNum = Controller::instance()->getDataStore()->channelNum();
    m_analogType = Controller::instance()->getDataStore()->analogValue();
    m_powerAddr = Controller::instance()->getDataStore()->powerAddr();
    // m_itemInfo.m_networkNum = "0";
    m_color = QColor(Qt::transparent);
    m_penColor = QColor(Qt::transparent);

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlags(ItemIsMovable|ItemIsSelectable|ItemIgnoresTransformations);

    setAcceptHoverEvents(true);
    setProperty("color",m_color);
    setProperty("scale",m_scale);

    // setProperty("angle",m_angle);
    m_itemTextFont.setPointSize(qFloor(m_radius/4));
    m_itemTextFont.setFamily("Arial");


    QString num = QString("%1").arg(Controller::instance()->getDataStore()->itemNum());

    m_itemInfo.m_addrNum= num;
    m_itemInfo.m_extNum = Controller::instance()->getDataStore()->extNum();
    m_itemInfo.m_loopNum = Controller::instance()->getDataStore()->loopNum();
    m_itemInfo.m_networkNum = Controller::instance()->getDataStore()->networkNum();
    m_itemInfo.m_deviceNum = QString("%1-%2").arg(m_itemInfo.m_loopNum).arg(m_itemInfo.m_addrNum);
    //    int itemIconIndex = ItemIconInfoToJson::currentIconIndex();
    //    setInfoFromIconIndex(itemIconIndex);




    //    connect(m_rotateAnimation,&QPropertyAnimation::valueChanged,this,[=]()
    //    {
    //      qreal angle=  qvariant_cast<qreal>(m_rotateAnimation->currentValue());
    //      setRotation(angle);

    //    });

    connect(Controller::instance()->getDrawImageThread(),&DrawImageThread::drawCurrentImage,this,[=](){
        //isStart = true;
        update();
    });


}

GraphicsItem::~GraphicsItem()
{

    stopAnimations();
    clearAlarmRecord();
    if(m_colorAnimation!=nullptr)
    {
        m_colorAnimation->deleteLater();
    }
    if(m_scaleAnimation!=nullptr)
    {
        m_scaleAnimation->deleteLater();
    }
    //stopRotationAnimation();
}

QRectF GraphicsItem::boundingRect() const
{
    qreal penWidth = 1;
    if(m_radius>0)
    {
        return QRectF(-m_radius*0.9 - penWidth / 2, -m_radius*0.9 - penWidth / 2,
                      m_radius*1.5+ penWidth, m_radius*1.5 + penWidth);
    }
    else
    {

        return QRectF(-40 - penWidth / 2, -40 - penWidth / 2,
                      108+ penWidth, 80 + penWidth);
    }

}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*option, QWidget */*widget*/)
{

     QString curDeviceNum=QString("%1-%2").arg(m_itemInfo.m_loopNum).arg(m_itemInfo.m_addrNum) ;
    if(m_itemTextIsVisiable)
    {
        int fontSize = qFloor(m_radius/4);
        m_itemTextFont.setPointSize(fontSize);
        painter->setFont(m_itemTextFont);
    }

    if(m_radius>0)
    {
        if(m_itemTextIsVisiable)
        {
            if(curDeviceNum.size()>8)
            {
                painter->drawText(QRectF(-m_radius*0.9,-m_radius*0.9,1.6*m_radius,1.6*m_radius),Qt::TextWordWrap|Qt::AlignLeft,curDeviceNum);
            }
            else
            {
                painter->drawText(QRectF(-m_radius*0.9,-m_radius*0.9,1.6*m_radius,1.6*m_radius),Qt::AlignHCenter,curDeviceNum);
            }

        }

        painter->setPen(m_penColor);
        painter->setBrush(m_color);
        painter->drawRect(QRectF(-m_radius*0.5,-m_radius*0.5,m_radius,m_radius));
        QRectF rectF = QRectF(-m_radius*0.35,-m_radius*0.35,m_radius*0.7,m_radius*0.7);
        painter->drawImage(rectF,Controller::instance()->getDrawImageThread()->getImageFromName(m_iconName));

    }


    if(!m_itemInfo.m_periodOfValidity.isEmpty()&&QDate::fromString(m_itemInfo.m_periodOfValidity,"yyyy/MM/dd")<=QDate::currentDate())
    {
        painter->setPen(QPen(Qt::red));
        painter->setBrush(Qt::red);
        painter->drawEllipse(QRectF(m_radius/2,m_radius/2,m_radius/10,m_radius/10));
    }

    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::black, 0, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(QRectF(-m_radius*0.55,-m_radius*0.55,1.1*m_radius,1.1*m_radius));
    }
}

void GraphicsItem::setColor(const QColor &color)
{
    m_color = color;
    m_penColor = QColor(Qt::blue);
    update();
}

void GraphicsItem::startAnimations()
{
//    if(m_colorAnimation==nullptr)
//    {

//    }

//    if(m_scaleAnimation==nullptr)
//    {
//        m_scaleAnimation = new QPropertyAnimation(this,"scale");
//        m_scaleAnimation->setStartValue(0.5);
//        m_scaleAnimation->setEndValue(2);
//        m_scaleAnimation->setDuration(1000);
//        m_scaleAnimation->setLoopCount(-1);
//        connect(m_scaleAnimation,&QPropertyAnimation::valueChanged,this,[=](const QVariant &/*value*/)
//        {
//            qreal scale =qvariant_cast<qreal> (m_scaleAnimation->currentValue());
//            setScale(scale);
//        });
//    }

//    if(m_parallelAnimGroup==nullptr)
//    {
//        m_parallelAnimGroup = new QParallelAnimationGroup(this);
//        m_parallelAnimGroup->addAnimation(m_colorAnimation);
//        m_parallelAnimGroup->addAnimation(m_scaleAnimation);
//        m_parallelAnimGroup->setLoopCount(-1);
//    }

//    if(m_parallelAnimGroup!=nullptr)
//    {
//        if(m_parallelAnimGroup->state()!=QParallelAnimationGroup::Running)
//        {
//            m_parallelAnimGroup->start();
//        }
//    }

}

void GraphicsItem::stopAnimations()
{
//    if(m_parallelAnimGroup!=nullptr)
//    {
//        m_parallelAnimGroup->stop();
//    }

    stopScaleAnimation();
    stopColorAnimation();
    restoreSize();
}


void GraphicsItem::startColorAnimation()
{

    if(m_colorAnimation==nullptr)
    {
        m_colorAnimation = new QPropertyAnimation(this,"color");
        m_colorAnimation->setStartValue(QColor(Qt::transparent));
        m_colorAnimation->setEndValue(QColor(Qt::red));
        m_colorAnimation->setDuration(1200);
        m_colorAnimation->setLoopCount(-1);
        connect(m_colorAnimation,&QPropertyAnimation::valueChanged,this,[=](const QVariant &/*value*/)
        {
            if(m_colorRunNum>60)
            {
                m_colorRunNum =0;
            }
            if(m_colorRunNum%12==0)
            {
                QColor color =qvariant_cast<QColor> (m_colorAnimation->currentValue());
                m_color = color;
                m_penColor = QColor(Qt::blue);
                update();
            }
            m_colorRunNum++;

        });
    }

    if(m_colorAnimation!=nullptr)
    {
        if(m_colorAnimation->state()!=QPropertyAnimation::Running)
        {
            m_colorAnimation->start();
        }
    }


}

void GraphicsItem::stopColorAnimation()
{
    if(m_colorAnimation!=nullptr)
    {
        m_colorAnimation->stop();
        m_colorRunNum=0;
    }

    m_color = QColor(Qt::transparent);
    m_penColor = QColor(Qt::transparent);
    update();
    //setColorEffectValue(0.0);
}

void GraphicsItem::startScaleAnimation()
{
    if(m_scaleAnimation==nullptr)
    {
        m_scaleAnimation = new QPropertyAnimation(this,"scale");
        m_scaleAnimation->setStartValue(0.5);
        m_scaleAnimation->setEndValue(2);
        m_scaleAnimation->setDuration(1200);
        m_scaleAnimation->setLoopCount(-1);
        connect(m_scaleAnimation,&QPropertyAnimation::valueChanged,this,[=](const QVariant &/*value*/)
        {

            if(m_scaleRunNum>=60)
            {
                m_scaleRunNum=0;
            }
            if(m_scaleRunNum%12==0)
            {
                qreal scale =qvariant_cast<qreal> (m_scaleAnimation->currentValue());
                setScale(scale);
            }
            m_scaleRunNum++;

        });
    }

    if(m_scaleAnimation!=nullptr)
    {

        if(m_scaleAnimation->state()!=QPropertyAnimation::Running)
        {
            m_scaleAnimation->start();
        }
    }


}

void GraphicsItem::stopScaleAnimation()
{
    if(m_scaleAnimation!=nullptr)
    {
        m_scaleAnimation->stop();
        m_scaleRunNum=0;
    }

}



void GraphicsItem::setAnimationDuration(int duration)
{
    if(m_colorAnimation!=nullptr)
    {
        m_colorAnimation->setDuration(duration);
    }
    if(m_scaleAnimation!=nullptr)
    {
        m_scaleAnimation->setDuration(duration);
    }

}

void GraphicsItem::setAnimationLoopCount(int count)
{

    if(m_colorAnimation!=nullptr)
    {
        m_colorAnimation->setLoopCount(count);
    }
    if(m_scaleAnimation!=nullptr)
    {
        m_scaleAnimation->setLoopCount(count);
    }

//    if(m_parallelAnimGroup!=nullptr)
//    {
//       m_parallelAnimGroup->setLoopCount(count);
//    }

}

QPointF GraphicsItem::graphicsItemPos() const
{
    return  this->pos();
}

void GraphicsItem::setColorStartValue(const QVariant &value)
{
    if(m_colorAnimation!=nullptr)
    {
        m_colorAnimation->setStartValue(value);
    }

}

void GraphicsItem::setColorEndValue(const QVariant &value)
{
    if(m_colorAnimation!=nullptr)
    {
        m_colorAnimation->setEndValue(value);
    }

}

void GraphicsItem::setScaleStartValue(const QVariant &value)
{
    if(m_scaleAnimation!=nullptr)
    {
        m_scaleAnimation->setStartValue(value);
    }

}

void GraphicsItem::setScaleEndValue(const QVariant &value)
{
    if(m_scaleAnimation!=nullptr)
    {
         m_scaleAnimation->setEndValue(value);
    }

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
        // update();
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
    Controller::instance()->getDataStore()->iconSize() =radius;
    m_radius = radius;
    //QRectF rectF = QRectF(-m_radius/2,-m_radius/2,m_radius*1.5,m_radius*1.5);
    //m_drawImageThread->setImageRect(rectF);
    emit sizeChanged(radius);
    update();
}

QString GraphicsItem::hoverText() const
{
    return m_hoverText;
}

void GraphicsItem::setHoverText(const QString &hoverText)
{
    m_hoverText = hoverText;
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
    QString curIconName = Controller::instance()->fileNameFromQml(iconName);

    if(m_iconName!=curIconName)
    {
        m_iconName = curIconName;
#ifdef Q_OS_LINUX
        if(!m_iconName.startsWith("/home"))
        {
            QFileInfo fileInfo(m_iconName);
            m_iconName =QApplication::applicationDirPath()+"/设备图标/" +fileInfo.fileName();
        }
#endif

        QString equipmentModel= ItemIconInfoToJson::getValue(QString::number(ItemIconInfoToJson::iconIndex(iconName)),"deviceName");
        m_itemInfo.m_equipmentModel =equipmentModel;

        Controller::instance()->getDrawImageThread()->drawImage(m_iconName);
    }
//    else
//    {
//        update();
//    }

}

QHash<QString, QVariant> GraphicsItem::itemInfo()
{
    QHash<QString, QVariant> itemHash;
    itemHash["extNum"] = m_itemInfo.m_extNum;
    itemHash["loopNum"] = m_itemInfo.m_loopNum;
    itemHash["addrNum"] = m_itemInfo.m_addrNum;
    itemHash["networkNum"]=m_itemInfo.m_networkNum;
    itemHash["powerAddr"] = m_powerAddr;
    itemHash["deviceNum"] = m_itemInfo.m_deviceNum;
    itemHash["equipmentModel"] = m_itemInfo.m_equipmentModel;
    itemHash["currentState"] = currentState();
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

void GraphicsItem::setAnlogValue(int curChannel, const QVariant &value)
{
    if(curChannel<=m_channelNum)
    {
        m_analogValueHash[curChannel] =value;
    }

}

QHash<int, QVariant> GraphicsItem::anlogValueHah()
{
    return m_analogValueHash;
}

ItemInfo &GraphicsItem::getItemInfo()
{
    return m_itemInfo;
}

int GraphicsItem::iconIndex()
{
    int index= ItemIconInfoToJson::iconIndex(m_iconName);
    m_iconIndex = index;
    return m_iconIndex;
}

void GraphicsItem::setIconIndex(int index)
{
    m_iconIndex = index;
}

void GraphicsItem::setInfoFromIconIndex(int itemIconIndex)
{

    QHash<QString,QVariant>itemIconInfoHash = ItemIconInfoToJson::getIconInfoHash();
    m_iconIndex = itemIconIndex;
    if(itemIconInfoHash.size()>0)
    {
        QHash<QString,QVariant> deviceHash=  itemIconInfoHash[QString("%1").arg(itemIconIndex)].toHash();
        m_itemInfo.m_equipmentModel= deviceHash.value("deviceName").toString();
        m_itemInfo.m_manufacturers = deviceHash.value("manufacturers").toString();
        m_itemInfo.m_periodOfValidity = deviceHash.value("periodOfvalidity").toString();
        m_itemInfo.m_deviceInstallTime = deviceHash.value("deviceInstallTime").toString();
    }
    else
    {
        m_itemInfo.m_equipmentModel ="";
        ItemIconInfoToJson::setCurrentIconIndex(-1);
    }
    if(itemIconIndex>=0)
    {
        QString currentIconName = ItemIconInfoToJson::getIconName(itemIconIndex);
        if(!currentIconName.isEmpty())
        {
            setIconName( Controller::instance()->fileNameFromQml(currentIconName));
        }
        else
        {
            ItemIconInfoToJson::setCurrentIconIndex(-1);
        }

    }
    else
    {
        ItemIconInfoToJson::setCurrentIconIndex(-1);
    }
    update();
}

bool GraphicsItem::itemTextIsVisiable()
{
    return m_itemTextIsVisiable;
}

void GraphicsItem::setPeriodOfValidity(const QString &period)
{
    m_itemInfo.m_periodOfValidity = period;
    update();
}

void GraphicsItem::setChannelNum(int num)
{
    m_channelNum = num;
}

int &GraphicsItem::channelNum()
{
    return m_channelNum;
}

void GraphicsItem::setItemTextVisiable(bool isOk)
{
    m_itemTextIsVisiable = isOk;
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

QString &GraphicsItem::networkNum()
{
    return m_itemInfo.m_networkNum;
}

QString GraphicsItem::currentState()
{
    QString curRecord = currentAlarmRecord();
    if(curRecord.isEmpty())
    {
        return tr("正常");
    }
    else
    {
        QString curResult = m_alarmStateHash.value(curRecord);
        if(curResult.isEmpty())
        {
            return tr("正常");
        }
        else
        {
            return curResult;
        }
    }


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

QString &GraphicsItem::deviceInstallTime()
{
    return m_itemInfo.m_deviceInstallTime;
}

QString GraphicsItem::alarmType()
{
    return  currentAlarmRecord();
}

QString &GraphicsItem::analogType()
{
    return m_analogType;
}

QString GraphicsItem::alarmTime(const QString &alarmType)
{
    return  m_alarmTimeHash.value(alarmType);
}

QString GraphicsItem::alarmReplyTime(const QString &alarmType)
{
    return  m_alarmReplyTimeHash.value(alarmType);
}

QString GraphicsItem::alarmState(const QString &alarmType)
{

    QString curState = m_alarmStateHash.value(alarmType);
    if(!curState.isEmpty())
    {
        return  curState;
    }
    else
    {
        return tr("正常");
    }

}

QString &GraphicsItem::powerAddr()
{
    return m_powerAddr;
}

QList<QString> GraphicsItem::alarmTypeList()
{
    return m_alarmTimeHash.keys();
}


QString GraphicsItem::currentAlarmRecord()
{
    QString targetStr = "";
    if(!m_alarmTimeHash.isEmpty())
    {
        int priority =100;
        QList<QString>alarmRecordList= m_alarmTimeHash.keys();
        foreach (QString alarmType, alarmRecordList)
        {
            QString alarmRecordState = m_alarmStateHash.value(alarmType);
            if(alarmRecordState!=tr("正常")&& !alarmRecordState.isEmpty())
            {
                priority = qMin(priority,int(m_alarmPriorityHash.value(alarmType)));
                if(priority==m_alarmPriorityHash.value(alarmType))
                {
                    targetStr = alarmType;
                }
            }

        }
    }

    return targetStr;
}



void GraphicsItem::setAlarmRecord(const QString &alarmType, const QString &alarmTime, const QString &alarmState)
{

       m_alarmTimeHash[alarmType]=alarmTime;
       m_alarmStateHash[alarmType]= alarmState;

        if(alarmType==tr("火警"))
        {
            m_alarmPriorityHash[alarmType] = FireAlarm;
            //alarmRecord.m_alarmPriority=FireAlarm;
        }
        else if(alarmType==tr("监管"))
        {
            m_alarmPriorityHash[alarmType] =Supervision;
        }
        else if(alarmType==tr("启动"))
        {
            m_alarmPriorityHash[alarmType] =Startover;
        }
        else if(alarmType==tr("反馈"))
        {
            m_alarmPriorityHash[alarmType] =Respond;
        }
        else if(alarmType==tr("故障"))
        {
            m_alarmPriorityHash[alarmType] = Breakdown;
        }
        else if(alarmType==tr("屏蔽"))
        {
            m_alarmPriorityHash[alarmType] =Shield;
        }
        else if(alarmType==tr("模拟火警"))
        {
            m_alarmPriorityHash[alarmType] =AnalogFireAlarm;//模拟火警
        }
        else if(alarmType==tr("模拟监管"))

        {
            m_alarmPriorityHash[alarmType] =AnalogSupervision;//模拟监管
        }
        else if(alarmType==tr("模拟启动"))
        {
            m_alarmPriorityHash[alarmType] =AnalogStartover;//模拟启动
        }

        else if(alarmType==tr("模拟反馈"))
        {
            m_alarmPriorityHash[alarmType] =AnalogRespond;//模拟反馈
        }

        else if(alarmType==tr("模拟故障"))
        {
            m_alarmPriorityHash[alarmType] =AnalogBreakdown;//模拟故障
        }

        else if(alarmType==tr("模拟屏蔽"))
        {
            m_alarmPriorityHash[alarmType] =AnalogShield;//模拟屏蔽
        }


   // }


}

void GraphicsItem::removeAlarmRecord(const QString &alarmType, const QString &alarmReplyTime)
{
    if(m_alarmTimeHash.contains(alarmType))
    {
        m_alarmReplyTimeHash[alarmType] = alarmReplyTime;
        m_alarmStateHash[alarmType]= tr("正常");

    }

}

void GraphicsItem::clearAllAlarm()
{
    foreach (QString type, m_alarmStateHash.keys())
    {
        m_alarmStateHash[type] = tr("正常");
        m_alarmReplyTimeHash[type] = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    }
}

void GraphicsItem::clearAlarmRecord()
{

    m_alarmStateHash.clear();
    m_alarmPriorityHash.clear();
    m_alarmTimeHash.clear();
    m_alarmReplyTimeHash.clear();
}

void GraphicsItem::setAlarmState(const QString &alarmType,const QString &alarmState)
{
    m_alarmStateHash[alarmType] = alarmState;
}

void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(Controller::instance()->getUserRight()!=UserManager::User&&ArchitePlanView::itemLimit())
    {
        setPos(event->scenePos());
        emit moveToPos(event->scenePos());
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
    QString curType ;
    if(currentState()==tr("正常")&&(!alarmType().isEmpty()))
    {
        curType =alarmType()+ tr("消除");
    }
    QString hoverText = QString(tr("分机号:%1")+"\n"+
                                tr("回路号:%2")+"\n"+
                                tr("地址号:%3")+"\n"+
                                tr("网络号:%4")+"\n"+
                                tr("电源地址:%5")+"\n"+
                                tr("设备编码:%6")+"\n"+
                                tr("设备:%7")+"\n"+
                                tr("事件类型:%8")+"\n"+
                                tr("状态:%9")+"\n"+
                                tr("时间:%10")+"\n"+
                                tr("系统:%11")+"\n"+
                                tr("建筑名称:%12")+"\n"+
                                tr("楼层:%13")+"\n"+
                                tr("位置:%14")+"\n"+
                                tr("制造商:%15")+"\n"+
                                tr("有效期:%16")+"\n"+
                                tr("操作员:%17")).arg(m_itemInfo.m_extNum).arg(m_itemInfo.m_loopNum).arg(m_itemInfo.m_addrNum).arg(m_itemInfo.m_networkNum)
            .arg(m_powerAddr).arg(m_itemInfo.m_deviceNum).arg(m_itemInfo.m_equipmentModel).arg(curType).arg(currentState())
            .arg(alarmTime(alarmType())).arg(m_itemInfo.m_sysOfDevice).arg(m_itemInfo.m_buildingName)
            .arg(m_itemInfo.m_floorOfDevice).arg(m_itemInfo.m_deviceLocation).arg(m_itemInfo.m_manufacturers)
            .arg(m_itemInfo.m_periodOfValidity).arg(m_itemInfo.m_deviceOperator);
    setHoverText(hoverText);
}

