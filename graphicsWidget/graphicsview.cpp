#include "graphicsview.h"
#include <QDebug>
#include <QTransform>
#include <QScrollBar>
#include <QOpenGLWidget>
#include "openglWidget/glwidget.h"
GraphicsView::GraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    m_scene = new GraphicsScene(this);
    m_pixmapItem = new QGraphicsPixmapItem;
    setScene(m_scene);
    m_scene->addItem(m_pixmapItem);
    zoom(1.5);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setViewport(new QOpenGLWidget(this));
}

void GraphicsView::zoom(qreal scaleValue)
{
    if(scaleValue>1.0&&scaleValue<10.0)
    {
        m_scale = scaleValue;
    }
    else
    {
        m_scale =1;
    }

}


void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->angleDelta().ry()>0)
    {
        scale(m_scale,m_scale);
    }
    else
    {
        scale(1.0/m_scale,1.0/m_scale);
    }
    e->accept();
}


QPixmap GraphicsView::graphicsPixmap() const
{
    return m_pixmapItem->pixmap();
}

QString GraphicsView::pixmapName()
{
    return m_pixmapName;
}

QList<QGraphicsItem *> GraphicsView::getItemList()
{
    return m_scene->getItemList();
}

QGraphicsItem *GraphicsView::getItem(int pos)
{
    return m_scene->getItem(pos);
}

void GraphicsView::loadPixmap(const QString &fileName)
{
    m_pixmapName = fileName;
    if(m_pixmapName.startsWith("file:///"))
    {
        QList<QString> nameList=  m_pixmapName.split("file:///");
        int size = nameList.size();
        if(size>0)
        {
            m_pixmapName = nameList.at(size-1);
        }
    }

    m_pixmapItem->setPixmap(QPixmap(m_pixmapName));

}
