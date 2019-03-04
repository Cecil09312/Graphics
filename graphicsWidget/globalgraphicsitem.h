#ifndef GLOBALGRAPHICSITEM_H
#define GLOBALGRAPHICSITEM_H
#include <QGraphicsItem>
#include "globalgraphicsscene.h"
#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include <QGraphicsItemAnimation>
#include <QTimeLine>

class GlobalGraphicsItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    Q_PROPERTY(QString iconName READ iconName WRITE setIconName)
    Q_PROPERTY(qreal itemSize READ itemSize WRITE setItemSize)
    Q_PROPERTY(QString buildName READ buildName WRITE setBuildName)
public:
    GlobalGraphicsItem(GlobalGraphicsScene*scene= nullptr);
    ~GlobalGraphicsItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
    void setIconName(const QString &name);
    QString iconName();
    qreal itemSize();
    void setItemSize(qreal radius);
    void setHoverText(const QString &hoverText);
    QString buildName();
    void setBuildName(const QString &name);
    void startAnimal();
protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
private:
    GlobalGraphicsScene *m_scene;
    QString m_iconName;
    qreal m_radius;
    QString m_hoverText;
    QString m_buildName;
    //QGraphicsItemAnimation *m_itemAnimation;
    //QTimeLine *m_timeLine;
};

#endif // GLOBALGRAPHICSITEM_H
