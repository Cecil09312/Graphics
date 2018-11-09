#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QParallelAnimationGroup>

class GraphicsScene;

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
    GraphicsItem(GraphicsScene *scene,QObject *parent=nullptr);
    ~GraphicsItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
    void startAnimation();
    void stopAnimation();
    void setAnimationDuration(int duration);
    void setAnimationLoopCount(int count);
    QPointF graphicsItemPos() const;
    void setColorStartValue(const QVariant &value);
    void setColorEndValue(const QVariant &value);
    void setScaleStartValue(const QVariant &value);
    void setScaleEndValue(const QVariant &value);

    void setColor(const QColor &color);
    QColor color() const;
    qreal radius() const;
    void setRadius(qreal radius);
    QString hoverText() const;
    void setHoverText(const QString &hoverText);
    QString itemText() const;
    void setItemText(const QString &itemText);
    QColor itemTextColor() const;
    void setItemTextColor(const QColor &color);
    QString typeName()const;
    void setTypeName(const QString &typeName);
    QString geoInfo() const;
    void setGeoInfo(const QString &geoInfo);

    QString iconName() const;
    void setIconName(const QString &iconName);

    bool isUseIcon() const;
    void setIsUseIcon(bool isUseIcon);
    QHash<QString,QVariant> itemInfo();

protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
private:
    QPropertyAnimation *m_colorAnimation;
    QPropertyAnimation *m_scaleAnimation;
    QParallelAnimationGroup *m_parallelAnimGroup;
    QColor m_color;
    GraphicsScene *m_graphicsScene;
    qreal m_radius;
    QString m_itemText;
    QString m_hoverText;
    QColor m_itemTextColor;
    QString m_geoInfo;
    QString m_typeName;
    bool m_isUseIcon;
    QString m_iconName;
    static int m_num;

//    QFont m_itemTextFont;
//    QFont m_hoverTextFont;
};

#endif // GRAPHICSITEM_H
