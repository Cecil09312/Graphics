#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include<QGraphicsView>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include "graphicsscene.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QGraphicsTextItem>
class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = Q_NULLPTR,int type = ArthitePlan);
    ~GraphicsView();
    QPixmap graphicsPixmap() const;
    QString pixmapName();
    QList<QGraphicsItem *> &getItemList();
    QGraphicsItem * getItem(int pos);
    bool haveAlarmType(const QString &type);
    bool haveAnyAlarm();
    QGraphicsScene* currentGraphicsScene(int type = ArthitePlan);
    void addGraphicsTextItem(const QPointF &pointF, const QString &alarmType);
    void removeGraphicsTextItem(const QString&alarmType);
    void clearGraphicsTextItem();
    QGraphicsTextItem *textItem(const QString&alarmType);

public slots:
    static void zoom(qreal scaleValue);
    void loadPixmap(const QString &fileName);
    void zoomIn();
    void zoomOut();
public:
    enum
    {
      ArthitePlan=1,
      SysArthitePlan
    };

protected:
    void wheelEvent(QWheelEvent*event);

private:
    static qreal m_scale;
    GraphicsScene *m_scene;
    QGraphicsScene*m_sysViewScene;
    QGraphicsSvgItem *m_svgItem;
    QString m_pixmapName;
    int m_viewType;
    QHash<QString,QGraphicsTextItem*>m_textItemHash;
    //QStringList m_alarmStringList;
};

#endif // GRAPHICSVIEW_H
