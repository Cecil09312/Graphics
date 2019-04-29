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
#include <time.h>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#ifdef Q_OS_LINUX
#define _SVID_SOURCE
#endif


CrtWidget::CrtWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_monitoringPackageNum(0),
    m_heartbeatIndex(0)
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
        if(!tableNameList.contains("AnalogInfo"))
        {
            m_sqliteManager->executeQuery("create table AnalogInfo(分机号 text, 回路号 text,地址号 text,网络号 text,当前通道 int, 模拟量类型 text ,结果 text)");
        }
    }


    m_infoTableView->hide();
    m_infoTableView->tableModel()->setDbDriver("QSQLITE");
    m_infoTableView->tableModel()->setDbName(dbName);
    m_infoTableView->tableModel()->setDbPort(888);
    m_infoTableView->tableModel()->setDbOpen(true);
    QStringList alarmInfoList,valueList;
    alarmInfoList << "分机号"<<"回路号"<<"地址号"<<"网络号"<<"设备编码" << "设备" <<"报警类型"<<"报警状态"<<"报警时间"
                  <<"报警恢复时间"<<"系统"<< "建筑名称"<<"楼层"<<"位置"<< "制造商" << "有效期" <<"操作员";

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
            QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,false));
            QMetaObject::invokeMethod(m_alarmObj,"setTransformColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
            arrayList.push_back(QString::number(0x04).toLocal8Bit());
            arrayList.push_back(QString::number(0x00).toLocal8Bit());
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,false));
            QMetaObject::invokeMethod(m_alarmObj,"setTransformColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
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
        Controller::instance()->getSpeechObj()->insertAlarmText(tr("传输故障"));
        Controller::instance()->getIndicatorObj()->writeData(m_indicatorProtocol->dataPackage(arrayList));
        QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"setTransformColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
    });


    connect(m_ftpManager,&FtpManager::uploadProgress,this,[=](qint64 bytesSent, qint64 bytesTotal)
    {
        QList<QByteArray> arrayList;
        static int num=0;
        if(bytesSent>=bytesTotal)
        {
            num = 0;
        }
        if(num>0 && num%20==0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,true));
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

    connect(m_architePlanView,&ArchitePlanView::alarmStateUpdate,this,[=]
            (const QString &extNum, const QString &loopNum,
            const QString &addressNum, const QString &networkNum,const QString &curAlarmState)
    {
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警状态 ='%1' where 分机号= '%2' and 回路号= '%3' and 地址号= '%4' and 网络号= '%5'").arg(curAlarmState).arg(extNum).arg(loopNum).arg(addressNum).arg(networkNum));
        alarmDataOnTable();
    });


    connect(m_controlCenterHeartbeatTimer,&QTimer::timeout,this,[=]()
    {

        static int index =0;
        if(m_heartbeatIndex>0 && m_heartbeatIndex==index)
        {
            QList<QByteArray>dataArrayList;
            dataArrayList.push_back(QString("%1").arg(19,4,10,QChar('0')).toLocal8Bit());
            dataArrayList.push_back(QString("%1").arg(index,4,10,QChar('0')).toLocal8Bit());
            dataArrayList.push_back(QString("%1").arg(999).toLocal8Bit());
            QByteArray sendArray=  m_monitoringProtocol->dataPackage(dataArrayList);
            Controller::instance()->getTcpObj()->writeData(sendArray);
            index++;
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("中心通信故障")))
            {
                Controller::instance()->getSpeechObj()->insertAlarmText(tr("中心通信故障"));
            }
            m_controlCenterHeartbeatTimer->stop();
            Controller::instance()->getTcpObj()->connectLink();
        }
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
            QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//主机通信
            Controller::instance()->getSpeechObj()->removeAlarmText(tr("主机通信故障"));

        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("主机通信故障")))
            {
                Controller::instance()->getSpeechObj()->insertAlarmText(tr("主机通信故障"));
            }
        }
    });

    connect(Controller::instance()->getTcpObj(),&TcpLink::isConnected,this,[=](bool connected)
    {
        if(connected)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//中心通信
            Controller::instance()->getSpeechObj()->removeAlarmText(tr("中心通信故障"));

            if(!m_controlCenterHeartbeatTimer->isActive())
            {
                m_controlCenterHeartbeatTimer->start(5000);
            }
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("中心通信故障")))
            {
                Controller::instance()->getSpeechObj()->insertAlarmText(tr("中心通信故障"));
            }
            m_controlCenterHeartbeatTimer->stop();
        }

    });


}

CrtWidget::~CrtWidget()
{

    m_controlCenterHeartbeatTimer->stop();
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

    // closeSys();

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
    Controller::instance()->getTransportInfo()->saveTransportInfoToJson();
    Controller::instance()->getOperatorInfo()->insertEvent(tr("系统关机"));

    m_architePlanView->saveInfo();
    m_loginQuickView->close();
    m_settingView->close();
    Controller::instance()->getMySqlManager()->close();
    Controller::instance()->getMySqlManager()->deleteLater();
    event->accept();
}


void CrtWidget::widgetExit()
{
    if(!m_architePlanView->havingAlarms())
    {
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
    else if(type==tr("启动"))
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
    if(status.endsWith(tr("主电")))
    {
        if(isOK)
        {
            if(Controller::instance()->getSpeechObj()->alarmTextList().contains(status+tr("故障")))
            {
                Controller::instance()->getSpeechObj()->removeAlarmText(status+tr("故障"));
            }

            QMetaObject::invokeMethod(m_alarmObj,"setMainConnunicationColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setMainConnunicationColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));//主电故障
            if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(status+tr("故障")))
            {
                Controller::instance()->getSpeechObj()->insertAlarmText(status+tr("故障"));
            }

        }

    }
    else if(status.endsWith(tr("备电")))
    {
        if(isOK)
        {
            if(Controller::instance()->getSpeechObj()->alarmTextList().contains(status+tr("故障")))
            {
                Controller::instance()->getSpeechObj()->removeAlarmText(status+tr("故障"));
            }

            QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));//备电故障
            if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(status+tr("故障")))
            {
                Controller::instance()->getSpeechObj()->insertAlarmText(status+tr("故障"));
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




    qmlRegisterSingletonType<Controller>("mySqlManager", 1, 0, "MySqlManager",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getMySqlManager();
    });

    setMySqlInfo();


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

    m_controlCenterHeartbeatTimer = new QTimer(this);

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
    m_loginQuickView->setTitle(tr("用户登陆界面"));
    m_settingView = new QQuickView;
    m_settingView->setSource(QUrl("qrc:/qml/infoSetting/SettingWindow.qml"));
    m_settingView->setGeometry(300,50,m_settingView->width(),m_settingView->height());
    m_settingView->setTitle(tr("信息设置界面"));
    m_infoQueryView = new QQuickView;
    m_infoQueryView->setSource(QUrl("qrc:/qml/infoSetting/InfoQuery.qml"));
    m_infoQueryView->setGeometry(300,50,m_infoQueryView->width(),m_infoQueryView->height());
    m_infoQueryView->setTitle(tr("信息查询界面"));
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
    if(m_architePlanView->currentAlarmType()==tr("全部"))
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
#endif
#ifdef Q_OS_LINUX
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

void CrtWidget::setMySqlInfo()
{
    SqlManager*sqlManager=   Controller::instance()->getMySqlManager();
    Q_ASSERT(sqlManager);
    QString mySqlInfoPath=  QCoreApplication::applicationDirPath()+"/mySqlInfo.json";
    QmlForJson qmlForJson;
    QHash<QString,QVariant> mySqlInfoHash=  qmlForJson.readFile(mySqlInfoPath).toHash();
    QString hostName="127.0.0.1", userName = "root",password="song",databaseName= "monitoring_center_db";
    int port =3306;
    if(!mySqlInfoHash.isEmpty())
    {
        hostName =  mySqlInfoHash["hostName"].toString();
        userName =   mySqlInfoHash["userName"] .toString();
        password=   mySqlInfoHash["password"].toString();
        databaseName =  mySqlInfoHash["databaseName"].toString();
        port = mySqlInfoHash["port"].toInt();
    }

    sqlManager->setDataBase("QMYSQL","mySqlConnect",hostName,userName,password,databaseName,port);

    bool mySqlOpen = sqlManager->open();
    if(!mySqlOpen)
    {
        if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("MySql数据库连接失败")))
        {
            Controller::instance()->getSpeechObj()->insertAlarmText(tr("MySql数据库连接失败"));
        }
    }
    else
    {

        Controller::instance()->getSpeechObj()->removeAlarmText(tr("MySql数据库连接失败"));
        if(!sqlManager->tableIsExist("sys_status"))
        {
            sqlManager->executeQuery("create table sys_status ( sys_name text,main_power text,prepare_power text,hand_auto_state text,run_state text);");
        }

        if(!sqlManager->tableIsExist("alarm_info"))
        {
            sqlManager->executeQuery("create table alarm_info ( sys_name text,device_num text,alarm_type text,current_state text,alarm_time text);");
        }

        if(!sqlManager->tableIsExist("fault_state"))
        {
            sqlManager->executeQuery("create table fault_state ( sys_name text,fault_type text,fault_state text,run_state text);");
        }

      //   Controller::instance()->getMySqlManager()->executeQuery(QString("insert into alarm_info values ('%1','%2','%3','%4','%5')").arg("自动报警系统").arg(tr("故障")).arg("常开门关闭").arg("正常").arg(tr("正常运行")));
    }
}

void CrtWidget::serialDataProcessing(const QByteArray &arrayValue)
{
    QHash<quint8,QString>alarmTypeHash,commuStatusHash,faultStateHash;
    alarmTypeHash[0x01] = tr("火警");
    alarmTypeHash[0x03] = tr("故障");
    alarmTypeHash[0x05] = tr("启动");
    alarmTypeHash[0x0a] = tr("监管");
    alarmTypeHash[0x0b] = tr("屏蔽");
    alarmTypeHash[0x0d] = tr("反馈");
    commuStatusHash[0x12] = tr("主电");
    commuStatusHash[0x13] = tr("备电");
    commuStatusHash[0x31] = tr("手自动设置");
    commuStatusHash[0x32] = tr("手自动确认");
    faultStateHash[0x01] = tr("过压");
    faultStateHash[0x03] = tr("欠压");
    faultStateHash[0x05] = tr("缺相");
    faultStateHash[0x07] = tr("错相");

    faultStateHash[0x09] = tr("过流");
    faultStateHash[11] = tr("霍尔故障");
    faultStateHash[13] = tr("断电");
    faultStateHash[15] = tr("模块故障");
    faultStateHash[17] = tr("回路故障");

    QString networkNum="0";
    int packageNum=0;
    QString timeStr;
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

        QString extNum = QString("%1").arg((type>>2)&0x3f);


        if(packageNum!=m_serialDataProtocol->dataPackageNum(array))
        {
            packageNum = m_serialDataProtocol->dataPackageNum(array);
            QList<QByteArray> sendDataArrayList;
            sendDataArrayList.push_back(QString::number(packageNum).toLocal8Bit());
            sendDataArrayList.push_back(QString::number(0x03).toLocal8Bit());
            Controller::instance()->getCommObj()->writeData(m_serialDataProtocol->dataPackage(sendDataArrayList));
        }

        //报警主机
        if((minuteValue&0x80)==0)
        {
            if(second<=60)
            {
                int bcd_year = m_serialDataProtocol->dataBytes(array,4,4).toHex().toInt();
                int bcd_month = m_serialDataProtocol->dataBytes(array,5,5).toHex().toInt();
                int bcd_date = m_serialDataProtocol->dataBytes(array,6,6).toHex().toInt();
                int bcd_hour = m_serialDataProtocol->dataBytes(array,7,7).toHex().toInt();
                int bcd_minute = m_serialDataProtocol->dataBytes(array,8,8).toHex().toInt();
                int bcd_second = m_serialDataProtocol->dataBytes(array,9,9).toHex().toInt();
                timeStr= QString("%1/%2/%3 %4:%5:%6").arg(bcd_year+2000).arg(bcd_month,2,10,QChar('0'))
                        .arg(bcd_date,2,10,QChar('0')).arg(bcd_hour,2,10,QChar('0')).arg(bcd_minute,2,10,QChar('0'))
                        .arg(bcd_second,2,10,QChar('0'));
            }
            else
            {
                timeStr = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
            }
        }
        else
        {
            timeStr= QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
            networkNum = QString::number(minute);
        }

        GraphicsItem*item =m_architePlanView->itemFormInfo(extNum,QString::number(loopNum),QString::number(addrNum),networkNum);
        QString sysName = m_architePlanView->deviceSysName(extNum);
        QStringList faultInfoList =  Controller::instance()->getMySqlManager()->executeQuery("select sys_name,fault_type from fault_state");
        QStringList sysStateList =  Controller::instance()->getMySqlManager()->executeQuery("select sys_name,fault_type from sys_status");
        switch (eventNum)
        {
        case 0x01:
        case 0x03:
        case 0x05:
        case 0x0b:
        case 0x0d:
        {
            m_architePlanView->createAlarm(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,alarmTypeHash[eventNum],false,timeStr);
            if(item!=nullptr)
            {
               Controller::instance()->getMySqlManager()->executeQuery(QString("insert into alarm_info values ('%1','%2','%3','%4','%5')").arg(item->sysOfDevice()).arg(item->deviceNum()).arg(item->alarmType()).arg(item->currentState()).arg(timeStr));
            }

            QString currentState;
            if(eventNum==0x03)
            {
                switch ((type&0x03))
                {
                case 1:
                    currentState = tr("常开门故障");
                    break;
                case 2:
                    currentState = tr("常闭门打开");
                    break;
                default:
                    currentState = tr("故障");
                    break;
                }
                m_architePlanView->updateAlarmState(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,currentState);


                if(!faultInfoList.contains(sysName)&&!faultInfoList.contains(tr("故障")))
                {
                    Controller::instance()->getMySqlManager()->executeQuery(QString("insert into fault_state values ('%1','%2','%3','%4')").arg(sysName).arg(tr("故障")).arg(currentState).arg(tr("正常运行")));
                }
                else
                {
                    Controller::instance()->getMySqlManager()->executeQuery(QString("update fault_state set fault_state='%1' where sys_name ='%2' and fault_type = '%3' )").arg(currentState).arg(sysName).arg(tr("故障")));
                }

            }
            else if(eventNum==0x0d)
            {
                if((type&0x03)==0x01)
                {
                    m_architePlanView->updateAlarmState(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,tr("常开门关闭"));
                    if(!faultInfoList.contains(sysName)&&!faultInfoList.contains(tr("反馈")))
                    {
                        Controller::instance()->getMySqlManager()->executeQuery(QString("insert into fault_state values ('%1','%2','%3','%4')").arg(sysName).arg(tr("常开门关闭")).arg(currentState).arg(tr("正常运行")));
                    }
                    else
                    {
                        Controller::instance()->getMySqlManager()->executeQuery(QString("update fault_state set fault_state='%1' where sys_name ='%2' and fault_type = '%3' )").arg(currentState).arg(sysName).arg(tr("反馈")));
                    }
                }

            }
            else if(eventNum==0x01)
            {
                QString dateTimeStr= QString("%1%2%3%4%5%6").arg((int)year+2000).arg((ushort)month,2,10,QChar('0'))
                        .arg((ushort)date,2,10,QChar('0')).arg((ushort)hour,2,10,QChar('0')).arg((ushort)minute,2,10,QChar('0'))
                        .arg((ushort)second,2,10,QChar('0'));
                sendFireInfo(extNum.toShort(),loopNum,addrNum,dateTimeStr);
            }


            if(second==0xa0)
            {
                if(faultStateHash.keys().contains(month))
                {
                    if(year==0xa0)
                    {
                        m_architePlanView->updateAlarmState(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,tr("A0模块")+ faultStateHash[month]);
                    }
                    else if(year==0xa1)
                    {
                        m_architePlanView->updateAlarmState(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,tr("A1通道")+ faultStateHash[month]);
                    }
                    else if(year==0xa2)
                    {
                        m_architePlanView->updateAlarmState(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,tr("A2通道")+ faultStateHash[month]);
                    }
                    else if(year==0xa3)
                    {
                        m_architePlanView->updateAlarmState(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,tr("A3通道")+ faultStateHash[month]);
                    }

                }

            }
        }
            break;
        case 0x0a:
        {
            switch((type&0x03))
            {
            case 0:
                m_architePlanView->createAlarm(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,alarmTypeHash[eventNum],false,timeStr);
                break;
            case 1:
                if(item!=nullptr)
                {
                    item->analogType() = tr("电流(A)");
                    item->setAnlogValue(0,year*256+second);
                    m_sqliteManager->executeQuery(QString("insert into AnalogInfo values('%1','%2','%3','%4','%5','%6','%7')").arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()).arg(0).arg(item->analogType()).arg(item->anlogValueHah().value(0).toString()));
                }
                break;
            case 2:
                if(item!=nullptr)
                {
                    item->analogType() = tr("温度(℃)");
                    item->setAnlogValue(0,year*256+second);
                    m_sqliteManager->executeQuery(QString("insert into AnalogInfo values('%1','%2','%3','%4','%5','%6','%7')").arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()).arg(0).arg(item->analogType()).arg(item->anlogValueHah().value(0).toString()));
                }
                break;
            default:
                break;

            }
        }
            break;
        case 0x02:
        case 0x04:
        case 0x06:
        case 0x0c:
        {
            m_architePlanView->eliminateAlarm(extNum,QString::number(loopNum),QString::number(addrNum),networkNum);
            if(item!=nullptr)
            {
               Controller::instance()->getMySqlManager()->executeQuery(QString("delete from alarm_info where sys_name = '%1' and device_num = '%2'").arg(item->sysOfDevice()).arg(item->deviceNum()));
            }
            if(eventNum==0x04)
            {
                QString currentState;
                switch ((type&0x03))
                {
                case 0:
                {
                    currentState = tr("故障恢复");
                }
                    break;
                case 1:
                {
                    currentState = tr("常开门故障恢复");
                }
                    break;

                case 2:
                {
                    currentState = tr("常开门关闭");
                }

                    break;
                default:
                    break;
                }
                Controller::instance()->getMySqlManager()->executeQuery(QString("update fault_state set fault_state='%1' where sys_name ='%2' and fault_type = '%3' )").arg(currentState).arg(sysName).arg(tr("故障")));
            }
            else if(eventNum==0x02)
            {
                if((type&0x03)==0x01)
                {
                    Controller::instance()->getMySqlManager()->executeQuery(QString("update fault_state set fault_state='%1' where sys_name ='%2' and fault_type = '%3' )").arg(tr("常开门打开")).arg(sysName).arg(tr("反馈")));
                }

            }
        }
            break;
        case 0x12:
        case 0x13:
        {
            QString powerState = "";
            if(loopNum==0)
            {
                communicationStatus(commuStatusHash[eventNum],true);
                powerState = tr("正常");
            }
            else if(loopNum==1)
            {
                communicationStatus(commuStatusHash[eventNum],false);
                powerState = tr("故障");
            }

            if(!sysStateList.contains(sysName))
            {
                if(eventNum== 0x12)
                {
                    Controller::instance()->getMySqlManager()->executeQuery(QString("insert into sys_status values ('%1','%2','%3','%4','%5')").arg(sysName).arg(powerState).arg("").arg("").arg(tr("正常运行")));
                }
                else if(eventNum==0x13)
                {
                    Controller::instance()->getMySqlManager()->executeQuery(QString("insert into sys_status values ('%1','%2','%3','%4','%5')").arg(sysName).arg("").arg(powerState).arg("").arg(tr("正常运行")));
                }

            }
            else
            {
                if(eventNum== 0x12)
                {
                    Controller::instance()->getMySqlManager()->executeQuery(QString("update sys_status set main_power='%1' where sys_name ='%2')").arg(powerState).arg(sysName));
                }
                else if(eventNum==0x13)
                {
                    Controller::instance()->getMySqlManager()->executeQuery(QString("update sys_status set prepare_power='%1' where sys_name ='%2')").arg(powerState).arg(sysName));
                }

            }
        }
            break;
        case 0x20:
        {
            if((loopNum|addrNum)==0x00)
            {
                Controller::instance()->getOperatorInfo()->insertEvent(tr("本机复位"));
            }
            else if((loopNum&addrNum)==0x01)
            {
                Controller::instance()->getOperatorInfo()->insertEvent(tr("网络复位"));
            }
        }
            break;
        case 0x23:
        {
            Controller::instance()->getSpeechObj()->stopSpeech();
            Controller::instance()->getOperatorInfo()->insertEvent(tr("消音"));
        }
            break;
        case 0x31:
        case 0x32:
        {
            QString handOrAutoState = "";
            switch (type)
            {
            case 0:
            {
                Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash[eventNum],tr("默认"));
                handOrAutoState = tr("默认");
            }
                break;
            case 1:
            {
                Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash[eventNum],tr("手动"));
                handOrAutoState = tr("手动");
            }
                break;
            case 2:
            {
                Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash[eventNum],tr("自动"));
                handOrAutoState = tr("自动");
            }
                break;
            default:
                break;
            }

            if(!sysStateList.contains(sysName))
            {
                if(eventNum==0x32)
                {
                    Controller::instance()->getMySqlManager()->executeQuery(QString("insert into sys_status values ('%1','%2','%3','%4','%5')").arg(sysName).arg("").arg("").arg(handOrAutoState).arg(tr("正常运行")));
                }

            }
            else
            {
                if(eventNum==0x32)
                {
                    Controller::instance()->getMySqlManager()->executeQuery(QString("update sys_status set hand_auto_state='%1' where sys_name ='%2')").arg(handOrAutoState).arg(sysName));
                }
            }
        }
            break;
        case 0x35:
        {
            GraphicsItem*currentItem  = m_architePlanView->itemFormInfo(QString::number(date),QString::number(loopNum),QString::number(addrNum),QString::number(month));
            switch (loopNum) {
            case 0x01:
            {
                if(currentItem!=nullptr)
                {
                    currentItem->currentState()= tr("火警");
                    currentItem->alarmType()= tr("光纤火警");
                    currentItem->deviceLocation() = QString::number(type*256+year)+tr("米");
                    m_architePlanView->createAlarm(currentItem);
                    Controller::instance()->getMySqlManager()->executeQuery(QString("insert into alarm_info values ('%1','%2','%3','%4','%5')").arg(currentItem->sysOfDevice()).arg(currentItem->deviceNum()).arg(currentItem->alarmType()).arg(currentItem->currentState()).arg(currentItem->getItemInfo().m_alarmTime));

                }
            }
                break;
            case 0x02:
            {
                if(currentItem!=nullptr)
                {
                    currentItem->currentState()= tr("故障");
                    currentItem->alarmType()= tr("光纤故障");
                    currentItem->deviceLocation() = QString::number(type*256+year)+tr("米");
                    m_architePlanView->createAlarm(currentItem);
                    Controller::instance()->getMySqlManager()->executeQuery(QString("insert into alarm_info values ('%1','%2','%3','%4','%5')").arg(currentItem->sysOfDevice()).arg(currentItem->deviceNum()).arg(currentItem->alarmType()).arg(currentItem->currentState()).arg(currentItem->getItemInfo().m_alarmTime));
                }
            }
                break;
            case 0x03:
            {
                m_architePlanView->eliminateAlarm(QString::number(date),QString::number(loopNum),QString::number(addrNum),QString::number(month));
                if(currentItem!=nullptr)
                {
                    Controller::instance()->getMySqlManager()->executeQuery(QString("delete from alarm_info where sys_name='%1' and device_num='%2'").arg(currentItem->sysOfDevice()).arg(currentItem->deviceNum()));
                }

            }
                break;
            case 0x04:
            {
                if(addrNum==0)
                {
                    communicationStatus(tr("光纤主电"),true);
                }
                else if(addrNum==1)
                {
                    communicationStatus(tr("光纤主电"),false);
                }
            }
                break;
            case 0x05:
            {
                if(addrNum==0)
                {
                    communicationStatus(tr("光纤备电"),true);
                }
                else if(addrNum==1)
                {
                    communicationStatus(tr("光纤备电"),false);
                }
            }
                break;
            default:
                break;
            }
        }
            break;
        case 0xdd://重传应答

            break;
        case 0xaa:
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
            break;
        case 0xbb://模拟量
        {

            int curChannel=0;
            if(second==0xa0)
            {
                curChannel = (year>>5)&0x07;
                QString name =QString("%1通道").arg(curChannel);
                switch(hour)
                {
                case 1:
                    name+=tr("A项");
                    break;
                case 2:
                    name+=tr("B项");
                    break;
                case 3:
                    name+=tr("C项");
                    break;
                default:
                    break;
                }
                switch(type&0x03)
                {
                case 0://交流电流
                {
                    if(item!=nullptr)
                    {
                        item->analogType() = tr("交流电流(mA)");
                        item->setAnlogValue(curChannel,date*256+month);
                    }
                }
                    break;
                case 1://直流电流
                {
                    // name += ;
                    if(item!=nullptr)
                    {
                        item->analogType() = tr("直流电流(mA)");
                        item->setAnlogValue(curChannel,(date*256+month)*10);
                    }
                }
                    break;
                case 2://交流电压
                {
                    // name += tr("交流电压(V)");
                    if(item!=nullptr)
                    {
                        item->analogType() = tr("交流电压(V)");
                        item->setAnlogValue(curChannel,date*256+month);
                    }
                }
                    break;
                case 3://直流电压
                {
                    //name += tr("直流电压(V)");
                    if(item!=nullptr)
                    {
                        item->analogType() = tr("直流电压(V)");
                        item->setAnlogValue(curChannel,(date*256+month)*0.1);
                    }
                }
                    break;
                default:
                    break;
                }


            }
            else
            {
                switch (type)
                {
                case 1:
                    if(item!=nullptr)
                    {
                        item->analogType() = tr("剩余电流(mA)");
                        item->setAnlogValue(0,year*256+date);
                    }
                    break;
                case 2:

                    if(item!=nullptr)
                    {
                        item->analogType() = tr("温度(℃)");
                        item->setAnlogValue(0,year*256+date);
                    }
                    break;
                default:
                    break;
                }

            }

            if(item!=nullptr)
            {
                m_sqliteManager->executeQuery(QString("insert into AnalogInfo values('%1','%2','%3','%4','%5','%6','%7')").arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()).arg(curChannel).arg(item->analogType()).arg(item->anlogValueHah().value(curChannel).toString()));
            }
        }
            break;
        default:
            break;
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
        QByteArray indexArray=m_monitoringProtocol->dataBytes(dataArray,4,7);
        int indexNum = indexArray.toInt();
        if(dataSize==8)
        {

            if(indexNum==m_monitoringPackageNum)
            {
                //火警信息接收成功
            }

        }
        else if(dataSize==14)
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
        else if(dataSize==11)
        {

            m_heartbeatIndex = indexNum;
            //            QList<QByteArray> sendArrayList;
            //            sendArrayList.push_back(QString("%1").arg(16,4,10,QChar('0')).toLocal8Bit());
            //            sendArrayList.push_back(indexArray);
            //            Controller::instance()->getTcpObj()->writeData(m_monitoringProtocol->dataPackage(sendArrayList));
            //            QByteArray typeArray=   dataArray.right(3);

            //            QString infoPath=  Controller::instance()->getTransportInfo()->transportInfo(typeInfoHash[typeArray.toInt()]);
            //            if(!infoPath.isEmpty())
            //            {
            //                m_ftpManager->uploadFile(infoPath);
            //            }

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

void CrtWidget::sendAnalogCommand(quint8 networkNum,quint8 extNum,quint8 loopNum,quint8 addrNum,quint8 channelNum,const QString& analogType)
{
    QHash<int,QString>analogTypeHash;
    analogTypeHash[0] = tr("无"), analogTypeHash[1] = tr("高度(m)");
    analogTypeHash[2] = tr("温度(℃)"),analogTypeHash[3] = tr("压力(Mpa)");
    analogTypeHash[4] = tr("压力(Kpa)"),analogTypeHash[5] = tr("气体浓度(%LEL)");
    analogTypeHash[6] = tr("气体浓度(PPM)"),analogTypeHash[7] = tr("气体浓度(%V/V)");
    analogTypeHash[8] = tr("气体浓度(KPPM)"),analogTypeHash[9] = tr("气体浓度(Mg/m^3)");
    analogTypeHash[10] = tr("时间(s)"),analogTypeHash[11] = tr("电压(V)");
    analogTypeHash[12] = tr("电流(A)"),analogTypeHash[13] = tr("流量(L/s)");
    analogTypeHash[14] = tr("风量(m^3/min)"), analogTypeHash[15] = tr("风速(m/s)");
    analogTypeHash[16] = tr("剩余电流(mA)"),analogTypeHash[17]=tr("烟参量");
    analogTypeHash[18] = tr("距离(m)"), analogTypeHash[24] = tr("交流电流(mA)");
    analogTypeHash[25] = tr("直流电流(mA)"),analogTypeHash[26] = tr("交流电压(V)");
    analogTypeHash[27] = tr("直流电压(V)");
    static quint8 num =0;
    num++;
    num = num%256;
    QList<QByteArray>dataArrayList;
    dataArrayList.push_back(QString::number(num).toLocal8Bit());
    dataArrayList.push_back(QString::number(0x08).toLocal8Bit());
    dataArrayList.push_back(QString::number(0xbc).toLocal8Bit());
    dataArrayList.push_back(QString::number(networkNum).toLocal8Bit());
    dataArrayList.push_back(QString::number(extNum).toLocal8Bit());
    dataArrayList.push_back(QString::number(loopNum).toLocal8Bit());
    dataArrayList.push_back(QString::number(addrNum).toLocal8Bit());
    quint8 curChannel= ((channelNum&0xe0)|(analogTypeHash.key(analogType)&0x1f));
    dataArrayList.push_back(QString::number(curChannel).toLocal8Bit());
    QByteArray array= m_serialDataProtocol->dataPackage(dataArrayList);
    Controller::instance()->getCommObj()->sendData(array);
}



void CrtWidget::closeSys()
{
    QProcess process;
#ifdef Q_OS_WIN

    process.start("shutdown -s -t 0");
#endif
#ifdef Q_OS_LINUX
    process.start("poweroff");
#endif
    process.waitForStarted();
    process.waitForFinished();
}

