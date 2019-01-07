#include "crtwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QQuickView>
#include <QQmlComponent>
#include <QQmlContext>
#include <QSplitter>
#include <QQuickItem>
#include "commnication/commobj.h"
#include "control/usermanager.h"
#include "control/controller.h"
#include <QHostAddress>
CrtWidget::CrtWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{

    initWidget();
    setWindowFlags(Qt::FramelessWindowHint);

    QObject *alarmObj = m_alarmQuickView->rootObject();
    Q_ASSERT(alarmObj);
    connect(m_architePlanView,&ArchitePlanView::alarmHappend,this,[=](const QString &type)
    {
        if(type==tr("火警"))
        {
            QMetaObject::invokeMethod(alarmObj,"setFireAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            //QMetaObject::invokeMethod(alarmObj,"startFireAnimation",Q_ARG(QVariant,true));
            QMetaObject::invokeMethod(alarmObj,"setFireAlarmText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
        }
        else if(type==tr("联动"))
        {
            QMetaObject::invokeMethod(alarmObj,"setLinkageAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            //QMetaObject::invokeMethod(alarmObj,"startLinkageAnimation",Q_ARG(QVariant,true));
            QMetaObject::invokeMethod(alarmObj,"setLinkageText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
        }
        else if(type==tr("监管"))
        {
            QMetaObject::invokeMethod(alarmObj,"setSuperviseAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
           // QMetaObject::invokeMethod(alarmObj,"startSuperviseAnimation",Q_ARG(QVariant,true));
            QMetaObject::invokeMethod(alarmObj,"setSuperviseText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
        }
        else if(type==tr("故障"))
        {
            QMetaObject::invokeMethod(alarmObj,"setfaultAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
           // QMetaObject::invokeMethod(alarmObj,"startFaultAnimation",Q_ARG(QVariant,true));
            QMetaObject::invokeMethod(alarmObj,"setFaultText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
        }
        else if(type==tr("反馈"))
        {
            QMetaObject::invokeMethod(alarmObj,"setFeedbackColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
           // QMetaObject::invokeMethod(alarmObj,"startFeedbackAnimation",Q_ARG(QVariant,true));
            QMetaObject::invokeMethod(alarmObj,"setFeedbackText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
        }
        else if(type==tr("屏蔽"))
        {
            QMetaObject::invokeMethod(alarmObj,"setShieldAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
           // QMetaObject::invokeMethod(alarmObj,"startShieldAnimation",Q_ARG(QVariant,true));
            QMetaObject::invokeMethod(alarmObj,"setShieldText",Q_ARG(QVariant,m_architePlanView->numOfTypeAlarm(type)));
        }

    });
    //qDebug() << CommObj::baudRates();
}

CrtWidget::~CrtWidget()
{
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


    qmlRegisterSingletonType<Controller>("serialPortInfo", 1, 0, "SerialPortInfo",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getCommObj();
    });

    qmlRegisterSingletonType<Controller>("sysArchitePlanView", 1, 0, "SysArchitePlanView",
                                         [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Controller::instance()->getSysArchitePlanView();
    });
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

    m_alarmQuickView = new QQuickView;
    m_alarmQuickView->setSource(QUrl("qrc:/qml/alarmItem/AlarmItem.qml"));
    m_alarmQuickView->rootContext()->setContextProperty("CrtWidget",this);
    m_alarmQuickView->rootContext()->setContextProperty("ArchitePlanView",m_architePlanView);

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

    Q_ASSERT(m_alarmQuickView);
    QObject *alarmObj = m_alarmQuickView->rootObject();
    Q_ASSERT(alarmObj);
    if(m_architePlanView->getWidgetMap().size()>0)
    {
        QMetaObject::invokeMethod(alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
    }
    else
    {
        QMetaObject::invokeMethod(alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
    }
    QMetaObject::invokeMethod(alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()));
    connect(m_architePlanView,&ArchitePlanView::toFirstPage,this,[=](){

        QMetaObject::invokeMethod(alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()));

    });

    connect(m_architePlanView,&ArchitePlanView::toLastPage,this,[=](){
        QMetaObject::invokeMethod(alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()));

    });

    connect(m_architePlanView,&ArchitePlanView::normalPage,this,[=](){

        QMetaObject::invokeMethod(alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()));
    });

    connect(m_architePlanView,&ArchitePlanView::noPage,this,[=](){

        QMetaObject::invokeMethod(alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()));
    });
}
