#ifndef SYSARCHITEPLANVIEW_H
#define SYSARCHITEPLANVIEW_H

#include <QWidget>
#include "graphicsWidget/graphicsview.h"
#include <QListView>
#include <QStackedWidget>
#include <QStringListModel>
#include "jsonEdit/qmlforjson.h"
#include "styleditemdelegate.h"
#include <QStandardItemModel>

class SysArchitePlanView : public QWidget
{
    Q_OBJECT
public:
    explicit SysArchitePlanView(QWidget *parent = nullptr);
    ~SysArchitePlanView();
    QVariant infoToJson();
    void retranslate();
    void setSysArchitePlanInfo();
signals:

public slots:
    void setSysDrawing(const QString& sysName,const QString &fileName);
    void setGraphicsViewScale(qreal scale);
    void currentGraphicsViewZoom(bool isZoomIn);
private:
    void init();


private:

    QListView *m_listView;
    QStandardItemModel *m_itemModel;
   // QHash<QString,GraphicsView*>m_graphicsViewHash;
    GraphicsView *m_graphicsView;
    StyledItemDelegate *m_delegate;
    QStringList m_sysViewNameList;
    QHash<QString,QVariant>m_imageNameHash;

};

#endif // SYSARCHITEPLANVIEW_H
