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
    QString m_deviceNum;//设备编号
    QString m_equipmentModel;//设备
//    QString m_equipmentName;//设备
//    QString m_equipmentType;//设备
    QString m_currentState;//状态
    QString m_alarmTime;//报警时间
    QString m_alarmReplyTime;//报警恢复时间
    QString m_sysOfDevice;//系统
    QString m_buildingName;//设施
    QString m_floorOfDevice;//楼层
    QString m_deviceLocation;//位置
    QString m_manufacturers;//制造商
    QString m_periodOfValidity;//有效期
};

class GraphicsItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
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
    ItemInfo &getItemInfo();
    Q_INVOKABLE QString &extNum();
    Q_INVOKABLE QString &loopNum();
    Q_INVOKABLE QString &addrNum();
    Q_INVOKABLE QString &currentState();
    Q_INVOKABLE QString &deviceNum();
    Q_INVOKABLE QString &equipmentModel();
    Q_INVOKABLE QString &sysOfDevice();
    Q_INVOKABLE QString &buildingName();
    Q_INVOKABLE QString &floorOfDevice();
    Q_INVOKABLE QString &deviceLocation();
    Q_INVOKABLE QString &manufacturers();
    Q_INVOKABLE QString &periodOfValidity();

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
