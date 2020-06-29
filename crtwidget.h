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
#ifdef Q_OS_LINUX
#include "control/segfault.h"
#endif

//#include "LogMsg/logmsg.h"
//#include "LogMsg/debuglogmsg.h"
enum SerialState
{
    NoState=0,
    Connected,
    Disconnected
};

enum Language
{
    English,
    Chinese
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
    Q_INVOKABLE bool isEnglish();
    void setIsEnglish(bool isEnglish);

    void closeQuickView();
    void retranslate();
    void initArchiteView();
    void readDeviceOnlineInfoFromJson();
    void startTranslate();
    void setItemIconInfo();

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
    void communicationStatus(const QString &status, bool isOK, const QString &extNum="", const QString &networkNum="");
    void emergencyPowerState(const QString &status, bool isOK, const QString &extNum, const QString &loopNum,const QString &addrNum,
                             const QString &powerAddr,const QString &networkNum="",const QString &reMark="");
    void serialDataProcessing(const QByteArray&array);
    void tcpDataProcessing(const QByteArray&arrayValue);
    void openHelpFile();
    void sendAnalogCommand(quint8 networkNum, quint8 extNum, quint8 loopNum, quint8 addrNum, quint8 channelNum, quint8 analogType);
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
    void showDeviceOnlineSetting();
    void clearCurrentAlarm();
    void setDeviceOnlineState(QString sNetworkNum);
    void noItemInfoSetting(const QString &extNum, const QString &loopNum, const QString&addrNum,
                           const QString&networkNum, const QString &alarmType, const QString &alarmState,
                           const QString&time, const QString&remarks="", const QString &powerAddr="0");
    void noItemUpdate(const QString &extNum, const QString &loopNum, const QString &addrNum,
                      const QString &networkNum, const QString &oldAlarmType, const QString &newAlarmType,
                      const QString &newAlarmState, const QString &time, const QString &remarks="",const QString &powerAddr="0");

    void chAndEnSwitch();
    void allAlarmStatistics();
   /*测试*/
    void sendSeralData();



private:
    void closeSys();
    void initWidget();

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

    void setIndicator(bool state);
    void closeAllOnlineState();
    void insertOtherAlarmInfo(const QString &info,const QString&extNum="",const QString&networkNum="");
    void updateOtherAlarmInfo(const QString &alarmType, const QString &curState, const QString &extNum="", const QString &networkNum="");
    void clearCheckExtInfo();

    void saveOnlineInfoToJson();

private slots:
    void processViewsData(const QByteArray &array);
    void showOnlineView();
    void alarmDataOnTable();
    void showMainPowerView();
    void showStandbyPowerView();
    void showHandOrAutoView();
    void checkCurrentVersion();
    void updateTime();
    void setTransportState();
    void enableExtNum(QString extNum, QString networkNum, bool state);
    void updateHandOrAutoState();
    void updateAllOnlineState(int network,bool state);
    void updateOneOnlineState(int network, int index, bool state);
    void networkChangedOnlineState(int network);



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

    QQuickView *m_deviceOnlineSettingView;
    QObject *m_toolBarObj;
    QObject*m_extNumObj;
    QObject *m_settingObj;
    QObject*m_infoQueryObj;
    QObject *m_deviceOnleSettingObj;


    QQuickView *m_mainPowerView;
    QQuickView *m_standbyPowerView;
    QQuickView *m_handOrAutoView;
    QObject *m_mainPowerObj;
    QObject*m_standbyPowerObj;
    QObject *m_handOrAutoObj;

    SqliteManager *m_sqliteManager;
    QObject *m_alarmObj;
    QObject *m_resetLoginObj;
    QObject *m_loginObj;
    QObject *m_closeLoginObj;
    QString m_alarmInfoDbName;
    SerialDataProtocol *m_serialDataProtocol;
    AbstractDataProtocol *m_monitoringProtocol;
   // AbstractDataProtocol *m_indicatorProtocol;
    int m_monitoringPackageNum;
    FtpManager *m_ftpManager;

    int m_heartbeatIndex;
    bool m_tcpIsConnected;
    //QList<quint8>m_packageNumList;
    const int c_heartBeatTime { 5*1000};
    bool m_serialConnected ;
    CustomTimer *m_mainHeartBeatTimer;
    CustomTimer *m_checkSendDataTimer;
    CustomTimer *m_updateTimer;
    CustomTimer *m_setVisibleTimer;
    CustomTimer *m_controlCenterHeartbeatTimer;
    CustomTimer * m_checkExtNumTimer;
    QString m_alarmSqlInfo;
    QString m_updateAlarmSqlInfo;
    QProcess m_process;
    bool m_sendEnable;
    SerialState m_serialConnectState;
    bool m_tcpConnectState;
    bool m_serialCurState;
   // QTcpSocket *m_controlCenterSocket;
    QHash<quint8,QString>m_sysNameHash;

    QHash<QString,bool>m_extNumStateHash;
    QHash<QString,bool>m_extAndNetworkStateHash;
    QHash<QString,bool>m_extEnableHash;
    //QHash<QString,bool>m_allExtEnableHash;
    QHash<int,bool>m_allCheckedHash;
   // QHash<QString,CustomTimer*>m_checkExtNumHash;
    QHash<QString,int>m_extNumTimesHash;
    QHash<QString,SerialState> m_extOnlineStateHash;
    //QHash<int,QDateTime> m_dataTimeHash;
    bool m_sendDataResult;
    QList<QByteArray>m_readSerialDataList;
    QList<QByteArray>m_fireDataList;
    //QMutex m_mutex;
    const int c_mainHeartBeatTimeOut {90000};
    const int c_mainHeartBeatTime {30000};
    QHash<QString,QString>m_mainPowerStateHash;
    QHash<QString,QString>m_standbyPowerStateHash;
    QHash<QString,QString>m_handOrAutoStateHash;
    bool m_receiveDataIsSuccess;
    Language m_curLanguage{Chinese};
    QTranslator m_translator;
    bool m_translatorToEnglish{false};
    bool m_firstEnter{true};
    QStringList m_selectAlarmInfoList;
    QList<QString>m_typeList;

    QHash<quint8,QString>m_alarmTypeHash,m_commuStatusHash,m_faultStateHash,m_emergencyDeviceTypeHash,m_emergencyStateHash,m_emergencyTypeHash;
    bool m_isAnalog{false};
   // Language m_language;
    //QTranslator *m_translator;




};

#endif // CRTWIDGET_H
