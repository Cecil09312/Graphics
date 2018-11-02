#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
#include <QGraphicsScene>
#include <QList>

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(QObject *parent = Q_NULLPTR);
    void addGraphicsItem(qreal ax, qreal ay);
    void addGraphicsItem(const QPointF &pointF);
    void removeGraphicsItem(qreal ax, qreal ay);
    void removeGraphicsItem(const QPointF &pointF);
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*event);
    void mousePressEvent(QGraphicsSceneMouseEvent*event);

public:
    QList<QGraphicsItem*>getItemList() const;
    QGraphicsItem*getItem(int pos) const;
private:
    QList<QGraphicsItem*>m_itemList;

};

#endif // GRAPHICSSCENE_H
