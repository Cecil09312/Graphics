#include "firstfirealarminfowidget.h"
#include <QPalette>
FirstFireAlarmInfoWidget::FirstFireAlarmInfoWidget(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowTitleHint);
    setWindowTitle(tr("首火警信息"));
    QLabel *titleLabel = new QLabel(tr("首火警信息"),this);
    titleLabel->setFont(QFont("Times New Roman",14));
    titleLabel->setAlignment(Qt::AlignHCenter);
    titleLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_firstFireBtn = new QPushButton(tr("首火警"),this);
    m_extLabel = new QLabel(this);
    m_loopLabel = new QLabel(this);
    m_addrNumLabel = new QLabel(this);
    m_networkNumLabel  = new QLabel(this);
    m_locationLabel = new QLabel(this);
    m_buildingNameLabel = new QLabel(this);
    m_deviceNumLabel = new QLabel(this);
    m_equipmentModelLabel = new QLabel(this);
    m_alarmTimeLabel = new QLabel(this);
    m_floorLabel = new QLabel(this);
    m_operatorLabel = new QLabel(this);
    m_sysLabel = new QLabel(this);

    m_firstFireBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    QFormLayout *fromLayout = new QFormLayout;
    fromLayout->addRow(tr("分机号:"),m_extLabel);
    fromLayout->addRow(tr("回路号:"),m_loopLabel);
    fromLayout->addRow(tr("地址号:"),m_addrNumLabel);
    fromLayout->addRow(tr("网络号:"),m_networkNumLabel);
    fromLayout->addRow(tr("位置:"),m_locationLabel);
    fromLayout->addRow(tr("设备:"),m_equipmentModelLabel);
    fromLayout->addRow(tr("设备编码:"),m_deviceNumLabel);
    fromLayout->addRow(tr("建筑名称:"),m_buildingNameLabel);
    fromLayout->addRow(tr("系统:"),m_sysLabel);
    fromLayout->addRow(tr("楼层:"),m_floorLabel);
    fromLayout->addRow(tr("报警时间:"),m_alarmTimeLabel);
    fromLayout->addRow(tr("操作员:"),m_operatorLabel);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(titleLabel);
    vLayout->addLayout(fromLayout);
    QHBoxLayout *btnVLayout = new QHBoxLayout;
    btnVLayout->addStretch(10);
    btnVLayout->addWidget(m_firstFireBtn);
    btnVLayout->addStretch(10);
    vLayout->addLayout(btnVLayout);
    setLayout(vLayout);
    resize(240,320);
    setMaximumHeight(320);
    setMinimumHeight(320);
    setMinimumWidth(240);
    setMaximumWidth(240);
    connect(m_firstFireBtn,&QPushButton::clicked,this,&FirstFireAlarmInfoWidget::toFirstFire);
}

FirstFireAlarmInfoWidget::~FirstFireAlarmInfoWidget()
{

}

void FirstFireAlarmInfoWidget::setFirstFireInfo(GraphicsItem *item)
{
    if(item!=nullptr)
    {
        m_extLabel->setText(item->extNum());
        m_loopLabel->setText(item->loopNum());
        m_addrNumLabel->setText(item->addrNum());
        m_networkNumLabel->setText(item->networkNum());
        m_locationLabel->setText(item->deviceLocation());
        m_buildingNameLabel->setText(item->buildingName());
        m_deviceNumLabel->setText(item->deviceNum());
        m_equipmentModelLabel->setText(item->equipmentModel());
        m_alarmTimeLabel->setText(item->getItemInfo().m_alarmTime);
        m_floorLabel->setText(item->floorOfDevice());
        m_operatorLabel->setText(item->deviceOperator());
        m_sysLabel->setText(item->sysOfDevice());
    }
}


