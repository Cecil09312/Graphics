#ifndef DATASTORE_H
#define DATASTORE_H
#include <QHash>
#include "graphicsWidget/graphicsview.h"
#include "graphicsWidget/graphicsitem.h"
#include <QObject>
/**
 * @brief The DataStore class
 * 用来保存不同类型的报警数据
 */


class DataStore:public QObject
{
    Q_OBJECT
public:
    DataStore(QObject*parnet=nullptr);
    ~DataStore();
    QHash<QString, QList<QGraphicsItem *> > &getTypeItemHash();
    QList<QGraphicsItem *> getTypeItemList(const QString &type);
    QGraphicsItem* getTypeItem(const QString &type,int pos);
    void deleteTypeItems(const QString &type);
    void deleteTypeItem(QGraphicsItem*item);
    void deleteTypeItem(const QString &type,QGraphicsItem*item);
    void deleteTypeItem(const QString &type,int pos);
    void insertTypeItem(const QString &type,QGraphicsItem*item);
    void deleteType(const QString &type, const QString&extNum, const QString&loopNum, const QString&addrNum, const QString &networkNum, const QString &powerAddr="0");
    void deleteTypeItem(const QString&extNum, const QString&loopNum, const QString&addrNum, const QString &networkNum);
    void deleteTypeNoItem(const QString&extNum);
    void insertTypeNoItem(const QString &type, const QString&extNum, const QString&loopNum, const QString&addrNum, const QString &networkNum, const QString &powerAddr="0", const QString &timeStr="");
    void clearTypeItem();
    int numOfTypeItem(const QString &type);
    GraphicsView *itemDisplayView(GraphicsItem *item);
    bool haveTypeItem(const QString &type, const QString&extNum, const QString&loopNum, const QString&addrNum, const QString &networkNum);
    QHash<QString,QList<QString> >&getTypeNoItemHash();
    int indexOfItem(const QString &extNum,const QString &loopNum,const QString &addrNum,const QString &networkNum,const QString &alarmType);
    int indexOfTypeFromItem(const QString &alarmType,GraphicsItem*item);
    bool containsAlarms(const QString &alarm);
    bool containsAlarmsNoItem(const QString &alarm);
    int itemNum();
    void setItemNum(int num);
    QString &loopNum();
    QString &extNum();
    QString &networkNum();
    QString &sysName();
    int &channelNum();
    QString &analogValue();
    qreal &iconSize();
    QString &oneOperator();
    QString &powerAddr();
    void reInit();
    void clearStoreAlarm();
    int & priority();
    QString priorityType(int prioityValue);
    QString currentPriorityType();
    void updatePriority();
    QString getTypeNoItemTime(const QString &str);

    void setPriority();
signals:

    void curLoopAddr(const QString loopNum,const QString addrNum);
private:
    QHash<QString,QList<QGraphicsItem*> >m_typeItemHash;
    QHash<QString,QList<QString> >m_typeNoItemHash;
    QHash<QString,QString>m_typeNoItemTimeHash;
    int s_itemNum;
    QString m_loopNum;
    QString m_extNum;
    QString m_networkNum;
    QString m_powerAddr;
    QString m_sysName;
    int m_channelNum;
    QString m_analogValue;
    qreal m_iconSize;
    QString m_operator;
    int s_itemNumTemp;
    int m_priority;
    QHash<QString,int>m_priorityHash;


};

#endif // DATASTORE_H
