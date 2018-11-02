#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
class GraphicsScene;

class GraphicsItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor/* NOTIFY colorChanged*/)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius/* NOTIFY radiusChanged*/)
    Q_PROPERTY(QString hoverText READ hoverText WRITE setHoverText/* NOTIFY hoverTextChanged*/)
    Q_PROPERTY(QString itemText READ itemText WRITE setItemText/* NOTIFY itemTextChanged*/)
    Q_PROPERTY(QColor itemTextColor READ itemTextColor WRITE setItemTextColor/* NOTIFY itemTextColorChanged*/)
public:
    GraphicsItem(GraphicsScene *scene,QObject *parent=nullptr);
    ~GraphicsItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void startAnimation();
    void stopAnimation();
    void setAnimationDuration(int duration);
    void setAnimationLoopCount(int count);
    QPointF graphicsItemPos() const;
    void setAnimationStartValue(const QVariant &value);
    void setAnimationEndValue(const QVariant &value);

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

protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
private:
    QPropertyAnimation *m_propertyAnimation;
    QColor m_color;
    GraphicsScene *m_graphicsScene;
    qreal m_radius;
    QString m_itemText;
    QString m_hoverText;
    QColor m_itemTextColor;
//    QFont m_itemTextFont;
//    QFont m_hoverTextFont;
};

#endif // GRAPHICSITEM_H
