#ifndef SVGITEM_H
#define SVGITEM_H
#include <QGraphicsSvgItem>
#include<QStyleOptionGraphicsItem>
#include <QWidget>
#include <QSvgRenderer>
class SvgItem : public QGraphicsItem
{
public:
    explicit SvgItem(QGraphicsItem *parent = Q_NULLPTR);
    ~SvgItem();
    QRectF boundingRect() const;
    void setSvgName(const QString &svgName);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
private:
    QString m_svgName;
    QRectF m_rectF;
    bool m_isCanUpdate;
};

#endif // SVGITEM_H
