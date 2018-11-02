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
class ArchitePlanView : public QWidget
{
    Q_OBJECT
public:
    explicit ArchitePlanView(QWidget *parent = nullptr);
    ~ArchitePlanView();

signals:

public slots:

private:
    void initWidget();
    void saveArchiteInfo();
    void initFromJsonFile();
private:
    TreeView *m_treeView;
    QStackedWidget *m_stackedWidget;
    QTabWidget *m_tabWidget;
    QMap<int,GraphicsView *>m_widgetMap;
    const QString c_jsonFilePath=QCoreApplication::applicationDirPath()+"/treeView.json";

};

#endif // ARCHITEPLANVIEW_H
