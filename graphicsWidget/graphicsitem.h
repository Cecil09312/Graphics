#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsSceneMouseEvent>

class GraphicsScene;

class GraphicsItem : public QGraphicsItem,public QObject
{


public:
    GraphicsItem(GraphicsScene *scene,QObject *parent=nullptr);
    ~GraphicsItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void setColor(const QColor &color);
    void startAnimation();
    void stopAnimation();
    void setAnimationDuration(int duration);
    void setAnimationLoopCount(int count);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
   // void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
private:
    QPropertyAnimation *m_propertyAnimation;
    QColor m_color;
    GraphicsScene *m_graphicsScene;

};

#endif // GRAPHICSITEM_H
