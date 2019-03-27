#include "styleditemdelegate.h"
#include <QPainter>

StyledItemDelegate::StyledItemDelegate(QObject *parent):
    QStyledItemDelegate(parent)

{

}

void StyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        painter->save();

        QVariant variant = index.data(Qt::UserRole+1);
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth( option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

        //QPainterPath画圆角矩形
        const qreal radius = 7;
        QPainterPath path;
        path.moveTo(rect.topRight() - QPointF(radius, 0));
        path.lineTo(rect.topLeft() + QPointF(radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));

        if(option.state.testFlag(QStyle::State_Selected))
        {
            painter->setPen(QPen(Qt::green));
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(path);
        }
        else if(option.state.testFlag(QStyle::State_MouseOver))
        {
            painter->setPen(QPen(Qt::yellow));
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(path);
        }
        else{


            painter->setPen(QPen(Qt::blue));
            painter->setBrush(QColor(229, 241, 255));
            painter->drawPath(path);
        }

        //绘制数据位置
        QRect dataRect = QRect(rect.left() +10, rect.top()+10, rect.width()-30, 20);
        QRect circle = QRect(dataRect.right()-5, rect.top()+10, 15, 15);
        if(option.state.testFlag(QStyle::State_Selected))
        {
            painter->setPen(QColor(Qt::green));
            painter->setBrush(Qt::green);

        }
        else if(option.state.testFlag(QStyle::State_MouseOver))
        {
            painter->setPen(QPen(Qt::yellow));
            painter->setBrush(Qt::yellow);

        }
        else
        {
            painter->setPen(QColor(Qt::blue));
            painter->setBrush(Qt::blue);
        }

        painter->drawEllipse(circle);
        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Times", 10, QFont::Medium));
        painter->drawText(dataRect,Qt::AlignCenter,variant.toString()); //绘制名字
        painter->restore();
    }
}

QSize StyledItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(150,60);
}
