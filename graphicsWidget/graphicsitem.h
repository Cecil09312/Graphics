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
class GraphicsScene;
struct ItemInfo
{
    QString m_extNum;//分机号
    QString m_loopNum;//回路号
    QString m_addrNum;//地址号
    QString m_alarmType;//报警类型
    QString m_deviceNum;//设备产品编码
    QString m_equipmentModel;//设备设施型号
    QString m_currentAlarmState;//报警当前状态
    QString m_alarmTime;//报警时间
    QString m_alarmReceiveTime;//报警收到时间
    QString m_alarmReplyTime;//报警恢复正常时间
    QString m_sysOfDevice;//设备所属系统
    QString m_protectedAreaName;//总保护区域名称
    QString m_buildingName;//建筑设施名称
    QString m_floorOfDevice;//设施所在楼层
    QString m_deviceLocation;//设施所在位置
    QString m_operatorOnDuty;//值班人员
};

class GraphicsItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    //    Q_PROPERTY(QColor color READ color WRITE setColor/* NOTIFY colorChanged*/)
    //    Q_PROPERTY(qreal radius READ radius WRITE setRadius/* NOTIFY radiusChanged*/)
    //    Q_PROPERTY(QString hoverText READ hoverText WRITE setHoverText/* NOTIFY hoverTextChanged*/)
    //    Q_PROPERTY(QString itemText READ itemText WRITE setItemText/* NOTIFY itemTextChanged*/)
    //    Q_PROPERTY(QColor itemTextColor READ itemTextColor WRITE setItemTextColor/* NOTIFY itemTextColorChanged*/)
public:
    GraphicsItem(GraphicsScene *scene);
    ~GraphicsItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
    void startAnimations();
    void stopAnimations();
    void startColorAnimation();
    void stopColorAnimation();
    void startScaleAnimation();
    void stopScaleAnimation();
    void setColorEffectStrength(qreal strength);
    void setAnimationDuration(int duration);
    void setAnimationLoopCount(int count);
    QPointF graphicsItemPos() const;
    void setColorStartValue(const QVariant &value);
    void setColorEndValue(const QVariant &value);
    void setScaleStartValue(const QVariant &value);
    void setScaleEndValue(const QVariant &value);

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
    ItemInfo &getItemInfo();
    Q_INVOKABLE QString &extNum();
    Q_INVOKABLE QString &loopNum();
    Q_INVOKABLE QString &addrNum();
    Q_INVOKABLE QString &alarmType();
    Q_INVOKABLE QString &deviceNum();
    Q_INVOKABLE QString &equipmentModel();
    Q_INVOKABLE QString &sysOfDevice();
    Q_INVOKABLE QString &protectedAreaName();
    Q_INVOKABLE QString &buildingName();
    Q_INVOKABLE QString &floorOfDevice();
    Q_INVOKABLE QString &deviceLocation();
    Q_INVOKABLE QString &operatorDuty();

protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
private:
    void updateHoverText();

private:
    QPropertyAnimation *m_colorAnimation;
    QPropertyAnimation *m_scaleAnimation;
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

};

#endif // GRAPHICSITEM_H
