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

#include "communication/tcplink.h"

#include "database/sqlitemanager.h"
#include "database/sqlmanager.h"
#include "database/mysqlmanager.h"
#include "dataStore/abstractdataprotocol.h"
#include "dataStore/serialdataprotocol.h"
#include <QCloseEvent>
#include "dataStore/monitoringprotocol.h"
#include "dataStore/indicatordataprotocol.h"
#include "communication/ftpmanager.h"
#include <QDesktopServices>

class CrtWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit CrtWidget(QWidget *parent = 0);
    ~CrtWidget();
    Q_INVOKABLE QString alarmInfoDbName();
    Q_INVOKABLE void queryViewShow();

protected:
    void closeEvent(QCloseEvent *event);


public slots:
    void widgetExit();
    void loginWidgetShow();
    void settingWindowShow();
    void logWidgetClose();
    void alarmChanged(QString alarm);
    void alarmStatistics(const QString &type);
    void communicationStatus(const QString &status,bool isOK);
    void serialDataProcessing(const QByteArray&arrayValue);
    void tcpDataProcessing(const QByteArray&arrayValue);
    void openHelpFile();
    void sendAnalogCommand(quint8 networkNum, quint8 extNum, quint8 loopNum, quint8 addrNum, quint8 channelNum, const QString &analogType);
    void startReset();
   /*测试*/
  // void sendSeralData();

private:
    void closeSys();
    void initWidget();
    void alarmDataOnTable();
    bool setSysTime(const QDateTime &dateTime);
    void sendFireInfo(quint8 extNum, quint8 loopNum, quint8 addrNum, const QString &dateTimeStr);
    void setMySqlInfo();
   // void processAlarmHostInfo(quint8 type, QString &extNum, quint8 loopNum, quint8 addrNum, const QString &timeStr);
    void hideTaskBar(bool isHidden);
private:
    QWidget *m_alarmContainer;
    QWidget *m_toolBarContainer;
    ArchitePlanView *m_architePlanView;
    InfoTableView*m_infoTableView;
    QQuickView *m_loginQuickView;
    QQuickView *m_alarmQuickView;
    QQuickView *m_settingView;
    QQuickView *m_infoQueryView;
    SqlManager *m_sqliteManager;
    QObject *m_alarmObj;
    QString m_alarmInfoDbName;
    AbstractDataProtocol *m_serialDataProtocol;
    AbstractDataProtocol *m_monitoringProtocol;
    AbstractDataProtocol *m_indicatorProtocol;
    int m_monitoringPackageNum;
    FtpManager *m_ftpManager;
    QTimer *m_controlCenterHeartbeatTimer;
    int m_heartbeatIndex;
    bool m_tcpIsConnected;

//    QHash<QString,int>m_packageNumHash;
};

#endif // CRTWIDGET_H
