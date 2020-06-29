#include "firstfirealarminfowidget.h"
#include <QPalette>
#include <QDesktopWidget>
FirstFireAlarmInfoWidget::FirstFireAlarmInfoWidget(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowTitleHint);
    setWindowTitle(tr("首火警信息"));
    m_titleLabel = new QLabel(tr("首火警信息"),this);
    m_titleLabel->setFont(QFont("Times New Roman",12));
    m_titleLabel->setAlignment(Qt::AlignHCenter);
    m_titleLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
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

    m_formLayout = new QFormLayout;
    m_formLayout1 = new QFormLayout;
    m_formLayout2 = new QFormLayout;
    m_formHLayout = new QHBoxLayout;
    m_formLayout1->addRow(tr("分机号:"),m_extLabel);
    m_formLayout1->addRow(tr("回路号:"),m_loopLabel);
    m_formLayout2->addRow(tr("地址号:"),m_addrNumLabel);
    m_formLayout2->addRow(tr("网络号:"),m_networkNumLabel);
    m_formHLayout->addLayout(m_formLayout1);
    m_formHLayout->addLayout(m_formLayout2);
    m_formLayout->addRow(tr("楼层:"),m_floorLabel);
    m_formLayout->addRow(tr("设备:"),m_equipmentModelLabel);
    m_formLayout->addRow(tr("设备编码:"),m_deviceNumLabel);
    m_formLayout->addRow(tr("操作员:"),m_operatorLabel);
    m_formLayout->addRow(tr("位置:"),m_locationLabel);
    m_formLayout->addRow(tr("系统:"),m_sysLabel);
    m_formLayout->addRow(tr("建筑名称:"),m_buildingNameLabel);
    m_formLayout->addRow(tr("时间:"),m_alarmTimeLabel);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(m_titleLabel);
    vLayout->addLayout(m_formHLayout);
    vLayout->addLayout(m_formLayout);
    QHBoxLayout *btnHLayout = new QHBoxLayout;
    btnHLayout->addStretch(10);
    btnHLayout->addWidget(m_firstFireBtn);
    btnHLayout->addStretch(10);
    vLayout->addLayout(btnHLayout);
    setLayout(vLayout);
    resize(240,260);
    setMaximumHeight(320);
    setMinimumHeight(320);
    setMinimumWidth(240);
    setMaximumWidth(240);
    connect(m_firstFireBtn,&QPushButton::clicked,this,&FirstFireAlarmInfoWidget::toFirstFire);
   // qDebug() <<"isActive" <<this->isActiveWindow();
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

void FirstFireAlarmInfoWidget::retranslate()
{
    setWindowTitle(tr("首火警信息"));
    m_firstFireBtn->setText(tr("首火警"));
    m_titleLabel->setText(tr("首火警信息"));

     QLabel *floorLabel= dynamic_cast<QLabel*> (m_formLayout->labelForField(m_floorLabel));
     if(floorLabel!=nullptr)
     {
         floorLabel->setText(tr("楼层:"));
     }
     QLabel *equipmentModelLabel= dynamic_cast<QLabel*> (m_formLayout->labelForField(m_equipmentModelLabel));
     if(equipmentModelLabel!=nullptr)
     {
         equipmentModelLabel->setText(tr("设备:"));
     }
     QLabel *deviceNumLabel= dynamic_cast<QLabel*> (m_formLayout->labelForField(m_deviceNumLabel));
     if(deviceNumLabel!=nullptr)
     {
         deviceNumLabel->setText(tr("设备编码:"));
     }
     QLabel *operatorLabel= dynamic_cast<QLabel*> (m_formLayout->labelForField(m_operatorLabel));
     if(operatorLabel!=nullptr)
     {
         operatorLabel->setText(tr("操作员:"));
     }
     QLabel *locationLabel= dynamic_cast<QLabel*> (m_formLayout->labelForField(m_locationLabel));
     if(locationLabel!=nullptr)
     {
         locationLabel->setText(tr("位置:"));
     }
     QLabel *sysLabel= dynamic_cast<QLabel*> (m_formLayout->labelForField(m_sysLabel));
     if(sysLabel!=nullptr)
     {
         sysLabel->setText(tr("系统:"));
     }

     QLabel *buildingNameLabel= dynamic_cast<QLabel*> (m_formLayout->labelForField(m_buildingNameLabel));
     if(buildingNameLabel!=nullptr)
     {
         buildingNameLabel->setText(tr("建筑名称:"));
     }
     QLabel *alarmTimeLabel= dynamic_cast<QLabel*> (m_formLayout->labelForField(m_alarmTimeLabel));
     if(alarmTimeLabel!=nullptr)
     {
         alarmTimeLabel->setText(tr("时间:"));
     }

     QLabel *extLabel = dynamic_cast<QLabel*>(m_formLayout1->labelForField(m_extLabel));
     if(extLabel!=nullptr)
     {
         extLabel->setText(tr("分机号:"));
     }

     QLabel *loopLabel = dynamic_cast<QLabel*>(m_formLayout1->labelForField(m_loopLabel));
     if(loopLabel!=nullptr)
     {
         loopLabel->setText(tr("回路号:"));
     }


     QLabel *addrNumLabel= dynamic_cast<QLabel*>(m_formLayout2->labelForField(m_addrNumLabel));
     if(addrNumLabel!=nullptr)
     {
         addrNumLabel->setText(tr("地址号:"));
     }

     QLabel *networkNumLabel = dynamic_cast<QLabel*>(m_formLayout2->labelForField(m_networkNumLabel));
     if(networkNumLabel!=nullptr)
     {
         networkNumLabel->setText(tr("网络号:"));
     }

}

void FirstFireAlarmInfoWidget::updateGeometry()
{
   QPoint rightPoint= QApplication::desktop()->screen()->geometry().bottomRight();
   this->setGeometry(QRect(rightPoint.x()-240,rightPoint.y()-500,width(),height()));
}



void FirstFireAlarmInfoWidget::setFirstFireInfo(const QString &extNum, const QString &loopNum, const QString &addrNum,const QString &networkNum,const QString &timeStr)
{
    m_extLabel->setText(extNum);
    m_loopLabel->setText(loopNum);
    m_addrNumLabel->setText(addrNum);
    m_networkNumLabel->setText(networkNum);

    m_locationLabel->setText("");
    m_buildingNameLabel->setText("");
    m_deviceNumLabel->setText("");
    m_equipmentModelLabel->setText(tr("未定义设备"));
    m_alarmTimeLabel->setText(timeStr);
    m_floorLabel->setText("");
    m_operatorLabel->setText("");
    m_sysLabel->setText("");
}




