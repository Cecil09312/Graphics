#ifndef ARCHITEPLANVIEW_H
#define ARCHITEPLANVIEW_H

#include <QWidget>
#include <QTreeView>
#include <QStackedWidget>
#include <QTabWidget>
#include "graphicsWidget/graphicsview.h"
#include <QStandardItemModel>
#include "treeview.h"
#include <QQuickView>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QTextToSpeech>
#include "architePlan/sysarchiteplanview.h"
#include "graphicsWidget/globalgraphicsview.h"
#include "graphicsWidget/globalgraphicsitem.h"
#include "database/sqlitemanager.h"
#include "database/sqlitemanager.h"
#include <QTimer>

class ArchitePlanView : public QWidget
{
    Q_OBJECT
public:
    explicit ArchitePlanView(QWidget *parent = nullptr);
    ~ArchitePlanView();
    int numOfTypeAlarm(const QString &type);//type类型的报警数目
    static QMap<int,GraphicsView *> &getWidgetMap();
    int totalPage();//总页数
    int currentPage();//当前页

    void eliminateAlarm(GraphicsItem *item);//消除报警
    void generateAlarm(const QString &alarmTypeName, GraphicsItem*item, GraphicsView *view, bool isAnalog=false);
    void insertAlarmWidget(const QString &type,GraphicsView*view);
    void deleteAlarmWidget(const QString &type, GraphicsView *view);
    void clearAlarmWidget();
    void clearAlarmWidget(const QString &type);

    QString currentAlarmType();

    GraphicsView * viewFromChildItem(QStandardItem* childItem);
    QList<GraphicsView *> viewsFromParentItem(QStandardItem* parentItem);
    QList<GraphicsView *> haveAlarms(const QString &alarm);
    void autoFitView(QGraphicsView *view);

    QStandardItem*getParnentItemFromView(GraphicsView*view);
    void saveArchiteInfoToDb();
    QStandardItem*getItemFromView(GraphicsView*view);
    GraphicsView* currentGraphicsView();
    bool havingAlarms();
    void saveArchiteInfo();//保存建筑平面信息
    void saveOtherArchiteInfo();//保存建筑平面信息以外的其它信息(包括系统图和总平面布局图)
    void saveInfo();//保存信息
    void createAlarm(GraphicsItem *item,const QString &alarmTime=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));//生成报警
    GraphicsItem *itemFormInfo(const QString &extNum, const QString &loopNum,
                               const QString &addressNum, const QString &networkNum);
    void updateAlarmState(const QString &extNum, const QString &loopNum,
                          const QString &addressNum, const QString &networkNum,const QString &curAlarmState);//更新报警状态
    Q_INVOKABLE QString architeInfoDbName();
    Q_INVOKABLE void setGlobalArchitePixmap(const QString &pixmapName);

    Q_INVOKABLE void createAlarm(const QString &extNum, const QString &loopNum,
                                 const QString &addressNum, const QString &networkNum, const QString &alarmTypeName,
                                 bool isAnalog=false, const QString &alarmTime=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));//生成报警

    Q_INVOKABLE void clearAlarm(bool alarmColorRedu=false);//清空报警
    Q_INVOKABLE void toAlarmView();
    Q_INVOKABLE void setCurrentAlarmType(const QString &type);//设置当前报警类型
    Q_INVOKABLE void toArchitePlan(const QString &extNum, const QString &loopNum,
                                   const QString &addressNum,const QString &networkNum);
    Q_INVOKABLE void saveMySqlInfo(const QString &hostName,const QString &userName,
                                    const QString &password,const QString &databaseName,int port);//将数据保存到mysql数据库
    QString deviceSysName(const QString &extNum);//设备系统名

signals:
    void alarmHappend(const QString &alarmType);
    void toLastPage();
    void toFirstPage();
    void noPage();
    void normalPage();
    void alarmItem(GraphicsItem *item);
    void eliminateAlarmFromTable(GraphicsItem *item);
    void clearAlarmFromTable();
    void editGlobalItem();
    void reduInstruction(bool alarmColorRedu);
    void tabIndex(int index);
    void alarmStateUpdate(const QString &extNum, const QString &loopNum,
                          const QString &addressNum, const QString &networkNum,const QString &curAlarmState);

public slots:
    void firstFireAlarm();
    void lastFireAlarm();
    void currentGraphicsViewZoom(bool isZoomIn);
    void toPreviousPage();
    void toNextPage();
    void deleteViewFromItem(QStandardItem *item);
    void viewsAutoSwitch();
    void startAutoSwitch(bool isAuto);
    void eliminateAlarm(const QString &extNum, const QString &loopNum,
                        const QString &addrNum, const QString &networkNum);//消除报警

    void setItemSize(qreal size);
    void setItemIcon(QString iconName);
    void setItemInfoFromType(const QString &type,const QString &info);

    void setItemsIcon(int index,QString iconName);
    void setItemsEquipmentModel(int index,QString device);
    void setItemsManufacturers(int index,QString manufacturers);
    void setItemsPeriodOfValidity(int index,QString periodOfValidity);

private:
    void initWidget();
    void showMenu(const QPoint &point );
    QHash<QString, QVariant> saveViewInfo(QStandardItem *item);
    void initFromJsonFile();
    void initFromDataBase(GraphicsView *view, const QString &buildingName, const QString &floor);
    GraphicsView* setViewFromJson(const QHash<QString, QVariant> &hash, QStandardItem *treeItem);
    void findFireAlarm(int pos);

    void setGlobalArchiteFromJson();
    void updateAlarmWidget(GraphicsView *currentView);
    void deleteAlarmWidget(GraphicsView *currentView);
private:
    TreeView *m_treeView;
    QStackedWidget *m_stackedWidget;
    QTabWidget *m_tabWidget;
    GlobalGraphicsView *m_globalGraphicsView;
    SysArchitePlanView *m_sysArchitePlanView;
    static QMap<int,GraphicsView *>m_widgetMap;
    QHash<QString,QList<GraphicsView *> >m_alarmWidgetHash;
    QString m_currentAlarmType;
    const QString c_jsonFilePath=QCoreApplication::applicationDirPath()+"/treeView.json";
    QString m_globalArchitePlanPixmapName;
    QHash<GlobalGraphicsItem*,QStandardItem*>m_globalToArchitePlanHash;
    QTimer *m_autoSwitchTimer;
    QList<GraphicsView *>m_alarmViewList;
    int m_alarmPos;
    QHash<QStandardItem*,GraphicsView *>m_itemToViewHash;

    SqlManager *m_sqliteManager;
    QString m_architeInfoDbName;
    int m_itemInfoTableSize,m_globalArchiteTableSize;
    QHash<GraphicsItem*,int>m_speechTextPosFromItemHash;

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

#endif // ARCHITEPLANVIEW_H
