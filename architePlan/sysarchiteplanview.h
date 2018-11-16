#ifndef SYSARCHITEPLANVIEW_H
#define SYSARCHITEPLANVIEW_H

#include <QWidget>
#include "graphicsWidget/graphicsview.h"
#include <QListView>
#include <QStackedWidget>
#include <QStringListModel>

class SysArchitePlanView : public QWidget
{
    Q_OBJECT
public:
    explicit SysArchitePlanView(QWidget *parent = nullptr);

signals:

public slots:
private:
    void init();
private:
    QStackedWidget *m_stackedWidget;
    QListView *m_listView;
    QStringListModel *m_strListModel;
    QHash<QString,GraphicsView*>m_graphicsViewHash;
};

#endif // SYSARCHITEPLANVIEW_H
