#include "firstfirealarminfowidget.h"
#include <QPalette>
FirstFireAlarmInfoWidget::FirstFireAlarmInfoWidget(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowTitleHint);
    setWindowTitle(tr("首火警信息"));
    QLabel *titleLabel = new QLabel(tr("首火警信息"),this);
    titleLabel->setFont(QFont("宋体",12));
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
    m_extLabel->setAlignment(Qt::AlignLeft);
    m_loopLabel->setAlignment(Qt::AlignLeft);
    m_networkNumLabel->setAlignment(Qt::AlignLeft);
    m_addrNumLabel->setAlignment(Qt::AlignLeft);

    m_floorLabel->setAlignment(Qt::AlignLeft);
    m_equipmentModelLabel->setAlignment(Qt::AlignLeft);
    m_deviceNumLabel->setAlignment(Qt::AlignLeft);
    m_operatorLabel->setAlignment(Qt::AlignLeft);

    m_locationLabel->setAlignment(Qt::AlignLeft);
    m_sysLabel->setAlignment(Qt::AlignLeft);
    m_buildingNameLabel->setAlignment(Qt::AlignLeft);
    m_alarmTimeLabel->setAlignment(Qt::AlignLeft);

    m_firstFireBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);

    QFormLayout *formLayout = new QFormLayout;
    QFormLayout *formLayout1 = new QFormLayout;
    QFormLayout *formLayout2 = new QFormLayout;
    QHBoxLayout *formHLayout = new QHBoxLayout;
    formLayout1->addRow(tr("分机号:"),m_extLabel);
    formLayout1->addRow(tr("回路号:"),m_loopLabel);
    formLayout2->addRow(tr("地址号:"),m_addrNumLabel);
    formLayout2->addRow(tr("网络号:"),m_networkNumLabel);
    formHLayout->addLayout(formLayout1);
    formHLayout->addLayout(formLayout2);
    formLayout->addRow(tr("楼层:"),m_floorLabel);
    formLayout->addRow(tr("设备:"),m_equipmentModelLabel);
    formLayout->addRow(tr("设备编码:"),m_deviceNumLabel);
    formLayout->addRow(tr("操作员:"),m_operatorLabel);
    formLayout->addRow(tr("位置:"),m_locationLabel);
    formLayout->addRow(tr("系统:"),m_sysLabel);
    formLayout->addRow(tr("建筑名称:"),m_buildingNameLabel);
    formLayout->addRow(tr("报警时间:"),m_alarmTimeLabel);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(titleLabel);
    vLayout->addLayout(formHLayout);
    vLayout->addLayout(formLayout);
    QHBoxLayout *btnHLayout = new QHBoxLayout;
    btnHLayout->addStretch(10);
    btnHLayout->addWidget(m_firstFireBtn);
    btnHLayout->addStretch(10);
    vLayout->addLayout(btnHLayout);
    setLayout(vLayout);
    resize(200,260);
    setMaximumHeight(260);
    setMinimumHeight(260);
    setMinimumWidth(200);
    setMaximumWidth(200);
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
        m_alarmTimeLabel->setText(item->alarmTime(item->alarmType()));
        m_floorLabel->setText(item->floorOfDevice());
        m_operatorLabel->setText(item->deviceOperator());
        m_sysLabel->setText(item->sysOfDevice());
    }
}


