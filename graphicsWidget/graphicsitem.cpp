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
    m_radius=DataStore::iconSize();
    m_graphicsScene = scene;
    m_itemInfo.m_manufacturers = tr("北京利达华信电子有限公司");
    m_itemInfo.m_sysOfDevice = DataStore::sysName();
    m_itemInfo.m_deviceOperator = DataStore::oneOperator();
    m_channelNum = DataStore::channelNum();
    m_analogType = DataStore::analogValue();
    m_powerAddr = DataStore::powerAddr();
   // m_itemInfo.m_networkNum = "0";

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlags(ItemIsMovable|ItemIsSelectable);
    m_colorEffect = new QGraphicsColorizeEffect(this);
    setAcceptHoverEvents(true);
    setGraphicsEffect(m_colorEffect);
    m_colorEffect->setStrength(0.0);
    setProperty("color",m_color);
    setProperty("scale",m_scale);
   // setProperty("angle",m_angle);
    m_colorAnimation = new QPropertyAnimation(this,"color");
    m_colorAnimation->setStartValue(QColor(Qt::black));
    m_colorAnimation->setEndValue(QColor(Qt::red));
    m_colorAnimation->setDuration(500);
    m_colorAnimation->setLoopCount(-1);
    m_scaleAnimation = new QPropertyAnimation(this,"scale");
    m_scaleAnimation->setStartValue(0.3);
    m_scaleAnimation->setEndValue(1.5);
    m_scaleAnimation->setDuration(1000);
    m_scaleAnimation->setLoopCount(-1);

//    m_rotateAnimation = new QPropertyAnimation(this,"angle");
//    m_rotateAnimation->setDuration(500);
//    m_rotateAnimation->setLoopCount(-1);
//    m_rotateAnimation->setStartValue(-30);
//    m_rotateAnimation->setEndValue(30);

    m_parallelAnimGroup = new QParallelAnimationGroup(this);
    m_parallelAnimGroup->addAnimation(m_colorAnimation);
   // m_parallelAnimGroup->addAnimation(m_rotateAnimation);
    m_parallelAnimGroup->addAnimation(m_scaleAnimation);
    m_parallelAnimGroup->setLoopCount(-1);

    m_itemTextFont.setPointSize(qFloor(m_radius/3));
    m_itemTextFont.setFamily("宋体");

    QString num = QString("%1").arg(DataStore::itemNum());
    m_itemInfo.m_deviceNum = num;
    m_itemInfo.m_addrNum= num;
    m_itemInfo.m_extNum = DataStore::extNum();
    m_itemInfo.m_loopNum = DataStore::loopNum();
    m_itemInfo.m_networkNum = DataStore::networkNum();
    int itemIconIndex = ItemIconInfoToJson::currentIconIndex();
    setInfoFromIconIndex(itemIconIndex);
    connect(m_colorAnimation,&QPropertyAnimation::valueChanged,this,[=](const QVariant &/*value*/)
    {
        QColor color =qvariant_cast<QColor> (m_colorAnimation->currentValue());
        m_color = color;
        m_colorEffect->setColor(m_color);

    });

    connect(m_scaleAnimation,&QPropertyAnimation::valueChanged,this,[=](const QVariant &/*value*/)
    {
        qreal scale =qvariant_cast<qreal> (m_scaleAnimation->currentValue());
        setScale(scale);

//        if(m_graphicsScene!=nullptr)
//        {
//            m_graphicsScene->update();
//        }
    });

//    connect(m_rotateAnimation,&QPropertyAnimation::valueChanged,this,[=]()
//    {
//      qreal angle=  qvariant_cast<qreal>(m_rotateAnimation->currentValue());
//      setRotation(angle);

//    });


}

GraphicsItem::~GraphicsItem()
{

    stopAnimations();
    clearAlarmRecord();
    //stopRotationAnimation();
}

QRectF GraphicsItem::boundingRect() const
{
    qreal penWidth = 1;
    if(m_radius>0)
    {
        return QRectF(-m_radius - penWidth / 2, -m_radius - penWidth / 2,
                      m_radius*3 + penWidth, m_radius*3 + penWidth);
    }
    else
    {

        return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                      25 + penWidth, 25 + penWidth);
    }

}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*option, QWidget */*widget*/)
{

    if(m_itemTextIsVisiable)
    {
        int fontSize =0;
        if(m_radius<=10)
        {
            fontSize = qFloor(m_radius/2.5);
        }
        else
        {
            fontSize = qFloor(m_radius/3);
        }
        m_itemTextFont.setPointSize(fontSize);
        painter->setFont(m_itemTextFont);
    }

//    QString curDeviceNum =m_itemInfo.m_deviceNum;
//    if(curDeviceNum.size()>9)
//    {
//        curDeviceNum.insert(9,"\n");
//    }

    if(m_radius>0)
    {
        if(m_itemTextIsVisiable)
        {
            painter->drawText(QRect(-m_radius,-m_radius,3*m_radius,3*m_radius),m_itemInfo.m_deviceNum);
        }

        QRectF rectF = QRectF(-m_radius/2,-m_radius/2,m_radius*1.5,m_radius*1.5);
        if(m_iconName.endsWith(".svg"))
        {
            QSvgRenderer renderer(m_iconName);
            renderer.render(painter,rectF);

        }
        else
        {
            if(!QPixmap(m_iconName).isNull())
            {
                painter->drawPixmap(rectF.toRect(),QPixmap(m_iconName));
            }
        }

    }
    else
    {
        if(!QPixmap(m_iconName).isNull())
        {
            painter->drawPixmap(-20/1.25,-20/1.25,40,40,QPixmap(m_iconName));
        }
        if(m_itemTextIsVisiable)
        {
            painter->drawText(QRectF(-20,-20,40,40),m_itemInfo.m_addrNum);
        }


    }

    if(!m_itemInfo.m_periodOfValidity.isEmpty()&&QDate::fromString(m_itemInfo.m_periodOfValidity,"yyyy/MM/dd")<=QDate::currentDate())
    {
        painter->setPen(QPen(Qt::red));
        painter->setBrush(Qt::red);
        painter->drawEllipse(m_radius,m_radius,m_radius/5,m_radius/5);
    }

    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::black, 0, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(m_radius*2.25, m_radius*2.25, -m_radius*2.65, -m_radius*2.65));
    }
}

void GraphicsItem::setColor(const QColor &color)
{
    m_color = color;
    m_colorEffect->setColor(m_color);
}

void GraphicsItem::startAnimations()
{
    m_colorEffect->setStrength(1.0);
    if(m_parallelAnimGroup->state()!=QParallelAnimationGroup::Running)
    {
        m_parallelAnimGroup->start();
    }
}

void GraphicsItem::stopAnimations()
{
    m_parallelAnimGroup->stop();
    stopScaleAnimation();
    stopColorAnimation();
    restoreSize();
}


void GraphicsItem::startColorAnimation()
{
    setColorEffectValue(1.0);
    if(m_colorAnimation->state()!=QPropertyAnimation::Running)
    {
         m_colorAnimation->start();
    }

}

void GraphicsItem::stopColorAnimation()
{
    m_colorAnimation->stop();
    setColorEffectValue(0.0);
}

void GraphicsItem::startScaleAnimation()
{
    if(m_scaleAnimation->state()!=QPropertyAnimation::Running)
    {
        m_scaleAnimation->start();
    }

}

void GraphicsItem::stopScaleAnimation()
{
    m_scaleAnimation->stop();
}

//void GraphicsItem::startRotationAnimation()
//{
//    if(m_rotateAnimation->state()!=QPropertyAnimation::Running)
//    {
//        m_rotateAnimation->start();
//    }

//}

//void GraphicsItem::stopRotationAnimation()
//{
//    m_rotateAnimation->stop();
//    setRotation(0);
//}

//void GraphicsItem::setColorEffectStrength(qreal strength)
//{
//    m_colorEffect->setStrength(strength);
//}

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
       // update();
    }
//    qreal curRotation = rotation();
//    if(curRotation!=0)
//    {
//        setRotation(0);
//    }
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
    DataStore::iconSize() =radius;
    m_radius = radius;
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
    m_iconName = Controller::instance()->fileNameFromQml(iconName);
#ifdef Q_OS_LINUX
                    if(!m_iconName.startsWith("/home"))
                    {
                        QFileInfo fileInfo(m_iconName);
                        m_iconName ="/home/rpdzkj/usr/设备图标/" +fileInfo.fileName();
                    }
#endif
    m_itemInfo.m_equipmentModel =Controller::instance()->getFileNameFromUrl(iconName,false);
    update();
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

    QHash<QString,QVariant>itemIconInfoHash = m_itemIconInfoToJson.getIconInfoHash();
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

QString &GraphicsItem::currentState()
{
    AlarmRecord *record=   currentAlarmRecord();
    if(record!=nullptr)
    {
        m_currentState = record->m_alarmRecordState;
    }
    else
    {
        m_currentState= tr("正常");
    }
    return m_currentState;
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
    return m_alarmRecordHash.key(currentAlarmRecord());
}

QString &GraphicsItem::analogType()
{
    return m_analogType;
}

QString GraphicsItem::alarmTime(const QString &alarmType)
{
    if(m_alarmRecordHash.contains(alarmType))
    {
        AlarmRecord *alramRecord =m_alarmRecordHash.value(alarmType);
        return alramRecord->m_alarmRecordTime;
    }
    else
    {
        return "";
    }
}

QString GraphicsItem::alarmReplyTime(const QString &alarmType)
{
    if(m_alarmRecordHash.contains(alarmType))
    {
        AlarmRecord *alramRecord =m_alarmRecordHash.value(alarmType);
        return alramRecord->m_alarmRecordReplyTime;
    }
    else
    {
        return "";
    }
}

QString GraphicsItem::alarmState(const QString &alarmType)
{
    if(m_alarmRecordHash.contains(alarmType))
    {
        AlarmRecord *alramRecord =m_alarmRecordHash.value(alarmType);
        return alramRecord->m_alarmRecordState;
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
    return m_alarmRecordHash.keys();
}

QList<AlarmRecord*> GraphicsItem::alarmRecordList()
{
    return m_alarmRecordHash.values();
}

AlarmRecord *GraphicsItem::record(const QString &alarmType)
{
    return m_alarmRecordHash.value(alarmType);
}

//QList<AlarmRecord> GraphicsItem::getAlarmRecordList() const
//{
//    return alarmRecordList;
//}

//void GraphicsItem::setAlarmRecordList(const QList<AlarmRecord> &value)
//{
//    alarmRecordList = value;
//}

AlarmRecord *GraphicsItem::currentAlarmRecord()
{

    AlarmRecord *curAlarmRecord=nullptr;
    if(!m_alarmRecordHash.isEmpty())
    {
        int priority =100;
        QList<QString>alarmRecordList= m_alarmRecordHash.keys();
        foreach (QString alarmType, alarmRecordList)
        {
            AlarmRecord *alarmRecord= m_alarmRecordHash.value(alarmType);
            if(alarmRecord->m_alarmRecordState!=tr("正常")&& !alarmRecord->m_alarmRecordState.isEmpty())
            {
                priority = qMin(priority,(int)alarmRecord->m_alarmPriority);
                if(priority==alarmRecord->m_alarmPriority)
                {
                    curAlarmRecord = alarmRecord;
                }
            }

        }
    }

    return curAlarmRecord;
}



void GraphicsItem::setAlarmRecord(const QString &alarmType, const QString &alarmTime, const QString &alarmState)
{
    AlarmRecord *alarmRecord = nullptr;
    if(m_alarmRecordHash.contains(alarmType)&&m_alarmRecordHash.value(alarmType)!=nullptr)
    {
        alarmRecord= m_alarmRecordHash.value(alarmType);

    }
    else
    {
        alarmRecord = new AlarmRecord;
        m_alarmRecordHash[alarmType] = alarmRecord;
    }
    if(alarmRecord!=nullptr)
    {
        alarmRecord->m_alarmRecordTime = alarmTime;
        alarmRecord->m_alarmRecordState = alarmState;
        if(alarmType==tr("火警"))
        {
            alarmRecord->m_alarmPriority=FireAlarm;
        }
        else if(alarmType==tr("监管"))
        {
            alarmRecord->m_alarmPriority=Supervision;
        }
        else if(alarmType==tr("启动"))
        {
            alarmRecord->m_alarmPriority=Startover;
        }
        else if(alarmType==tr("反馈"))
        {
            alarmRecord->m_alarmPriority=Feedback;
        }
        else if(alarmType==tr("故障"))
        {
            alarmRecord->m_alarmPriority = Breakdown;
        }
        else if(alarmType==tr("屏蔽"))
        {
            alarmRecord->m_alarmPriority=Shield;
        }
        else if(alarmType==tr("模拟火警"))
        {
            alarmRecord->m_alarmPriority=AnalogFireAlarm;//模拟火警
        }
        else if(alarmType==tr("模拟监管"))

        {
            alarmRecord->m_alarmPriority=AnalogSupervision;//模拟监管
        }
        else if(alarmType==tr("模拟启动"))
        {
            alarmRecord->m_alarmPriority=AnalogStartover;//模拟启动
        }

        else if(alarmType==tr("模拟反馈"))
        {
            alarmRecord->m_alarmPriority=AnalogFeedback;//模拟反馈
        }

        else if(alarmType==tr("模拟故障"))
        {
            alarmRecord->m_alarmPriority=AnalogBreakdown;//模拟故障
        }

        else if(alarmType==tr("模拟屏蔽"))
        {
            alarmRecord->m_alarmPriority=AnalogShield;//模拟屏蔽
        }


    }


}

void GraphicsItem::removeAlarmRecord(const QString &alarmType, const QString &alarmReplyTime)
{
    if(m_alarmRecordHash.contains(alarmType))
    {
        AlarmRecord *alarmRecord = m_alarmRecordHash[alarmType];
        if(alarmRecord!=nullptr)
        {
            alarmRecord->m_alarmRecordReplyTime =alarmReplyTime;
            alarmRecord->m_alarmRecordState = tr("正常");

        }
    }

}

void GraphicsItem::clearAllAlarm()
{
    QList<AlarmRecord*>alarmRecordList=m_alarmRecordHash.values();
    foreach (AlarmRecord*record, alarmRecordList)
    {
        if(record!=nullptr)
        {
            record->m_alarmRecordState = tr("正常");
            record->m_alarmRecordReplyTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        }

    }
}

void GraphicsItem::clearAlarmRecord()
{

    foreach (AlarmRecord *alarmRecord, m_alarmRecordHash) {
        delete alarmRecord;
        alarmRecord = nullptr;
    }
    m_alarmRecordHash.clear();
}

void GraphicsItem::setAlarmState(const QString &alarmType,const QString &alarmState)
{
    if(m_alarmRecordHash.contains(alarmType))
    {
        m_alarmRecordHash[alarmType]->m_alarmRecordState = alarmState;

    }
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
    QString hoverText = QString("分机号:%1\n"
                                "回路号:%2\n"
                                "地址号:%3\n"
                                "网络号:%4\n"
                                "电源地址:%5\n"
                                "设备编码:%6\n"
                                "设备:%7\n"
                                "事件类型:%8\n"
                                "状态:%9\n"
                                "时间:%10\n"
                                "系统:%11\n"
                                "建筑名称:%12\n"
                                "楼层:%13\n"
                                "位置:%14\n"
                                "制造商:%15\n"
                                "有效期:%16\n"
                                "操作员:%17").arg(m_itemInfo.m_extNum).arg(m_itemInfo.m_loopNum).arg(m_itemInfo.m_addrNum).arg(m_itemInfo.m_networkNum)
            .arg(m_powerAddr).arg(m_itemInfo.m_deviceNum).arg(m_itemInfo.m_equipmentModel).arg(curType).arg(currentState())
            .arg(alarmTime(alarmType())).arg(m_itemInfo.m_sysOfDevice).arg(m_itemInfo.m_buildingName)
            .arg(m_itemInfo.m_floorOfDevice).arg(m_itemInfo.m_deviceLocation).arg(m_itemInfo.m_manufacturers)
            .arg(m_itemInfo.m_periodOfValidity).arg(m_itemInfo.m_deviceOperator);
    setHoverText(hoverText);
}

