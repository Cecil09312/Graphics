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
    GraphicsView(QWidget *parent = Q_NULLPTR,int type = ArthitePlan);
    ~GraphicsView();
    QPixmap graphicsPixmap() const;
    QString pixmapName();
    QList<QGraphicsItem *> getItemList();
    QGraphicsItem * getItem(int pos);

public slots:
    void zoom(qreal scaleValue);
    void loadPixmap(const QString &fileName);
public:
    enum
    {
      ArthitePlan=1,
      SysArthitePlan
    };

protected:
    void wheelEvent(QWheelEvent*event);

private:
    qreal m_scale;
    GraphicsScene *m_scene;
    QGraphicsScene*m_sysViewScene;
    //QGraphicsSvgItem *m_svgItem;
    QGraphicsPixmapItem *m_pixmapItem;
    QString m_pixmapName;
    int m_viewType;
};

#endif // GRAPHICSVIEW_H
