#ifndef DATASTORE_H
#define DATASTORE_H
#include <QHash>
#include <QGraphicsItem>
/**
 * @brief The DataStore class
 * 用来保存不同类型的报警数据
 */
class DataStore
{
public:
     DataStore();
     ~DataStore();
     QHash<QString, QList<QGraphicsItem *> > &getTypeItemHash();
     QList<QGraphicsItem*> &getTypeItemList(const QString &type);
     QGraphicsItem* getTypeItem(const QString &type,int pos);
     void deleteTypeItems(const QString &type);
     void deleteTypeItem(QGraphicsItem*item);
     void deleteTypeItem(const QString &type,QGraphicsItem*item);
     void deleteTypeItem(const QString &type,int pos);
     void insertTypeItem(const QString &type,QGraphicsItem*item);
     void clearTypeItem();
     int numOfTypeItem(const QString &type);
private:
     QHash<QString,QList<QGraphicsItem*> >m_typeItemHash;
};

#endif // DATASTORE_H
