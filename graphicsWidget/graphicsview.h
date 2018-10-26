#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include<QGraphicsView>
#include <QPaintEvent>
#include <QPainter>
class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = Q_NULLPTR);
public slots:
    void zoom(qreal scaleValue);

protected:
//    void paintEvent(QPaintEvent*event);
//    void mouseMoveEvent(QMouseEvent*event);
//    void mousePressEvent(QMouseEvent*event);
//    void mouseReleaseEvent(QMouseEvent*event);

    void wheelEvent(QWheelEvent*event);
private:
    qreal m_scale;
};

#endif // GRAPHICSVIEW_H
