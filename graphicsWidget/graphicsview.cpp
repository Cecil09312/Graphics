#include "graphicsview.h"
#include <QDebug>
#include <QTransform>

GraphicsView::GraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
   zoom(1.5);
}

void GraphicsView::zoom(qreal scaleValue)
{
    if(scaleValue>1.0&&scaleValue<20.0)
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


//void GraphicsView::paintEvent(QPaintEvent *event)
//{
//    qDebug() << event->rect().x();
//}

//void GraphicsView::mouseMoveEvent(QMouseEvent *event)
//{

//}

//void GraphicsView::mousePressEvent(QMouseEvent *event)
//{

//}

//void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
//{

//}
