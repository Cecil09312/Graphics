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
    Q_INVOKABLE bool buildNameIsExist(const QString &name);
    GlobalGraphicsItem *itemFromBuildingName(const QString &buildingName);
    void retranslate();
    QList<GlobalGraphicsItem *> &currentItemList();
    void sortItemList(const QList<QString> &nameList);
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
    void setCurPersonOnDuty(GlobalGraphicsItem*item,const QString &person);
    void setItemIcon(GlobalGraphicsItem*item,const QString &icon);
    void setItemSize(GlobalGraphicsItem*item,qreal size);
    void saveGeneralLayoutItems();
    void deleteItems();



protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*event);
    void mousePressEvent(QGraphicsSceneMouseEvent*event);
private:
    QPointF m_pointF;
    QMenu *m_menu;
    QAction *m_removeItemAction;
   // QAction *m_removeSelectItemAction;
    QAction *m_editItemAction;
    QAction *m_clearItemAction;
    QAction *m_goToAchitePlanAction;
   // QAction *m_saveGeneralLayoutItemsAction;
    int m_num;
    QQuickView *m_globalItemSettingView;
    QObject *m_globalItemObj;
    QList<GlobalGraphicsItem *>m_globalCurrentItemList;
};

#endif // GLOBALGRAPHICSSCENE_H
