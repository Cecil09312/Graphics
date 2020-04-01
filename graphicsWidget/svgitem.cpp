#include "svgitem.h"
#include <QSvgRenderer>
#include <QDebug>
#include <QPainter>
#include <QImage>
#include "control/controller.h"
#include <QtConcurrent/QtConcurrent>
SvgItem::SvgItem(QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    m_rectF = QRectF(0, 0, 1024, 768);

}

SvgItem::~SvgItem()
{

}

QRectF SvgItem::boundingRect() const
{
    return m_rectF;
}

void SvgItem::setSvgName(const QString &svgName)
{
    m_svgName = svgName;

    QPixmap pixMap = QPixmap(svgName);
    m_rectF=QRectF(0,0,pixMap.width(),pixMap.height());

    //update();
}


void SvgItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    if(m_svgName.endsWith(".svg"))
    {
        m_renderer.load(m_svgName);
        m_renderer.render(painter,m_rectF);
    }
    else
    {
        painter->drawPixmap(m_rectF.toRect(),QPixmap(m_svgName));
    }

}

