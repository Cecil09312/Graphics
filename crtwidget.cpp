#include "crtwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QQuickView>
#include <QQmlComponent>
#include <QQmlContext>
#include <QSplitter>
#include <QQuickItem>
#include "communication/seriallink.h"
#include "control/usermanager.h"
#include "control/controller.h"
#include <QHostAddress>
#include "communication/configurationmanager.h"
#include "database/sqlitemanager.h"
#include "database/sqlmanager.h"
#include <QProcess>
#include<time.h>
#ifdef Q_OS_WIN
#include <windows.h>

#elif Q_OS_LINUX
#define _SVID_SOURCE
#endif

CrtWidget::CrtWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_monitoringPackageNum(0)
{

    setWindowFlags(Qt::FramelessWindowHint|Qt::Window);
    QString dbName = QCoreApplication::applicationDirPath()+"/alarmInfo.db";
    m_alarmInfoDbName = dbName;
    initWidget();

    Controller::instance()->getOperatorInfo()->insertEvent(tr("系统开机"));
    m_sqliteManager->setDataBase("QSQLITE","alarmInfo","","","",dbName,888);
    m_sqliteManager->open();
    if(m_sqliteManager->isOpen())
    {
        QStringList tableNameList = m_sqliteManager->getTables();
        if(!tableNameList.contains("AlarmInfo"))
        {
            m_sqliteManager->executeQuery("create table AlarmInfo(分机号 text, 回路号 text,地址号 text,网络号 text,设备编码 text ,设备 text,报警类型 text,报警状态 text,报警时间 text,报警恢复时间 text,系统 text,建筑名称 text,楼层 text,位置 text,制造商 text,有效期 text,操作员 text)");
        }
    }


    m_infoTableView->hide();
    m_infoTableView->tableModel()->setDbDriver("QSQLITE");
    m_infoTableView->tableModel()->setDbName(dbName);
    m_infoTableView->tableModel()->setDbPort(888);
    m_infoTableView->tableModel()->setDbOpen(true);
    QStringList alarmInfoList,valueList;
    alarmInfoList << "分机号"<<"回路号"<<"地址号"<<"网络号"<<"设备编码"
                  << "设备"<<"报警类型"<<"报警状态"<<"报警时间"<<"报警恢复时间"<<"系统"<< "建筑名称"<<"楼层"<<"位置"<< "制造商" << "有效期" <<"操作员";

    for(int i=0;i<alarmInfoList.size();i++)
    {
        QString str = QString("'%%1'").arg(i+1);
        valueList.push_back(str);
    }
    QString sqlInfo = QString("insert into AlarmInfo (%1) values (%2)").arg(alarmInfoList.join(",")).arg(valueList.join(","));
    Controller::instance()->getCommObj()->connectLink();
    Controller::instance()->getTcpObj()->connectLink();
    connect(m_architePlanView,&ArchitePlanView::alarmHappend,this,&CrtWidget::alarmStatistics);
    connect(m_architePlanView,&ArchitePlanView::alarmItem,this,[=](GraphicsItem *item)
    {
        m_sqliteManager->executeQuery(sqlInfo.arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()).arg(item->deviceNum())
                                      .arg(item->equipmentModel()).arg(item->alarmType()).arg(item->currentState()).arg(item->getItemInfo().m_alarmTime)
                                      .arg(item->getItemInfo().m_alarmReplyTime).arg(item->sysOfDevice()).arg(item->buildingName())
                                      .arg(item->floorOfDevice()).arg(item->deviceLocation()).arg(item->manufacturers()).arg(item->periodOfValidity()).arg(item->deviceOperator()));
        alarmDataOnTable();
    });

    connect(m_infoTableView,&InfoTableView::tableValue,this,[=](QSqlRecord record)
    {
        QString extNum=  record.value("分机号").toString();
        QString loopNum=  record.value("回路号").toString();
        QString addrNum=  record.value("地址号").toString();
        QString networkNum=  record.value("网络号").toString();
        m_architePlanView->toArchitePlan(extNum,loopNum,addrNum,networkNum);

    });

    connect(m_infoTableView,&InfoTableView::fitToWiew,this,[=]()
    {
        GraphicsView *view = m_architePlanView->currentGraphicsView();
        if(view!=nullptr)
        {
            m_architePlanView->autoFitView(view);
        }

    });


    connect(m_architePlanView,&ArchitePlanView::clearAlarmFromTable,this,[=]()
    {
        QString eliminateTime =  QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复时间 ='%1',报警状态 = '正常' where 报警状态 != '正常'").arg(eliminateTime));
        alarmDataOnTable();
    });

    connect(m_architePlanView,&ArchitePlanView::eliminateAlarmFromTable,this,[=](GraphicsItem *item)
    {
        QString eliminateTime =  QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复时间 ='%1',报警状态 = '正常' where 报警状态 != '正常' and 设备编码 = '%2'").arg(eliminateTime).arg(item->deviceNum()));
        alarmDataOnTable();
    });

    connect(m_architePlanView,&ArchitePlanView::tabIndex,this,[=](int index)
    {
        if(index==1)
        {
            m_infoTableView->show();
        }
        else
        {
            m_infoTableView->hide();
        }
    });

    connect(Controller::instance()->getCommObj(),&AbstractLink::getData,this,&CrtWidget::serialDataProcessing);

    connect(Controller::instance()->getTcpObj(),&AbstractLink::getData,this,&CrtWidget::tcpDataProcessing);

    connect(m_ftpManager,&FtpManager::sendFileSuccess,this,[=](bool isOk)
    {
        QList<QByteArray> arrayList;
        if(isOk)
        {
            arrayList.push_back(QString::number(0x04).toLocal8Bit());
            arrayList.push_back(QString::number(0x00).toLocal8Bit());
        }
        else
        {
            arrayList.push_back(QString::number(0x04).toLocal8Bit());
            arrayList.push_back(QString::number(0x01).toLocal8Bit());
        }

        Controller::instance()->getIndicatorObj()->writeData(m_indicatorProtocol->dataPackage(arrayList));
    });

    connect(m_ftpManager,&FtpManager::ftpError,this,[=](const QString&error)
    {
        Q_UNUSED(error)
        QList<QByteArray> arrayList;
        arrayList.push_back(QString::number(0x03).toLocal8Bit());
        arrayList.push_back(QString::number(0x00).toLocal8Bit());
        Controller::instance()->getIndicatorObj()->writeData(m_indicatorProtocol->dataPackage(arrayList));
    });


    connect(m_ftpManager,&FtpManager::uploadProgress,this,[=](qint64 bytesSent, qint64 bytesTotal)
    {
        QList<QByteArray> arrayList;
        static int num=0;
        if(bytesSent>=bytesTotal)
        {
            num = 0;
        }
        if(num>0 && num%10==0)
        {
            arrayList.push_back(QString::number(0x02).toLocal8Bit());
            arrayList.push_back(QString::number(0x00).toLocal8Bit());
        }
        else
        {
            arrayList.push_back(QString::number(0x02).toLocal8Bit());
            arrayList.push_back(QString::number(0x01).toLocal8Bit());
        }
        num++;

        Controller::instance()->getIndicatorObj()->writeData(m_indicatorProtocol->dataPackage(arrayList));
    });

    Q_ASSERT(m_alarmObj);

    if(m_architePlanView->totalPage()>0)
    {
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
    }
    else
    {
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
    }

    QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()+1));
    connect(m_alarmObj,SIGNAL(currentAlarmType(QString)),this,SLOT(alarmChanged(QString)));
    connect(m_architePlanView,&ArchitePlanView::toFirstPage,this,[=]()
    {
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()+1));
    });

    connect(m_architePlanView,&ArchitePlanView::reduInstruction,this,[=](bool isOk)
    {
        QMetaObject::invokeMethod(m_alarmObj,"allAlarmClear",Q_ARG(QVariant,isOk));
    });

    connect(m_architePlanView,&ArchitePlanView::toLastPage,this,[=]()
    {
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()+1));
    });

    connect(m_architePlanView,&ArchitePlanView::normalPage,this,[=]()
    {
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()+1));
    });

    connect(m_architePlanView,&ArchitePlanView::noPage,this,[=]()
    {
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()+1));
    });


    connect(Controller::instance()->getCommObj(),&AbstractLink::isConnected,this,[=](bool connected)
    {
        if(connected)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//设备通信
            if(Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("设备通信故障")))
            {
                Controller::instance()->getSpeechObj()->removeAlarmText(tr("设备通信故障"));
            }
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("设备通信故障")))
            {
                Controller::instance()->getSpeechObj()->insertAlarmText(tr("设备通信故障"));
            }
        }
    });

    connect(Controller::instance()->getTcpObj(),&TcpLink::isConnected,this,[=](bool connected)
    {
        if(connected)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//中心通信
            if(Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("中心通信故障")))
            {
                Controller::instance()->getSpeechObj()->removeAlarmText(tr("中心通信故障"));
            }
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("中心通信故障")))
            {
                Controller::instance()->getSpeechObj()->insertAlarmText(tr("中心通信故障"));
            }
        }

    });
}

CrtWidget::~CrtWidget()
{
    Controller::instance()->getTransportInfo()->saveTransportInfoToJson();
    Controller::instance()->getOperatorInfo()->insertEvent(tr("系统关机"));
    m_sqliteManager->close();
    m_sqliteManager->deleteLater();
    m_infoQueryView->close();
    m_infoQueryView->deleteLater();
    delete m_alarmContainer;
    delete m_toolBarContainer;
    m_loginQuickView->deleteLater();
    m_settingView->deleteLater();
    delete m_serialDataProtocol ;
    delete m_monitoringProtocol;
    delete m_indicatorProtocol;
    m_ftpManager->deleteLater();
}

QString CrtWidget::alarmInfoDbName()
{
    return m_alarmInfoDbName;
}

void CrtWidget::queryViewShow()
{
    m_architePlanView->saveArchiteInfoToDb();
    m_infoQueryView->show();

}


void CrtWidget::closeEvent(QCloseEvent *event)
{

    Controller::instance()->getSerialConfigurationManager()->saveConfiguration();
    Controller::instance()->getTcpConfigurationManager()->saveConfiguration();

    m_loginQuickView->close();
    m_settingView->close();
    event->accept();
}


void CrtWidget::widgetExit()
{
    if(!m_architePlanView->havingAlarms())
    {
        /*
        QProcess process;
#ifdef Q_OS_WIN

            process.start("shutdown -s -t 0");
#elif Q_OS_LINUX
     process.start("poweroff");
#endif
        process.waitForStarted();
        process.waitForFinished();
*/
        close();
    }
    else
    {
        QMessageBox::critical(this,tr("警告！"),tr("有报警信息存在，清除后才能关闭"));
    }

}

void CrtWidget::loginWidgetShow()
{
    QObject *loginObj =  m_loginQuickView->rootObject();
    m_loginQuickView->show();
    if(loginObj!=nullptr)
    {
        QMetaObject::invokeMethod(loginObj,"clearLoginInfo");
    }
}

void CrtWidget::settingWindowShow()
{

    UserManager::UserRight userRight=   Controller::instance()->getUserRight();
    if(userRight!=UserManager::Employee)
    {
        m_settingView->close();
        m_settingView->show();
    }
    else
    {
        m_settingView->close();
        QMessageBox::critical(nullptr,tr("警告"),tr("此权限不允许打开，请重新登陆到其它权限。"));
    }

}

//void CrtWidget::toFirstFireAlarm()
//{
//    m_architePlanView->firstFireAlarm();
//}

//void CrtWidget::toLastFireAlarm()
//{
//    m_architePlanView->lastFireAlarm();
//}

void CrtWidget::logWidgetClose()
{
    m_loginQuickView->close();
}

void CrtWidget::alarmChanged(QString alarm)
{
    m_architePlanView->setCurrentAlarmType(alarm);
    int currentPage = m_architePlanView->currentPage();
    int totalPage = m_architePlanView->totalPage();

    alarmDataOnTable();
    Q_ASSERT(m_alarmObj);
    if(totalPage>1)
    {
        if(currentPage>0&&currentPage<totalPage-1)
        {
            QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
            QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,true));
        }
        else if(currentPage<=0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
            QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
        }
        else if(currentPage==totalPage-1)
        {
            QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
            QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,true));
        }
    }
    else if(totalPage==1)
    {
        if(currentPage<0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
            QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
            QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
        }

    }
    else if(totalPage<=0)
    {
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
    }
    QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,totalPage),Q_ARG(QVariant,currentPage+1));
}

void CrtWidget::alarmStatistics(const QString &type)
{
    Q_ASSERT(m_alarmObj);
    int typeNum = m_architePlanView->numOfTypeAlarm(type);
    if(type==tr("火警"))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }

        QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmText",Q_ARG(QVariant,typeNum));
    }
    else if(type==tr("联动"))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setLinkageAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setLinkageAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }

        QMetaObject::invokeMethod(m_alarmObj,"setLinkageText",Q_ARG(QVariant,typeNum));
    }
    else if(type==tr("监管"))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setSuperviseAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"orange"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setSuperviseAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }
        QMetaObject::invokeMethod(m_alarmObj,"setSuperviseText",Q_ARG(QVariant,typeNum));
    }
    else if(type==tr("故障"))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setfaultAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setfaultAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }
        QMetaObject::invokeMethod(m_alarmObj,"setFaultText",Q_ARG(QVariant,typeNum));
    }
    else if(type==tr("反馈"))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setFeedbackColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"blue"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setFeedbackColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }
        QMetaObject::invokeMethod(m_alarmObj,"setFeedbackText",Q_ARG(QVariant,typeNum));
    }
    else if(type==tr("屏蔽"))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setShieldAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"pink"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setShieldAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }
        QMetaObject::invokeMethod(m_alarmObj,"setShieldText",Q_ARG(QVariant,typeNum));
    }
}

void CrtWidget::communicationStatus(const QString &status, bool isOK)
{
    Q_ASSERT(m_alarmObj);
    if(status==tr("主电"))
    {
        if(isOK)
        {
            if(Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("主电故障")))
            {
                Controller::instance()->getSpeechObj()->removeAlarmText(tr("主电故障"));
            }

            QMetaObject::invokeMethod(m_alarmObj,"setMainConnunicationColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setMainConnunicationColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));//主电故障
            if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("主电故障")))
            {
                Controller::instance()->getSpeechObj()->insertAlarmText(tr("主电故障"));
            }

        }

    }
    else if(status==tr("备电"))
    {
        if(isOK)
        {
            if(Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("备电故障")))
            {
                Controller::instance()->getSpeechObj()->removeAlarmText(tr("备电故障"));
            }

            QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));//备电故障
            if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("备电故障")))
            {
                Controller::instance()->getSpeechObj()->insertAlarmText(tr("备电故障"));
            }
        }
    }
}

void CrtWidget::initWidget()
{
    Controller::instance()->setCrtWidget(this);
    qmlRegisterSingletonType<Controller>("userManager", 1, 0, "UserManager",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return Controller::instance()->getUserManager();
    });

    qmlRegisterSingletonType<Controller>("controller", 1, 0, "Controller",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return Controller::instance();
    });

    qmlRegisterSingletonType<Controller>("crtWidget", 1, 0, "Crt",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return Controller::instance()->getCrtWidget();
    });

    qmlRegisterSingletonType<Controller>("operatorInfo", 1, 0, "OperatorInfo",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return Controller::instance()->getOperatorInfo();
    });

    qmlRegisterSingletonType<Controller>("transportInfo", 1, 0, "TransportInfo",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return Controller::instance()->getTransportInfo();
    });

    qmlRegisterSingletonType<Controller>("serialConfigurationManager", 1, 0, "SerialPortInfo",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getSerialConfigurationManager();
    });

    qmlRegisterSingletonType<Controller>("indicatorConfigurationManager", 1, 0, "IndicatorConfiguration",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getIndicatorConfigurationManager();
    });

    qmlRegisterSingletonType<Controller>("ftpConfigurationManager", 1, 0, "FtpInfo",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getFtpConfigurationManager();
    });


    qmlRegisterSingletonType<Controller>("tcpConfigurationManager", 1, 0, "TcpInfo",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getTcpConfigurationManager();
    });


    qmlRegisterSingletonType<Controller>("sysArchitePlanView", 1, 0, "SysArchitePlanView",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getSysArchitePlanView();
    });


    qmlRegisterSingletonType<Controller>("serialLink", 1, 0, "SerialLink",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getCommObj();
    });

    qmlRegisterSingletonType<Controller>("indicatorLightCom", 1, 0, "IndicatorLightCom",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getIndicatorObj();
    });

    qmlRegisterSingletonType<Controller>("tcpLink", 1, 0, "TcpLink",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getTcpObj();
    });

    qmlRegisterSingletonType<Controller>("speechObj", 1, 0, "SpeechObj",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getSpeechObj();
    });

    m_serialDataProtocol = new SerialDataProtocol;
    m_monitoringProtocol = new MonitoringProtocol;
    m_indicatorProtocol = new IndicatorDataProtocol;
    m_ftpManager = new FtpManager();
    qmlRegisterType<QmlTableModel>("qmlTableModel",1,0,"QmlTableModel");
    qmlRegisterType<QmlForJson>("qmlForJson",1,0,"QmlForJson");
    qmlRegisterType<ItemIconInfoToJson>("itemIconInfoToJson",1,0,"ItemIconInfoToJson");
    QVBoxLayout *globalVLayout = new QVBoxLayout;
    QQuickView *toolBarQuickView = new QQuickView;
    toolBarQuickView->setSource(QUrl("qrc:/qml/toolBar/ToolBarWindow.qml"));
    toolBarQuickView->rootContext()->setContextProperty("CrtWidget",this);
    m_toolBarContainer = QWidget::createWindowContainer(toolBarQuickView, this);
    m_toolBarContainer->setMinimumHeight(60);
    m_toolBarContainer->setMaximumHeight(60);
    m_toolBarContainer->setMinimumWidth(100);
    m_toolBarContainer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    globalVLayout->addWidget(m_toolBarContainer);

    m_architePlanView = new ArchitePlanView(this);
    m_architePlanView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    Controller::instance()->setArchitePlanView(m_architePlanView);
    qmlRegisterSingletonType<Controller>("architePlanView", 1, 0, "ArchitePlanView",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getArchitePlanView();
    });

    m_infoTableView = new InfoTableView(this);
    m_infoTableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_sqliteManager = SqlManager::fromDriver("QSQLITE");
    m_alarmQuickView = new QQuickView;
    m_alarmQuickView->setSource(QUrl("qrc:/qml/alarmItem/AlarmItem.qml"));
    m_alarmQuickView->rootContext()->setContextProperty("CrtWidget",this);
    m_alarmQuickView->rootContext()->setContextProperty("ArchitePlanView",m_architePlanView);
    m_alarmObj = m_alarmQuickView->rootObject();

    m_alarmContainer = QWidget::createWindowContainer(m_alarmQuickView, this);
    m_alarmContainer->setMinimumHeight(100);
    m_alarmContainer->setMinimumWidth(150);
    m_alarmContainer->setMaximumWidth(150);
    m_alarmContainer->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);

    m_loginQuickView = new QQuickView;
    m_loginQuickView->setSource(QUrl("qrc:/qml/logInWindow/LoginWindow.qml"));
    m_loginQuickView->setGeometry(500,50,m_loginQuickView->width(),m_loginQuickView->height());
    m_loginQuickView->rootContext()->setContextProperty("CrtWidget",this);

    m_settingView = new QQuickView;
    m_settingView->setSource(QUrl("qrc:/qml/infoSetting/SettingWindow.qml"));

    m_infoQueryView = new QQuickView;
    m_infoQueryView->setSource(QUrl("qrc:/qml/infoSetting/InfoQuery.qml"));
    QHBoxLayout *globalHLayout = new QHBoxLayout;
    globalHLayout->addWidget(m_alarmContainer);
    QSplitter *splitter = new QSplitter(Qt::Vertical,this);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(m_architePlanView);
    splitter->addWidget(m_infoTableView);
    splitter->setStretchFactor(splitter->indexOf(m_architePlanView), 80);
    splitter->setStretchFactor(splitter->indexOf(m_infoTableView), 20);
    globalHLayout->addWidget(splitter);
    globalVLayout->addLayout(globalHLayout);
    globalVLayout->setSpacing(0);
    globalVLayout->setContentsMargins(QMargins(0,0,0,0));
    setLayout(globalVLayout);

}

void CrtWidget::alarmDataOnTable()
{

    QStringList alarmInfoList;
    alarmInfoList << "分机号"<<"回路号"<<"地址号"<<"网络号"<<"设备编码"
                  << "设备"<<"报警类型"<<"报警状态"<<"报警时间"<<"系统"
                  << "建筑名称"<<"楼层"<<"位置";
    if(m_architePlanView->currentAlarmType()=="全部")
    {
        m_infoTableView->tableModel()->sqlCommit(QString("select %1 from AlarmInfo where 报警状态 != '正常'").arg(alarmInfoList.join(",")));
    }
    else
    {
        m_infoTableView->tableModel()->sqlCommit(QString("select %1 from AlarmInfo where 报警状态 ='%2'").arg(alarmInfoList.join(",")).arg(m_architePlanView->currentAlarmType()));
    }
}

bool CrtWidget::setSysTime(const QDateTime &dateTime)
{

    bool isSuccess = false;

#ifdef Q_OS_WIN
    SYSTEMTIME system_time;
    GetLocalTime(&system_time);
    system_time.wYear = dateTime.date().year();
    system_time.wMonth = dateTime.date().month();
    system_time.wDay = dateTime.date().day();
    system_time.wHour = dateTime.time().hour();
    system_time.wMinute = dateTime.time().minute();
    system_time.wSecond = dateTime.time().second();


    if(SetLocalTime(&system_time)==0)
    {
        isSuccess= false;
    }
    else
    {
        isSuccess = true;
    }
#elif Q_OS_LINUX
    QDateTime curDateTime = QDateTime::currentDateTime();
    curDateTime.setDate(dateTime.date());
    curDateTime.setTime(dateTime.time());
    time_t tt= (time_t)curDateTime.toTime_t();
    if(stime(&tt)==0)
    {
        isSuccess = true;
    }
    else
    {
        isSuccess = false;
    }
#endif
    return isSuccess;
}

void CrtWidget::sendFireInfo(quint8 extNum, quint8 loopNum, quint8 addrNum,const QString &dateTimeStr)
{
    m_monitoringPackageNum++;
    QList<QByteArray> valueList;
    valueList.push_back(QString("%1").arg(44,4,10,QChar('0')).toLocal8Bit());
    valueList.push_back(QString("%1").arg(m_monitoringPackageNum,4,10,QChar('0')).toLocal8Bit());
    valueList.push_back(QByteArray("000"));
    QString extNumStr=QString("%1").arg(extNum,4,10,QChar('0'));
    valueList.push_back(extNumStr.toLocal8Bit());
    valueList.push_back(QString("%1").arg(loopNum,3,10,QChar('0')).toLocal8Bit());
    valueList.push_back(QString("%1").arg(addrNum,4,10,QChar('0')).toLocal8Bit());

    valueList.push_back(dateTimeStr.toLocal8Bit());
    Controller::instance()->getTcpObj()->writeData(m_monitoringProtocol->dataPackage(valueList));
}

void CrtWidget::serialDataProcessing(const QByteArray &arrayValue)
{
    QHash<quint8,QString>alarmTypeHash,eliminateAlarmHash,commuStatusHash;
    alarmTypeHash[0x01] = tr("火警");
    alarmTypeHash[0x03] = tr("故障");
    alarmTypeHash[0x05] = tr("联动");
    alarmTypeHash[0x0a] = tr("监管");
    alarmTypeHash[0x0b] = tr("屏蔽");
    alarmTypeHash[0x0d] = tr("反馈");
    commuStatusHash[0x12] = tr("主电");
    commuStatusHash[0x13] = tr("备电");
    commuStatusHash[0x23] = tr("消音");
    commuStatusHash[0x20] = tr("复位");
    commuStatusHash[0x31] = tr("手自动设置");
    commuStatusHash[0x32] = tr("手自动确认");
    eliminateAlarmHash[0x02] = tr("反馈消除");
    eliminateAlarmHash[0x04] = tr("故障恢复");
    eliminateAlarmHash[0x06] = tr("停止");
    eliminateAlarmHash[0x0c] = tr("屏蔽解除");
    QString networkNum="0";
    QString timeStr;
    QString extNum;
    QList<QByteArray>dataArrayList=  m_serialDataProtocol->frameData(arrayValue);
    foreach (QByteArray array, dataArrayList)
    {
        quint8 eventNum =  m_serialDataProtocol->dataByte(array,0);//事件
        quint8 loopNum =  m_serialDataProtocol->dataByte(array,1);//回路
        quint8 addrNum =  m_serialDataProtocol->dataByte(array,2);//地址
        quint8 type = m_serialDataProtocol->dataByte(array,3);
        quint8 minuteValue = m_serialDataProtocol->dataByte(array,8);
        quint8 year= m_serialDataProtocol->dataByte(array,4);
        quint8 month= m_serialDataProtocol->dataByte(array,5);
        quint8 date = m_serialDataProtocol->dataByte(array,6);
        quint8 hour = m_serialDataProtocol->dataByte(array,7);
        quint8 minute= minuteValue&0x7f;
        quint8 second= m_serialDataProtocol->dataByte(array,9);

        //报警主机
        if((minuteValue&0x80)==0)
        {
            timeStr= QString("%1/%2/%3 %4:%5:%6").arg((int)year+2000).arg((ushort)month,2,10,QChar('0'))
                    .arg((ushort)date,2,10,QChar('0')).arg((ushort)hour,2,10,QChar('0')).arg((ushort)minute,2,10,QChar('0'))
                    .arg((ushort)second,2,10,QChar('0'));
        }
        else
        {
            timeStr= QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
            networkNum = QString::number(minute);
        }



        if(commuStatusHash.keys().contains(eventNum))
        {
            if(commuStatusHash[eventNum]==tr("主电")||commuStatusHash[eventNum]==tr("备电"))
            {
                if(loopNum==0)
                {
                    communicationStatus(commuStatusHash[eventNum],true);
                }
                else if(loopNum==1)
                {
                    communicationStatus(commuStatusHash[eventNum],false);
                }
            }
            else if(commuStatusHash[eventNum]==tr("复位"))
            {
                if(type==0x00)
                {
                    Controller::instance()->getOperatorInfo()->insertEvent(tr("本机复位"));
                }
                else if(type==0x11)
                {
                    Controller::instance()->getOperatorInfo()->insertEvent(tr("网络复位"));
                }
            }
            else if(commuStatusHash[eventNum]==tr("消音"))
            {
                Controller::instance()->getSpeechObj()->stopSpeech();
                Controller::instance()->getOperatorInfo()->insertEvent(tr("消音"));
            }
            else
            {
                switch (type) {
                case 0:
                    Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash[eventNum],tr("默认"));
                    break;
                case 1:
                    Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash[eventNum],tr("手动"));
                    break;
                case 2:
                    Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash[eventNum],tr("自动"));
                    break;
                default:
                    break;
                }
            }
        }

        if(alarmTypeHash.keys().contains(eventNum))
        {

            if(alarmTypeHash[eventNum]==tr("监管"))
            {
                if((type&0x03)==0)
                {
                    extNum = QString("%1").arg(type>>2&0x3f);
                }
                else if((type&0x03)==1)//漏电
                {


                }
                else if((type&0x03)==2)//温度
                {

                }
            }
            else if(alarmTypeHash[eventNum]==tr("屏蔽")||alarmTypeHash[eventNum]==tr("反馈"))
            {
                extNum = QString::number(type);
            }
            else
            {
                extNum = QString("%1").arg((type>>2)&0x3f);
                if(alarmTypeHash[eventNum]==tr("火警"))
                {
                    QString dateTimeStr= QString("%1%2%3%4%5%6").arg((int)year+2000).arg((ushort)month,2,10,QChar('0'))
                           .arg((ushort)date,2,10,QChar('0')).arg((ushort)hour,2,10,QChar('0')).arg((ushort)minute,2,10,QChar('0'))
                           .arg((ushort)second,2,10,QChar('0'));
                    sendFireInfo(extNum.toShort(),loopNum,addrNum,dateTimeStr);
                }
            }

            m_architePlanView->createAlarm(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,alarmTypeHash[eventNum],false,timeStr);
        }

        if(eliminateAlarmHash.keys().contains(eventNum))
        {
            if(eliminateAlarmHash[eventNum]==tr("屏蔽解除"))
            {
                extNum = QString::number(type);
            }
            else
            {
                extNum = QString("%1").arg((type>>2)&0x3f);
            }
            m_architePlanView->eliminateAlarm(extNum,QString::number(loopNum),QString::number(addrNum),networkNum);
        }

        if(eventNum==0xaa)
        {
            bool isSuccess = setSysTime(QDateTime::fromString(timeStr,"yyyy/MM/dd hh:mm:ss"));
            if(isSuccess)
            {
                Controller::instance()->getOperatorInfo()->insertEvent(tr("校时"),tr("成功"));
            }
            else
            {
                Controller::instance()->getOperatorInfo()->insertEvent(tr("校时"),tr("失败"));
            }

        }
        else if(eventNum==0xbb)
        {
            GraphicsItem*item =m_architePlanView->itemFormInfo(extNum,QString::number(loopNum),QString::number(addrNum),networkNum);
            switch (type)
            {
            case 1:
                if(item!=nullptr)
                {
                  item->setAnlogValue(tr("剩余电流"),year*256+date);
                }
                break;
            case 2:

                if(item!=nullptr)
                {
                  item->setAnlogValue(tr("温度"),year*256+date);
                }
                break;
            default:
                break;
            }


        }

    }
}

void CrtWidget::tcpDataProcessing(const QByteArray &arrayValue)
{
    QHash<int,QString>typeInfoHash;
    typeInfoHash[0]=tr("消防控制室的管理机构");
    typeInfoHash[1]=tr("系统竣工图纸");
    typeInfoHash[2]=tr("各分系统控制逻辑关系说明");
    typeInfoHash[3]=tr("设备使用说明书");
    typeInfoHash[4]=tr("系统操作规程");
    typeInfoHash[5]=tr("应急预案");
    typeInfoHash[6]=tr("值班制度");
    typeInfoHash[7]=tr("维护保养制度");
    typeInfoHash[8]=tr("维护保养记录");
    QList<QByteArray>dataArrayList=  m_monitoringProtocol->frameData(arrayValue);
    foreach (QByteArray dataArray, dataArrayList)
    {
        int dataSize = dataArray.size();
        QByteArray indexArray=m_monitoringProtocol->dataBytes(dataArray,4,dataSize-1);
        int indexNum = indexArray.toInt();
        if(dataSize==8)
        {

            if(indexNum==m_monitoringPackageNum)
            {
                //火警信息接收成功
            }

        }
        else if(dataSize>8)
        {
            QList<QByteArray> sendArrayList;
            sendArrayList.push_back(QString("%1").arg(16,4,10,QChar('0')).toLocal8Bit());
            sendArrayList.push_back(indexArray);
            Controller::instance()->getTcpObj()->writeData(m_monitoringProtocol->dataPackage(sendArrayList));
            QByteArray typeArray=   dataArray.right(3);

            QString infoPath=  Controller::instance()->getTransportInfo()->transportInfo(typeInfoHash[typeArray.toInt()]);
            if(!infoPath.isEmpty())
            {
                m_ftpManager->uploadFile(infoPath);
            }

        }
    }
}

void CrtWidget::openHelpFile()
{
    QUrl url =   QUrl(QCoreApplication::applicationDirPath()+"/help/help.html");
    if(!url.isEmpty())
    {
        QDesktopServices::openUrl(url);
    }
}

