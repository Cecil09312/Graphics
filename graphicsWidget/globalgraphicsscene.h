#ifndef GLOBALGRAPHICSSCENE_H
#define GLOBALGRAPHICSSCENE_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSvgItem>
#include <QMenu>
#include <QAction>

class GlobalGraphicsItem;
class GlobalGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GlobalGraphicsScene(QObject *parent = Q_NULLPTR);
    ~GlobalGraphicsScene();
    void showMenu(const QPoint &point);
    GlobalGraphicsItem *addGlobalGraphicsItem(QPointF point);
    void clearGraphicsItem();
signals:
    void editItem();
    void goToArchitePlan(GlobalGraphicsItem*item);
    void addGlobalItem(GlobalGraphicsItem*item);
    void deleteGlobalItem(GlobalGraphicsItem*item);
    void clearItem();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*event);
    void mousePressEvent(QGraphicsSceneMouseEvent*event);
private:
    QPointF m_pointF;
    QMenu *m_menu;
    QAction *m_removeItemAction;
    QAction *m_removeSelectItemAction;
    QAction *m_editItemAction;
    QAction *m_clearItemAction;
    QAction *m_goToAchitePlanAction;
    int m_num;

};

#endif // GLOBALGRAPHICSSCENE_H
