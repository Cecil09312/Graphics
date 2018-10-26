#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "pixmapitem.h"
#include "graphicsview.h"
#include <QGraphicsScene>
#include <QPixmap>
#include "graphicsscene.h"
class GraphicsWidget : public QWidget
{
    Q_OBJECT

public:
    GraphicsWidget(QWidget *parent = 0);
    ~GraphicsWidget();
public slots:
    void loadPixmap(const QString &fileName);
private:
    //PixmapItem *m_pixmapItem;
    GraphicsView *m_graphicsView;
    GraphicsScene *m_scene;
   // QGraphicsSvgItem *m_svgItem;
    QGraphicsPixmapItem *m_pixmapItem;
};


#endif // WIDGET_H
