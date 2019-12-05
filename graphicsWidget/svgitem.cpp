#include "svgitem.h"
#include <QSvgRenderer>
#include <QDebug>
#include <QPainter>
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
    update();
}



void SvgItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    if(m_svgName.endsWith(".svg"))
    {
        QSvgRenderer renderer(m_svgName);
        renderer.render(painter,m_rectF);
    }
    else
    {
        painter->drawPixmap(m_rectF.toRect(),QPixmap(m_svgName));
    }

}
