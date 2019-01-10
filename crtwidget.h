#ifndef CRTWIDGET_H
#define CRTWIDGET_H

#include <QWidget>
#include <QToolBar>
#include <QToolButton>
#include "architePlan/architeplanview.h"
#include "infoTable/infotableview.h"
#include <QQuickView>
#include <QOpenGLWidget>
#include <QQmlApplicationEngine>
#include "qmlTableModel/qmltablemodel.h"
#include "jsonEdit/qmlforjson.h"
#include "jsonEdit/itemiconinfotojson.h"
#include "commnication/TcpLink.h"

class CrtWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit CrtWidget(QWidget *parent = 0);
    ~CrtWidget();

public slots:
    void widgetExit();
    void loginWidgetShow();
    void settingWindowShow();
//    void toFirstFireAlarm();
//    void toLastFireAlarm();
    void logWidgetClose();
private:
    void initWidget();
private:
    QWidget *m_alarmContainer;
    QWidget *m_toolBarContainer;
    ArchitePlanView *m_architePlanView;
    InfoTableView*m_infoTableView;
    QQuickView *m_loginQuickView;
    QQuickView *m_alarmQuickView;
    QQmlApplicationEngine *m_settingViewEngine;

};

#endif // CRTWIDGET_H
