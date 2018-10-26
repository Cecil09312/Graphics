#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
#include <QGraphicsScene>
#include <QList>

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(QObject *parent = Q_NULLPTR);
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*event);
    void mousePressEvent(QGraphicsSceneMouseEvent*event);


private:
    QList<QGraphicsItem*>m_itemList;

};

#endif // GRAPHICSSCENE_H
