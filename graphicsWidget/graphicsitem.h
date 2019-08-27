#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QParallelAnimationGroup>
#include <QGraphicsSvgItem>
#include "jsonEdit/itemiconinfotojson.h"
#include <QGraphicsColorizeEffect>
#include <QPair> 
class GraphicsScene;
struct ItemInfo
{
    QString m_extNum;//分机号
    QString m_loopNum;//回路号
    QString m_addrNum;//地址号
    QString m_networkNum;//网络号
    QString m_deviceNum;//设备编码
    QString m_equipmentModel;//设备
    //QString m_alarmType;//报警类型
   // QString m_currentState;//报警状态
   // QString m_alarmTime;//报警时间
   // QString m_alarmReplyTime;//报警恢复时间
    QString m_sysOfDevice;//系统
    QString m_buildingName;//建筑名称
    QString m_floorOfDevice;//楼层
    QString m_deviceLocation;//位置
    QString m_manufacturers;//制造商
    QString m_periodOfValidity;//有效期
    QString m_deviceOperator;//操作员
};

enum AlarmPriority//报警优先级
{
    FireAlarm=1,//火警
    Supervision,//监管
    Startover,//启动
    Feedback,//反馈
    Breakdown,//故障
    Shield,//屏蔽
    AnalogFireAlarm,//模拟火警
    AnalogSupervision,//模拟监管
    AnalogStartover,//模拟启动
    AnalogFeedback,//模拟反馈
    AnalogBreakdown,//模拟故障
    AnalogShield//模拟屏蔽
};

struct AlarmRecord//用来记录所有报警信息
{
    AlarmPriority m_alarmPriority;
   // QString m_alarmRecordType;//记录报警类型
    QString m_alarmRecordTime;//记录报警时间
    QString m_alarmRecordState;//报警状态
    QString m_alarmRecordReplyTime;//报警恢复时间
};

class GraphicsItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    GraphicsItem(GraphicsScene *scene);
    ~GraphicsItem();
    QRectF boundingRect() const;

    void startAnimations();
    void stopAnimations();
    void startColorAnimation();
    void stopColorAnimation();
    void startScaleAnimation();
    void stopScaleAnimation();
    void startRotationAnimation();
    void stopRotationAnimation();
    void setColorEffectStrength(qreal strength);
    void setAnimationDuration(int duration);
    void setAnimationLoopCount(int count);
    QPointF graphicsItemPos() const;
    void setColorStartValue(const QVariant &value);
    void setColorEndValue(const QVariant &value);
    void setScaleStartValue(const QVariant &value);
    void setScaleEndValue(const QVariant &value);
    void setColorEffectValue(qreal value);
    void restoreSize();

    void setColor(const QColor &color);
    QColor color() const;
    qreal radius() const;
    void setRadius(qreal radius);
    QString hoverText() const;
    void setHoverText(const QString &hoverText);

    QColor itemTextColor() const;
    void setItemTextColor(const QColor &color);
    QString iconName() const;
    void setIconName(const QString &iconName);
    QHash<QString,QVariant> itemInfo();
    void setItemInfo(const ItemInfo &itemInfo);
    void setAnlogValue(int curChannel,const QVariant &value);
    QHash<int, QVariant >anlogValueHah();
    ItemInfo &getItemInfo();
    int iconIndex();
    void setInfoFromIconIndex(int itemIconIndex);
    bool itemTextIsVisiable();
    Q_INVOKABLE void setPeriodOfValidity(const QString &period);
    Q_INVOKABLE void setChannelNum(int num);
    Q_INVOKABLE int &channelNum();

    Q_INVOKABLE void setItemTextVisiable(bool isOk);

    Q_INVOKABLE QString &extNum();
    Q_INVOKABLE QString &loopNum();
    Q_INVOKABLE QString &addrNum();
    Q_INVOKABLE QString &networkNum();
    Q_INVOKABLE QString &currentState();
    Q_INVOKABLE QString &deviceNum();
    Q_INVOKABLE QString &equipmentModel();
    Q_INVOKABLE QString &sysOfDevice();
    Q_INVOKABLE QString &buildingName();
    Q_INVOKABLE QString &floorOfDevice();
    Q_INVOKABLE QString &deviceLocation();
    Q_INVOKABLE QString &manufacturers();
    Q_INVOKABLE QString &periodOfValidity();
    Q_INVOKABLE QString &deviceOperator();
    Q_INVOKABLE QString alarmType();
    Q_INVOKABLE QString &analogType();
    Q_INVOKABLE QString alarmTime(const QString  &alarmType);
    Q_INVOKABLE QString alarmReplyTime(const QString  &alarmType);
    Q_INVOKABLE QString alarmState(const QString  &alarmType);
    QList<QString>alarmTypeList();
    QList<AlarmRecord *> alarmRecordList();
    AlarmRecord *record(const QString &alarmType);
    AlarmRecord *currentAlarmRecord();
    void setAlarmRecord(const QString &alarmType,const QString &alarmTime,const QString &alarmState);
    void removeAlarmRecord(const QString &alarmType, const QString &alarmReplyTime);
    void clearAllAlarm();
    void clearAlarmRecord();
    void setAlarmState(const QString &alarmType, const QString &alarmState);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
private:
    void updateHoverText();

private:
    QPropertyAnimation *m_colorAnimation;
    QPropertyAnimation *m_scaleAnimation;
    QPropertyAnimation *m_rotateAnimation;
    QParallelAnimationGroup *m_parallelAnimGroup;
    QColor m_color;
    GraphicsScene *m_graphicsScene;
    qreal m_radius;
    QString m_hoverText;
    QColor m_itemTextColor;
    QString m_iconName;
    static int m_num;
    ItemInfo m_itemInfo;
    QFont m_itemTextFont;
    ItemIconInfoToJson m_itemIconInfoToJson;
    QGraphicsColorizeEffect *m_colorEffect;
    int m_iconIndex;
    QHash<int, QVariant >m_analogValueHash;//模拟量
    int m_channelNum;//通道号
    QString m_analogType;
    bool m_itemTextIsVisiable;
    QHash<QString,AlarmRecord*>m_alarmRecordHash;
    QString m_currentState;
    qreal m_angle;
    qreal m_scale;
};

#endif // GRAPHICSITEM_H
