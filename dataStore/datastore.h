#ifndef DATASTORE_H
#define DATASTORE_H
#include <QHash>
#include "graphicsWidget/graphicsview.h"
#include "graphicsWidget/graphicsitem.h"
/**
 * @brief The DataStore class
 * 用来保存不同类型的报警数据
 */
class DataStore
{
public:
     DataStore();
     ~DataStore();
     static QHash<QString, QList<QGraphicsItem *> > &getTypeItemHash();
     static QList<QGraphicsItem*> &getTypeItemList(const QString &type);
     static QGraphicsItem* getTypeItem(const QString &type,int pos);
     static void deleteTypeItems(const QString &type);
     static void deleteTypeItem(QGraphicsItem*item);
     static void deleteTypeItem(const QString &type,QGraphicsItem*item);
     static void deleteTypeItem(const QString &type,int pos);
     static void insertTypeItem(const QString &type,QGraphicsItem*item);
     static void clearTypeItem();
     static int numOfTypeItem(const QString &type);
     static GraphicsView *itemDisplayView(GraphicsItem *item);
private:
     static QHash<QString,QList<QGraphicsItem*> >m_typeItemHash;

};

#endif // DATASTORE_H
