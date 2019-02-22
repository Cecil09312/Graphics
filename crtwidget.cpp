#include "crtwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QQuickView>
#include <QQmlComponent>
#include <QQmlContext>
#include <QSplitter>
#include <QQuickItem>
#include "communication/SerialLink.h"
#include "control/usermanager.h"
#include "control/controller.h"
#include <QHostAddress>
#include "communication/configurationmanager.h"
#include "database/sqlitemanager.h"
#include "database/sqlmanager.h"


CrtWidget::CrtWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{

    initWidget();

    setWindowFlags(Qt::FramelessWindowHint|Qt::Window);
    QString dbName = QCoreApplication::applicationDirPath()+"/alarmInfo.db";
    m_sqliteManager->setDataBase("QSQLITE","alarmInfo","","","",dbName,888);
    m_sqliteManager->open();
    if(m_sqliteManager->isOpen())
    {
        QStringList tableNameList = m_sqliteManager->getTables(dbName);
        if(!tableNameList.contains("AlarmInfo"))
        {
            m_sqliteManager->executeQuery("create table AlarmInfo(分机号 text, 回路号 text,地址号 text,报警类型 text,设备产品编号 text ,设备设施型号 text,报警当前状态 text,报警时间 text,报警收到时间 text,报警恢复正常时间 text,设备所属系统 text,总保护区域名称 text,建筑设施名称 text,设施所在楼层 text,设施所在位置 text,值班人员 text)");
        }
    }

    m_infoTableView->tableModel()->setDbDriver("QSQLITE");
    m_infoTableView->tableModel()->setDbName(dbName);
    // m_infoTableView->tableModel()->setDbConnectionName("alarmInfo");
    m_infoTableView->tableModel()->setDbPort(888);
    m_infoTableView->tableModel()->setDbOpen(true);
    // m_infoTableView->tableModel()->sqlCommit("select *from AlarmInfo");
    QStringList alarmInfoList,valueList;
    alarmInfoList << "分机号"<<"回路号"<<"地址号"<<"报警类型"<<"设备产品编号"
                  << "设备设施型号"<<"报警当前状态"<<"报警时间"<<"报警收到时间"
                  << "报警恢复正常时间"<<"设备所属系统"<<"总保护区域名称"
                  << "建筑设施名称"<<"设施所在楼层"<<"设施所在位置"<<"值班人员";

    //    alarmInfoList << "分机号"<<"回路号"<<"地址号"<<"报警类型"<<"设备产品编号"
    //                  << "设备设施型号"<<"报警收到时间"<<"设备所属系统"<<"总保护区域名称"
    //                  << "建筑设施名称"<<"设施所在楼层"<<"设施所在位置"<<"值班人员";
    for(int i=0;i<alarmInfoList.size();i++)
    {
        QString str = QString("'%%1'").arg(i+1);
        valueList.push_back(str);
    }
    QString sqlInfo = QString("insert into AlarmInfo (%1) values (%2)").arg(alarmInfoList.join(",")).arg(valueList.join(","));
    connect(m_architePlanView,&ArchitePlanView::alarmHappend,this,&CrtWidget::alarmStatistics);
    connect(m_architePlanView,&ArchitePlanView::alarmItem,this,[=](GraphicsItem *item)
    {
        m_sqliteManager->executeQuery(sqlInfo.arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->alarmType()).arg(item->deviceNum())
                                      .arg(item->equipmentModel()).arg(item->getItemInfo().m_currentAlarmState).arg(item->getItemInfo().m_alarmTime)
                                      .arg(item->getItemInfo().m_alarmReceiveTime).arg(item->getItemInfo().m_alarmReplyTime).arg(item->sysOfDevice()).arg(item->protectedAreaName()).arg(item->buildingName())
                                      .arg(item->floorOfDevice()).arg(item->deviceLocation()).arg(item->operatorDuty()));
        alarmDataOnTable();
    });

    connect(m_infoTableView,&InfoTableView::tableValue,this,[=](QSqlRecord record)
    {
        QString deviceNum=  record.value("设备产品编号").toString();
        QList<QList<QGraphicsItem *> > globalValueList= DataStore::getTypeItemHash().values();
        GraphicsView *view = nullptr;
        foreach (QList<QGraphicsItem *>valueList, globalValueList)
        {
            foreach (QGraphicsItem *item, valueList)
            {
                GraphicsItem *currentItem = dynamic_cast<GraphicsItem *>(item);
                if(currentItem->deviceNum()==deviceNum)
                {
                    view = DataStore::itemDisplayView(currentItem);
                    break;
                }
            }
        }

        if(view!=nullptr)
        {
            m_architePlanView->autoFitView(view);
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
    connect(m_architePlanView,&ArchitePlanView::toFirstPage,this,[=](){
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()+1));

    });

    connect(m_architePlanView,&ArchitePlanView::toLastPage,this,[=](){
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()+1));

    });

    connect(m_architePlanView,&ArchitePlanView::normalPage,this,[=](){
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()+1));
    });

    connect(m_architePlanView,&ArchitePlanView::noPage,this,[=](){
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()+1));

    });

    connect(m_architePlanView,&ArchitePlanView::clearAlarmFromTable,this,[=]()
    {
        QString eliminateTime =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复正常时间 ='%1',报警当前状态 = '报警消除' where 报警当前状态 = '正在报警'").arg(eliminateTime));
        alarmDataOnTable();
    });

    connect(m_architePlanView,&ArchitePlanView::eliminateAlarmFromTable,this,[=](GraphicsItem *item)
    {
        QString eliminateTime =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复正常时间 ='%1',报警当前状态 = '报警消除' where 报警当前状态 = '正在报警' and 设备产品编号 = '%2'").arg(eliminateTime).arg(item->deviceNum()));
        alarmDataOnTable();
    });
}

CrtWidget::~CrtWidget()
{
    m_sqliteManager->close();
    delete m_alarmContainer;
    delete m_toolBarContainer;
    delete m_loginQuickView;
    m_settingViewEngine->deleteLater();
}

void CrtWidget::widgetExit()
{
    close();
}

void CrtWidget::loginWidgetShow()
{
    m_loginQuickView->show();
}

void CrtWidget::settingWindowShow()
{
    //m_settingQuickView->show();
    m_settingViewEngine->load(QUrl("qrc:/qml/infoSetting/SettingWindow.qml"));
    // m_settingViewEngine->rootContext()->setContextProperty("Controller",Controller::instance());
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
    if(totalPage>0)
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
    else
    {
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
    }
    QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,totalPage),Q_ARG(QVariant,currentPage+1));
}

void CrtWidget::alarmStatistics(const QString &type)
{
    Q_ASSERT(m_alarmObj);
    if(type==tr("火警"))
    {
        QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        //QMetaObject::invokeMethod(m_alarmObj,"startFireAnimation",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
    }
    else if(type==tr("联动"))
    {
        QMetaObject::invokeMethod(m_alarmObj,"setLinkageAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        //QMetaObject::invokeMethod(m_alarmObj,"startLinkageAnimation",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"setLinkageText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
    }
    else if(type==tr("监管"))
    {
        QMetaObject::invokeMethod(m_alarmObj,"setSuperviseAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        // QMetaObject::invokeMethod(m_alarmObj,"startSuperviseAnimation",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"setSuperviseText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
    }
    else if(type==tr("故障"))
    {
        QMetaObject::invokeMethod(m_alarmObj,"setfaultAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        // QMetaObject::invokeMethod(m_alarmObj,"startFaultAnimation",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"setFaultText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
    }
    else if(type==tr("反馈"))
    {
        QMetaObject::invokeMethod(m_alarmObj,"setFeedbackColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        // QMetaObject::invokeMethod(m_alarmObj,"startFeedbackAnimation",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"setFeedbackText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
    }
    else if(type==tr("屏蔽"))
    {
        QMetaObject::invokeMethod(m_alarmObj,"setShieldAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        // QMetaObject::invokeMethod(m_alarmObj,"startShieldAnimation",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"setShieldText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
    }
}

void CrtWidget::initWidget()
{

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


    qmlRegisterSingletonType<Controller>("serialConfigurationManager", 1, 0, "SerialPortInfo",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getSerialConfigurationManager();
    });



    qmlRegisterSingletonType<Controller>("sysArchitePlanView", 1, 0, "SysArchitePlanView",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getSysArchitePlanView();
    });

    //    qmlRegisterSingletonType<Controller>("speechObj", 1, 0, "SpeechObj",
    //                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
    //        Q_UNUSED(engine)
    //        Q_UNUSED(scriptEngine)
    //        return Controller::instance()->getSpeechObj();
    //    });


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
    m_sqliteManager = SqlManager::fromDriver("QSQLITER");
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

    m_settingViewEngine = new QQmlApplicationEngine();

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
    alarmInfoList << "分机号"<<"回路号"<<"地址号"<<"报警类型"<<"设备产品编号"
                  << "设备设施型号"<<"报警收到时间"<<"设备所属系统"<<"总保护区域名称"
                  << "建筑设施名称"<<"设施所在楼层"<<"设施所在位置"<<"值班人员";
    if(m_architePlanView->currentAlarmType()=="全部")
    {
        m_infoTableView->tableModel()->sqlCommit(QString("select %1 from AlarmInfo where 报警当前状态 = '正在报警'").arg(alarmInfoList.join(",")));
    }
    else
    {
        m_infoTableView->tableModel()->sqlCommit(QString("select %1 from AlarmInfo where 报警类型 ='%2' and 报警当前状态 = '正在报警'").arg(alarmInfoList.join(",")).arg(m_architePlanView->currentAlarmType()));
    }
}

