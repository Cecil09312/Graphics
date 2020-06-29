#ifndef SVGITEM_H
#define SVGITEM_H
#include <QGraphicsSvgItem>
#include<QStyleOptionGraphicsItem>
#include <QWidget>
#include <QSvgRenderer>
#include "drawimagethread.h"
#include <QtConcurrent/QtConcurrent>


//class SvgRenderThread:public QThread
//{
//    explicit SvgRenderThread(QObject *parent=nullptr);
//    ~SvgRenderThread();
//signals:
//   // void renderSvg(QPainter *painter);

//};
class SvgItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit SvgItem(QGraphicsItem *parent = Q_NULLPTR);
    ~SvgItem();
    QRectF boundingRect() const;
    void setSvgName(const QString &svgName);

signals:
    void renderSvg(QPainter *painter);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
private:
    QString m_svgName;
    QRectF m_rectF;
    bool m_isCanUpdate;

   // QSvgRenderer m_renderer;
};

#endif // SVGITEM_H
