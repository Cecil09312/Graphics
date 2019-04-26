#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
#include <QGraphicsScene>
#include <QList>
#include <QMenu>
#include <QAction>
#include <QQuickView>
#include <QActionGroup>

class GraphicsItem;
class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(QObject *parent = Q_NULLPTR);
    ~GraphicsScene();
    void addGraphicsItem(qreal ax, qreal ay);
    void addGraphicsItem(const QPointF &pointF);
    void removeGraphicsItem(qreal ax, qreal ay);
    void removeGraphicsItem(const QPointF &pointF);
    void showMenu(const QPoint &point );
    QPointF currentScenePos();
    QList<QGraphicsItem *> &getItemList();
    QGraphicsItem*getItem(int pos) const;
    void setItemInfo(GraphicsItem*item, const QHash<QString,QVariant>&itemHash);
    bool isHavingAlarms();
    //void setItemFromIconIndex();

signals:
    void createItem(GraphicsItem *item);signals:


protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*event);
    void mousePressEvent(QGraphicsSceneMouseEvent*event);

private slots:
    void setItemColor(QColor color);
    void setItemSize(qreal size);
    void setItemIcon(QString iconName);
    void setItemInfoFromType(const QString &type,const QString &info);
    void getAlarm(QString extNum, QString loopNum, QString addrNum,QString networkNum, QString alarmState);
    void clearAlarms();
    void setItemsIcon(int index,QString iconName);
    void setItemsEquipmentModel(int index,QString device);
    void setItemsManufacturers(int index,QString manufacturers);
    void setItemsPeriodOfValidity(int index,QString periodOfValidity);
    void restoreAlarm(QString extNum,QString loopNum, QString addrNum,QString networkNum);
private:
    void init();

private:
    QList<QGraphicsItem*>m_itemList;
    QMenu *m_graphicsItemSettingMenu;
    QActionGroup *m_modeActionGroup;
    QAction *m_deleteAction;
    QAction *m_editAction;
    QAction *m_clearAction;
    QAction *m_deleteSelectedAction;
    QAction *m_closeAction;
    QAction *m_handDragAction;
    QAction *m_rubberBandDragAction;
    QAction *m_analogAlarmAction;
    QAction *m_maintenanceAction;
    QPointF m_currentPointF;
    QQuickView *m_itemSettingView;
    QQuickView *m_analogAlarmView;
    QQuickView *m_maintenanceView;
    QObject *m_itemSettingObj;
    QObject *m_analogAlarmObj;
};

#endif // GRAPHICSSCENE_H
