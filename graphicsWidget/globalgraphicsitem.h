#ifndef GLOBALGRAPHICSITEM_H
#define GLOBALGRAPHICSITEM_H
#include <QGraphicsItem>
#include "globalgraphicsscene.h"
#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include <QPropertyAnimation>
#include <QTimeLine>
#include "drawimagethread.h"

class GlobalGraphicsItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    Q_PROPERTY(QString iconName READ iconName WRITE setIconName)
    Q_PROPERTY(qreal itemSize READ itemSize WRITE setItemSize)
    Q_PROPERTY(QString buildName READ buildName WRITE setBuildName)
    Q_PROPERTY(QString personOnDuty READ personOnDuty WRITE setPersonOnDuty)
public:
    GlobalGraphicsItem(GlobalGraphicsScene*scene= nullptr);
    ~GlobalGraphicsItem();
    QRectF boundingRect() const;

    void setIconName(const QString &name);
    QString iconName();
    qreal itemSize();
    void setItemSize(qreal radius);
    void setHoverText(const QString &hoverText);
    QString buildName();
    void setBuildName(const QString &name);
    void startAnimal(bool isStart);
    bool animalIsRunning();
    QString personOnDuty();
    void setPersonOnDuty(const QString&name);
    bool curAnimationRunState();
    void setCurAnimationRunState(bool curState);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
private:
    GlobalGraphicsScene *m_scene;
    QString m_iconName;
    qreal m_radius;

    QString m_hoverText;
    QString m_buildName;
    QString m_personOnDuty;
    QPropertyAnimation *m_propertyAnimation;
    QFont m_font;
    int m_scaleRunNum{0};
    bool m_curAnimationState;

};

#endif // GLOBALGRAPHICSITEM_H
