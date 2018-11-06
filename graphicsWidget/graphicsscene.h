#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
#include <QGraphicsScene>
#include <QList>
#include <QMenu>
#include <QAction>
#include <QQuickView>

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(QObject *parent = Q_NULLPTR);
    ~GraphicsScene();
    void addGraphicsItem(qreal ax, qreal ay);
    void addGraphicsItem(const QPointF &pointF);
    void removeGraphicsItem(qreal ax, qreal ay);
    void removeGraphicsItem(const QPointF &pointF);
    void showMenu(const QPoint &point );
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*event);
    void mousePressEvent(QGraphicsSceneMouseEvent*event);

public:
     QList<QGraphicsItem*>getItemList() const;
     QGraphicsItem*getItem(int pos) const;
    // static QList<QGraphicsItem *> getTypeItemList(const QString &type);
private:
     QList<QGraphicsItem*>m_itemList;
     //static  QMap<QString,QList<QGraphicsItem*> >s_typeItemMap;
     QMenu *m_menu;
     QAction *m_deleteAction;
     QAction *m_editAction;
     QAction *m_clearAction;
     QAction *m_deleteSelectedAction;
     QAction *m_closeAction;
     QPointF m_currentPointF;
     QQuickView *m_itemSettingView;
};

#endif // GRAPHICSSCENE_H
