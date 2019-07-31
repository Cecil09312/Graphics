#ifndef GLOBALGRAPHICSSCENE_H
#define GLOBALGRAPHICSSCENE_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSvgItem>
#include <QMenu>
#include <QAction>
#include <QQuickView>

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
    void closeQuickView();
    Q_INVOKABLE QString currentBuildName();
    Q_INVOKABLE qreal currentItemSize();
    Q_INVOKABLE QString currentItemIcon();
    Q_INVOKABLE QString personOnDuty();
public slots:
    void setCurrentItemSize(qreal size);
    void setCurrentItemIcon(const QString &icon);
    void setCurrentBuildName(const QString &name);
    void setPersonOnDuty(const QString &personName);
signals:
    void editItem();
    void goToArchitePlan(GlobalGraphicsItem*item);
    void addGlobalItem(GlobalGraphicsItem*item);
    void deleteGlobalItem(GlobalGraphicsItem*item);
    void clearItem();
    void setBuildingName(GlobalGraphicsItem*item,const QString &name);

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
    QQuickView *m_globalItemSettingView;
    QObject *m_globalItemObj;
};

#endif // GLOBALGRAPHICSSCENE_H
