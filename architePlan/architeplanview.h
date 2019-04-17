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
#include <QTimer>
#include "database/sqlitemanager.h"
#include "database/sqlitemanager.h"

class ArchitePlanView : public QWidget
{
    Q_OBJECT
public:
    explicit ArchitePlanView(QWidget *parent = nullptr);
    ~ArchitePlanView();
    int numOfTypeAlarm(const QString &type);

    static QMap<int,GraphicsView *> &getWidgetMap();
    int totalPage();
    int currentPage();


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
    void saveArchiteInfo();
    void saveOtherArchiteInfo();
    Q_INVOKABLE QString architeInfoDbName();
    Q_INVOKABLE void setGlobalArchitePixmap(const QString &pixmapName);
    Q_INVOKABLE void clearAlarm(bool alarmColorRedu=false);
    Q_INVOKABLE void createAlarm(const QString &extNum, const QString &loopNum,
                                 const QString &addressNum, const QString &alarmTypeName,
                                 bool isAnalog=false,const QString &alarmTime=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
    Q_INVOKABLE void eliminateAlarm(const QString &extNum, const QString &loopNum,
                        const QString &addrNum);//消除报警
    Q_INVOKABLE void toAlarmView();
    Q_INVOKABLE void setCurrentAlarmType(const QString &type);
    Q_INVOKABLE void toArchitePlan(const QString &extNum, const QString &loopNum,
                                   const QString &addressNum);
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

public slots:
    void firstFireAlarm();
    void lastFireAlarm();
    void currentGraphicsViewZoom(bool isZoomIn);
    void toPreviousPage();
    void toNextPage();
    void deleteViewFromItem(QStandardItem *item);
    void viewsAutoSwitch();
    void startAutoSwitch(bool isAuto);
private:
    void initWidget();

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

    QList<QVariant> m_jsonValueList;

    SqlManager *m_sqliteManager;
    QString m_architeInfoDbName;
    int m_itemInfoTableSize,m_globalArchiteTableSize;
    QHash<GraphicsItem*,int>m_speechTextPosFromItemHash;

};

#endif // ARCHITEPLANVIEW_H
