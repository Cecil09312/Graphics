#ifndef DATASTORE_H
#define DATASTORE_H
#include <QHash>
#include "graphicsWidget/graphicsview.h"
#include "graphicsWidget/graphicsitem.h"
/**
 * @brief The DataStore class
 * 用来保存不同类型的报警数据
 */

struct DataInfo
{
    QString m_extNum;//分机号
    QString m_loopNum;//回路号
    QString m_addrNum;//地址号
    QString m_networkNum;//网络号
};
class DataStore
{
public:
     DataStore();
     ~DataStore();
     static QHash<QString, QList<QGraphicsItem *> > &getTypeItemHash();
     static QList<QGraphicsItem *> getTypeItemList(const QString &type);
     static QGraphicsItem* getTypeItem(const QString &type,int pos);
     static void deleteTypeItems(const QString &type);
     static void deleteTypeItem(QGraphicsItem*item);
     static void deleteTypeItem(const QString &type,QGraphicsItem*item);
     static void deleteTypeItem(const QString &type,int pos);
     static void insertTypeItem(const QString &type,QGraphicsItem*item);
     static bool deleteType(const QString &type, const QString&extNum, const QString&loopNum, const QString&addrNum, const QString &networkNum);
     static void deleteTypeItem(const QString&extNum, const QString&loopNum, const QString&addrNum, const QString &networkNum);
     static void insertTypeNoItem(const QString &type, const QString&extNum, const QString&loopNum, const QString&addrNum, const QString &networkNum);
     static void clearTypeItem();
     static int numOfTypeItem(const QString &type);
     static GraphicsView *itemDisplayView(GraphicsItem *item);
     static bool haveTypeItem(const QString &type, const QString&extNum, const QString&loopNum, const QString&addrNum, const QString &networkNum);
     static QHash<QString,QList<DataInfo*> >&getTypeNoItemHash();
     static QString getTypeNoItemKey(DataInfo*dataInfo);
     static void deleteDataInfo(DataInfo*dataInfo);
     static indexOfItem(const QString &extNum,const QString &loopNum,const QString &addrNum,const QString &networkNum,const QString &alarmType);

private:
     static QHash<QString,QList<QGraphicsItem*> >m_typeItemHash;
     static QHash<QString,QList<DataInfo*> >m_typeNoItemHash;


};

#endif // DATASTORE_H
