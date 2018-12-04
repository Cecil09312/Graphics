#ifndef SYSARCHITEPLANVIEW_H
#define SYSARCHITEPLANVIEW_H

#include <QWidget>
#include "graphicsWidget/graphicsview.h"
#include <QListView>
#include <QStackedWidget>
#include <QStringListModel>
#include "jsonEdit/qmlforjson.h"

class SysArchitePlanView : public QWidget
{
    Q_OBJECT
public:
    explicit SysArchitePlanView(QWidget *parent = nullptr);
    ~SysArchitePlanView();
    QVariant infoToJson();
signals:

public slots:
    void setSysDrawing(const QString& sysName,const QString &fileName);
    void setGraphicsViewScale(qreal scale);
    void currentGraphicsViewZoom(bool isZoomIn);
private:
    void init();
    void setSysArchitePlanInfo();

private:
    QStackedWidget *m_stackedWidget;
    QListView *m_listView;
    QStringListModel *m_strListModel;
    QHash<QString,GraphicsView*>m_graphicsViewHash;

};

#endif // SYSARCHITEPLANVIEW_H
