#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFrame>
#include "pixmapitem.h"
#include "graphicsview.h"
#include <QGraphicsScene>
#include <QPixmap>
#include "graphicsscene.h"
#include <QGLWidget>
class GraphicsWidget : public QFrame
{
    Q_OBJECT

public:
    GraphicsWidget(QWidget *parent = 0);
    ~GraphicsWidget();
    QPixmap graphicsPixmap() const;
    QString pixmapName();
public slots:
    void loadPixmap(const QString &fileName);
private:
    GraphicsView *m_graphicsView;
    GraphicsScene *m_scene;
    QGraphicsPixmapItem *m_pixmapItem;
    QString m_pixmapName;
};


#endif // WIDGET_H
