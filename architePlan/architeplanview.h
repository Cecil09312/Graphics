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
class ArchitePlanView : public QWidget
{
    Q_OBJECT
public:
    explicit ArchitePlanView(QWidget *parent = nullptr);
    ~ArchitePlanView();
    int numOfTypeAlarm(const QString &type);
    Q_INVOKABLE void setGlobalArchitePixmap(const QString &pixmapName);
    QMap<int,GraphicsView *> &getWidgetMap();
    int totalPage();
    int currentPage();
    Q_INVOKABLE void clearAlarm();
    Q_INVOKABLE void createAlarm(const QString &alarmTypeName);
    void eliminateAlarm(GraphicsItem *item);//消除报警
    void generateAlarm(const QString &alarmTypeName,GraphicsItem*item);
    void insertAlarmWidget(const QString &type,GraphicsView*view);
    void deleteAlarmWidget(const QString &type, GraphicsView *view);
    void clearAlarmWidget();
    void clearAlarmWidget(const QString &type);
    Q_INVOKABLE void setCurrentAlarmType(const QString &type);
    QString currentAlarmType();

    GraphicsView * viewToParentItem(QStandardItem* item);
    QList<GraphicsView *>viewsToChildItem(QStandardItem* item);

signals:
    void alarmHappend(const QString &alarmType);
    void toLastPage();
    void toFirstPage();
    void noPage();
    void normalPage();
    void alarmItem(GraphicsItem *item);
    void eliminateAlarmFromTable(GraphicsItem *item);
    void clearAlarmFromTable();
public slots:
    void firstFireAlarm();
    void lastFireAlarm();
    void currentGraphicsViewZoom(bool isZoomIn);
    void toPreviousPage();
    void toNextPage();
    void deleteViewFromItem(QStandardItem *item);



private:
    void initWidget();
    void saveArchiteInfo();
    void autoFitView(QGraphicsView *view);
    QHash<QString, QVariant> saveViewInfo(QStandardItem *item);
    void initFromJsonFile();
    void setViewFromJson(const QHash<QString, QVariant> &hash, QStandardItem *treeItem);
    void findFireAlarm(int pos);
    void saveOtherArchiteInfo();
    void setGlobalArchiteFromJson();
    void updateAlarmWidget(GraphicsView *currentView);
    void deleteAlarmWidget(GraphicsView *currentView);


private:
    TreeView *m_treeView;
    QStackedWidget *m_stackedWidget;
    QTabWidget *m_tabWidget;
    QWidget *m_globalGraphicsView;
    SysArchitePlanView *m_sysArchitePlanView;
    QMap<int,GraphicsView *>m_widgetMap;
    QHash<QString,QList<GraphicsView *> >m_alarmWidgetHash;
    QString m_currentAlarmType;
    const QString c_jsonFilePath=QCoreApplication::applicationDirPath()+"/treeView.json";
    QString m_globalArchitePlanPixmapName;
};

#endif // ARCHITEPLANVIEW_H
