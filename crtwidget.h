#ifndef CRTWIDGET_H
#define CRTWIDGET_H

#include <QWidget>
#include <QToolBar>
#include <QToolButton>
#include "architePlan/architeplanview.h"
#include "infoTable/infotableview.h"
#include <QQuickView>
//#include <QOpenGLWidget>
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
#include <QMouseEvent>
#include <QMessageBox>
//#include "LogMsg/logmsg.h"
//#include "LogMsg/debuglogmsg.h"
enum SerialState
{
    NoState=0,
    Connected,
    Disconnected
};

class CrtWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CrtWidget(QWidget *parent = nullptr);
    ~CrtWidget();
    Q_INVOKABLE QString alarmInfoDbName();
    Q_INVOKABLE void queryViewShow();
    Q_INVOKABLE void transportIndicator(bool isOk);//传输指示灯


    void closeQuickView();

protected:
    void closeEvent(QCloseEvent *event);
signals:
    void getSerialData();
    void getTcpData();
    void checkExtNum(quint8 extNum,quint8 networkNum);

public slots:
    void widgetExit();
    void closeLogInView();
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
    void clearVoice();//
    void setIndicatorState(bool isOk);
    void resetLoginState(bool isOk);
    void resetLoginViewShow();
    void resetLoginViewClose();
    void setControlCenterEnable(bool enable);
    void closeControlCenterHeartbeat();
    void reSendAllCmd();
    void closeAll();
   /*测试*/
    void sendSeralData();

private:
    void closeSys();
    void initWidget();
    void alarmDataOnTable();
    bool setSysTime(const QDateTime &dateTime);
    void sendFireInfo(quint8 extNum, quint8 loopNum, quint8 addrNum, const QString &dateTimeStr);
    void reSendCmd();//广播重传指令
    void reSendUnicastCmd(quint8 extNum, quint8 networkNum);
    void startProcess(const QString &cmd);
    void resetAllState();
    void sendControlCenterHeartbeat();
    void sendAlarmInfo(quint8 sysNum,quint8 alarmType,quint8 alarmState,quint8 runState,const QString &timeStr);
    void sendPowerState(quint8 sysNum, quint8 mainPowerState, quint8 reservationState,quint8 runState, const QString &timeStr);
    void sendHandOrAutoState(quint8 sysNum, quint8 handOrAutoState,quint8 runState, const QString &timeStr);
    void setTransportState();
    void setIndicator(bool state);
    void closeAllOnlineState();

private slots:
    void processViewsData();
    void showOnlineView();


private:
    QWidget *m_alarmContainer;
    QWidget *m_toolBarContainer;
    ArchitePlanView *m_architePlanView;
    InfoTableView*m_infoTableView;
    QQuickView *m_loginQuickView;
    QQuickView *m_closeLoginView;
    QQuickView *m_resetLoginQuickView;
    QQuickView *m_alarmQuickView;
    QQuickView *m_settingView;
    QQuickView *m_infoQueryView;
    QQuickView *m_extNumStateView;
    QObject *m_toolBarObj;
    QObject*m_extNumObj;
    QObject *m_settingObj;
    SqlManager *m_sqliteManager;
    QObject *m_alarmObj;
    QObject *m_resetLoginObj;
    QObject *m_loginObj;
    QObject *m_closeLoginObj;
    QString m_alarmInfoDbName;
    AbstractDataProtocol *m_serialDataProtocol;
    AbstractDataProtocol *m_monitoringProtocol;
    AbstractDataProtocol *m_indicatorProtocol;
    int m_monitoringPackageNum;
    FtpManager *m_ftpManager;
    CustomTimer *m_controlCenterHeartbeatTimer;
    int m_heartbeatIndex;
    bool m_tcpIsConnected;
    //QList<quint8>m_packageNumList;
    const int c_heartBeatTime { 5*1000};
    const int c_updateTime {15};
    bool m_serialConnected ;
    CustomTimer *m_mainHeartBeatTimer;
    CustomTimer *m_checkSendDataTimer;
    QTimer *m_updateViewTimer;
    QString m_alarmSqlInfo;
    QString m_updateAlarmSqlInfo;
    QProcess m_process;
    bool m_sendEnable;
    SerialState m_serialConnectState;
    bool m_tcpConnectState;
    bool m_serialCurState;
   // QTcpSocket *m_controlCenterSocket;
    QHash<quint8,QString>m_sysNameHash;
    QHash<quint8,bool>m_extNumStateHash;
    QHash<quint8,CustomTimer*>m_checkExtNumHash;
    QHash<quint8,int>m_extNumTimesHash;
    QHash<quint8,SerialState> m_extOnlineStateHash;
    //QHash<int,QDateTime> m_dataTimeHash;
    bool m_sendDataResult;
    QList<QByteArray>m_readSerialDataList;
    QList<QByteArray>m_fireDataList;
    QMutex m_mutex;
    const int c_mainHeartBeatTimeOut {90000};
    const int c_mainHeartBeatTime {30000};

};

#endif // CRTWIDGET_H
