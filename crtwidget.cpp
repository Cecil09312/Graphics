#include "crtwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QQuickView>
#include <QQmlComponent>
#include <QQmlContext>
#include <QSplitter>
CrtWidget::CrtWidget(QWidget *parent) :
    QWidget(parent)
{
    initWidget();
    setWindowFlags(Qt::FramelessWindowHint);
}

CrtWidget::~CrtWidget()
{
    delete m_alarmContainer;
    delete m_toolBarContainer;
    delete m_loginQuickView;
}

void CrtWidget::widgetExit()
{
    close();
}

void CrtWidget::loginWidgetShow()
{
    m_loginQuickView->show();
}

void CrtWidget::toFirstFireAlarm()
{
    m_architePlanView->firstFireAlarm();
}

void CrtWidget::toLastFireAlarm()
{
    m_architePlanView->lastFireAlarm();
}

void CrtWidget::logWidgetClose()
{
    m_loginQuickView->close();
}

void CrtWidget::initWidget()
{
    QVBoxLayout *globalVLayout = new QVBoxLayout;

    QQuickView *toolBarQuickView = new QQuickView;
    toolBarQuickView->setSource(QUrl("qrc:/qml/ToolBarWindow.qml"));
    toolBarQuickView->rootContext()->setContextProperty("CrtWidget",this);
    m_toolBarContainer = QWidget::createWindowContainer(toolBarQuickView, this,Qt::SubWindow);

    m_toolBarContainer->setMinimumHeight(60);
    m_toolBarContainer->setMaximumHeight(60);
    m_toolBarContainer->setMinimumWidth(100);
    m_toolBarContainer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    globalVLayout->addWidget(m_toolBarContainer);

    QQuickView *alarmQuickView = new QQuickView;
    alarmQuickView->setSource(QUrl("qrc:/qml/AlarmItem.qml"));
    alarmQuickView->rootContext()->setContextProperty("CrtWidget",this);
    m_alarmContainer = QWidget::createWindowContainer(alarmQuickView, this,Qt::SubWindow);

    m_alarmContainer->setMinimumHeight(100);
    m_alarmContainer->setMinimumWidth(150);
    m_alarmContainer->setMaximumWidth(150);
    m_alarmContainer->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);

    QHBoxLayout *globalHLayout = new QHBoxLayout;
    globalHLayout->addWidget(m_alarmContainer);

    m_architePlanView = new ArchitePlanView(this);
    m_architePlanView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_infoTableView = new InfoTableView(this);
    m_infoTableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
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
    m_loginQuickView = new QQuickView;
    m_loginQuickView->setSource(QUrl("qrc:/qml/LoginWindow.qml"));
    m_loginQuickView->setGeometry(500,50,m_loginQuickView->width(),m_loginQuickView->height());
    m_loginQuickView->rootContext()->setContextProperty("CrtWidget",this);
   // loginQuickView->show();

}
