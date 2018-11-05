#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include<QGraphicsView>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include "graphicsscene.h"
#include <QGLWidget>
#include <QGraphicsPixmapItem>
#include <QtOpenGL>
class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = Q_NULLPTR);
    ~GraphicsView();
    QPixmap graphicsPixmap() const;
    QString pixmapName();
    QList<QGraphicsItem *> getItemList();
    QGraphicsItem * getItem(int pos);
public slots:
    void zoom(qreal scaleValue);
    void loadPixmap(const QString &fileName);

protected:
    //    void paintEvent(QPaintEvent*event);
    //    void mouseMoveEvent(QMouseEvent*event);
    //    void mousePressEvent(QMouseEvent*event);
    //    void mouseReleaseEvent(QMouseEvent*event);

    void wheelEvent(QWheelEvent*event);
private:
    qreal m_scale;
    GraphicsScene *m_scene;
    //QGraphicsSvgItem *m_svgItem;
    QGraphicsPixmapItem *m_pixmapItem;
    QString m_pixmapName;
};

#endif // GRAPHICSVIEW_H
