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

signals:
    void alarmHappend(const QString &alarmType);
public slots:
    void creatAlarm();
    void firstFireAlarm();
    void lastFireAlarm();
    void currentGraphicsViewZoom(bool isZoomIn);


private:
    void initWidget();
    void saveArchiteInfo();
    void autoFitView(QGraphicsView *view,const QString &alarmTypeName);
    QHash<QString, QVariant> saveViewInfo(QStandardItem *item);
    void initFromJsonFile();
    void setViewFromJson(const QHash<QString, QVariant> &hash, QStandardItem *treeItem);
    void findFireAlarm(int pos);
    void saveOtherArchiteInfo();
    void setGlobalArchiteFromJson();


private:
    TreeView *m_treeView;
    QStackedWidget *m_stackedWidget;
    QTabWidget *m_tabWidget;
    QWidget *m_globalGraphicsView;
    SysArchitePlanView *m_sysArchitePlanView;
    QMap<int,GraphicsView *>m_widgetMap;
    const QString c_jsonFilePath=QCoreApplication::applicationDirPath()+"/treeView.json";
    QTextToSpeech *m_textToSpeech;
    QString m_globalArchitePlanPixmapName;
};

#endif // ARCHITEPLANVIEW_H
