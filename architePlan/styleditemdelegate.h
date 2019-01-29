#ifndef STYLEDITEMDELEGATE_H
#define STYLEDITEMDELEGATE_H
#include <QStyledItemDelegate>

class StyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    StyledItemDelegate(QObject *parent = Q_NULLPTR);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // STYLEDITEMDELEGATE_H
