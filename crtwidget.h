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
#include "communication/TcpLink.h"
#include "database/sqlitemanager.h"
#include "database/sqlmanager.h"
#include "dataStore/abstractdataprotocol.h"
#include "dataStore/serialdataprotocol.h"
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
    void logWidgetClose();
    void alarmChanged(QString alarm);
    void alarmStatistics(const QString &type);
private:
    void initWidget();
    void alarmDataOnTable();
private:
    QWidget *m_alarmContainer;
    QWidget *m_toolBarContainer;
    ArchitePlanView *m_architePlanView;
    InfoTableView*m_infoTableView;
    QQuickView *m_loginQuickView;
    QQuickView *m_alarmQuickView;
    QQmlApplicationEngine *m_settingViewEngine;
    SqlManager *m_sqliteManager;
    QObject *m_alarmObj;
};

#endif // CRTWIDGET_H
