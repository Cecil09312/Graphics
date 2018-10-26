#include "crtwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QQuickView>
#include <QQmlComponent>
#include <QQmlContext>
CrtWidget::CrtWidget(QWidget *parent) :
    QWidget(parent)
{
    initWidget();
}

CrtWidget::~CrtWidget()
{
    delete m_alarmContainer;
    delete m_toolBarContainer;
}

void CrtWidget::widgetExit()
{
    close();
}

void CrtWidget::loginWidgetShow()
{
    m_loginQuickView->show();
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

    m_alarmContainer = QWidget::createWindowContainer(alarmQuickView, this,Qt::SubWindow);

    m_alarmContainer->setMinimumHeight(100);
    m_alarmContainer->setMinimumWidth(150);
    m_alarmContainer->setMaximumWidth(150);
    m_alarmContainer->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);

    QHBoxLayout *globalHLayout = new QHBoxLayout;
    globalHLayout->addWidget(m_alarmContainer);

    m_architePlanView = new ArchitePlanView(this);
    m_architePlanView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(m_architePlanView);
    m_infoTableView = new InfoTableView(this);
    m_infoTableView->setMaximumHeight(180);
    m_infoTableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    vLayout->addWidget(m_infoTableView);
    globalHLayout->addLayout(vLayout);
    globalVLayout->addLayout(globalHLayout);
    globalVLayout->setSpacing(0);
    globalVLayout->setContentsMargins(QMargins(0,0,0,0));
    setLayout(globalVLayout);
    m_loginQuickView = new QQuickView;
    m_loginQuickView->setSource(QUrl("qrc:/qml/LoginWindow.qml"));
    m_loginQuickView->setGeometry(500,50,m_loginQuickView->width(),m_loginQuickView->height());
   // loginQuickView->show();

}
