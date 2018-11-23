#include "graphicsview.h"
#include <QDebug>
#include <QTransform>
#include <QScrollBar>
#include <QOpenGLWidget>
#include "openglWidget/glwidget.h"
#include "control/controller.h"
qreal GraphicsView::m_scale =1;
GraphicsView::GraphicsView(QWidget *parent, int type):
    QGraphicsView(parent),
    m_viewType(type)
{

    m_pixmapItem = new QGraphicsPixmapItem;
    zoom(1.5);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setViewport(new QOpenGLWidget(this));
    if(m_viewType==ArthitePlan)
    {
        setContextMenuPolicy(Qt::CustomContextMenu);
        m_scene = new GraphicsScene(this);
        setScene(m_scene);
        m_scene->addItem(m_pixmapItem);
        m_sysViewScene  = nullptr;
    }
    else
    {
        m_scene = nullptr;
        m_sysViewScene = new QGraphicsScene(this);
        m_sysViewScene->addItem(m_pixmapItem);
        setScene(m_sysViewScene);
    }

    connect(this,&GraphicsView::customContextMenuRequested,this,[=](const QPoint&/*pos*/)
    {
        if(m_scene!=nullptr)
        {
            m_scene->showMenu(QCursor::pos());
        }
    });

}

GraphicsView::~GraphicsView()
{

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
        zoomIn();
    }
    else
    {
        zoomOut();
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
    QList<QGraphicsItem*>itemList;
    if(m_scene!=nullptr)
    {
        itemList= m_scene->getItemList();
    }
    return itemList;
}

QGraphicsItem *GraphicsView::getItem(int pos)
{
    QGraphicsItem *item = nullptr;
    if(m_scene!=nullptr)
    {
        item = m_scene->getItem(pos);
    }

    return item;
}

void GraphicsView::loadPixmap(const QString &fileName)
{
    m_pixmapName = Controller::instance()->fileNameFromQml(fileName);
    m_pixmapItem->setPixmap(QPixmap(m_pixmapName));
}

void GraphicsView::zoomIn()
{
    //qDebug() <<"m_scale" <<m_scale;
    scale(m_scale,m_scale);
}

void GraphicsView::zoomOut()
{
    scale(1.0/m_scale,1.0/m_scale);
}
