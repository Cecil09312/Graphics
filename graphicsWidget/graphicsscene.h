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
    GraphicsItem* addGraphicsItem(qreal ax, qreal ay);
    GraphicsItem* addGraphicsItem(const QPointF &pointF);
    void removeGraphicsItem(qreal ax, qreal ay);
    void removeGraphicsItem(const QPointF &pointF);
    void removeGraphicsItem(QGraphicsItem*currentItem);

    QPointF currentScenePos();
    QList<QGraphicsItem *> &getItemList();
    QGraphicsItem*getItem(int pos) const;
    void setItemInfo(GraphicsItem*item, const QHash<QString,QVariant>&itemHash);
    bool isHavingAlarms();
    //void setItemFromIconIndex();
    void checkSettingItem();
    QString itemOldInfo(GraphicsItem*item);
    void clearItemOldInfo();

signals:
    void createItem(GraphicsItem *item);
    void addOneItem(GraphicsItem *item);
    void itemSetting(GraphicsItem *item);


protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*event);
    void mousePressEvent(QGraphicsSceneMouseEvent*event);

public slots:
    void setItemColor(QColor color);
    void setItemSize(qreal size);
    void setItemIcon(QString iconName);
    void setItemInfoFromType(const QString &type,const QString &info);
    void setItemsIcon(int index,QString iconName);
    void setItemsEquipmentModel(int index,QString device);
    void setItemsManufacturers(int index,QString manufacturers);
    void setItemsPeriodOfValidity(int index,QString periodOfValidity);
    void setDeviceInstallTime(int index,QString installTime);

private:
    QList<QGraphicsItem*>m_itemList;
    QPointF m_currentPointF;
    QList<GraphicsItem*>m_settingItemList;
    QHash<GraphicsItem*,QString>m_oldItemInfoHash;
};

#endif // GRAPHICSSCENE_H
