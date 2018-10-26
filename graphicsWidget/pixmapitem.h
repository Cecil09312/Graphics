#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
class PixmapItem : public QGraphicsPixmapItem
{
public:
    PixmapItem();
protected:
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // PIXMAPITEM_H
