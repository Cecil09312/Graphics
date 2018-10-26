#ifndef CRTWIDGET_H
#define CRTWIDGET_H

#include <QWidget>
#include <QToolBar>
#include <QToolButton>
#include "architePlan/architeplanview.h"
#include "infoTable/infotableview.h"
#include <QQuickView>

class CrtWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CrtWidget(QWidget *parent = 0);
    ~CrtWidget();

public slots:
    void widgetExit();
    void loginWidgetShow();
private:
    void initWidget();
private:
    QWidget *m_alarmContainer;
    QWidget *m_toolBarContainer;
    ArchitePlanView *m_architePlanView;
    InfoTableView*m_infoTableView;
    QQuickView *m_loginQuickView;
   // QQuickView *m_architeSettingView;
};

#endif // CRTWIDGET_H
