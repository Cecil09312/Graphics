#ifndef ARCHITEPLANVIEW_H
#define ARCHITEPLANVIEW_H

#include <QWidget>
#include <QTreeView>
#include <QStackedWidget>
#include <QTabWidget>
#include "graphicsWidget/graphicswidget.h"
#include <QStandardItemModel>
#include "treeview.h"
#include <QQuickView>

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
private:
    TreeView *m_treeView;
    QStackedWidget *m_stackedWidget;
    QTabWidget *m_tabWidget;
    QMap<int,GraphicsWidget *>m_widgetMap;
   // GraphicsWidget *m_graphicsWidget;
};

#endif // ARCHITEPLANVIEW_H
