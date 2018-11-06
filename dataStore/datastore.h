#ifndef DATASTORE_H
#define DATASTORE_H
#include <QHash>
#include <QGraphicsItem>

class DataStore
{

public:
     DataStore();
     QHash<QString, QList<QGraphicsItem *> > &getTypeItemHash();
     QList<QGraphicsItem*> &getTypeItemList(const QString &type);
     QGraphicsItem* getTypeItem(const QString &type,int pos);
     void deleteTypeItems(const QString &type);
     void deleteTypeItem(QGraphicsItem*item);
     void deleteTypeItem(const QString &type,QGraphicsItem*item);
     void deleteTypeItem(const QString &type,int pos);
     void insertTypeItem(const QString &type,QGraphicsItem*item);
     void clearTypeItem();
private:
     QHash<QString,QList<QGraphicsItem*> >m_typeItemHash;
};

#endif // DATASTORE_H
