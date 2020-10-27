#include "firstalarminfowidget.h"
#include <QPalette>
#include <QDesktopWidget>
FirstAlarmInfoWidget::FirstAlarmInfoWidget(QWidget *parent)
    : QFrame(parent)
{

    this->setFont(QFont("Times new Roman",8));
    m_titleLabel = new QLabel(tr("首火警信息"),this);
    m_titleLabel->setFont(QFont("Times New Roman",9));
   // m_titleLabel->setAlignment(Qt::AlignHCenter);
    m_titleLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_fireGifLabel = new QLabel(this);
    m_alarmGifLabel = new QLabel(this);
    m_fireGifLabel->resize(50,50);
    m_alarmGifLabel->resize(50,50);


    setObjectName("firstAlarmFrame");



    m_fireGifLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    m_alarmGifLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    m_fireMovie = new QMovie(this);
    m_alarmMovie = new QMovie(this);
    //m_fireGifLabel->setStyleSheet("QLabel{border-image:url(:/images/fire.gif)}");
    m_fireMovie->setFileName(":/images/fire.gif");
    m_alarmMovie->setFileName(":/images/alarm.gif");
    m_fireMovie->setScaledSize(QSize(50,50));
    m_alarmMovie->setScaledSize(QSize(50,50));
    m_fireGifLabel->setMovie(m_fireMovie);
    m_alarmGifLabel->setMovie(m_alarmMovie);
    m_fireMovie->start();
    m_alarmMovie->start();
    m_extLabel = new QLabel(this);
    m_loopLabel = new QLabel(this);
    m_addrNumLabel = new QLabel(this);
    m_networkNumLabel  = new QLabel(this);
    m_locationLabel = new QLabel(this);
    m_buildingNameLabel = new QLabel(this);

    m_equipmentModelLabel = new QLabel(this);
    m_alarmTimeLabel = new QLabel(this);
    m_floorLabel = new QLabel(this);

    m_sysLabel = new QLabel(this);
    m_extLabel->setAlignment(Qt::AlignLeft);
    m_loopLabel->setAlignment(Qt::AlignLeft);
    m_networkNumLabel->setAlignment(Qt::AlignLeft);
    m_addrNumLabel->setAlignment(Qt::AlignLeft);

    m_floorLabel->setAlignment(Qt::AlignLeft);
   m_equipmentModelLabel->setAlignment(Qt::AlignLeft);


    m_locationLabel->setAlignment(Qt::AlignLeft);
    m_sysLabel->setAlignment(Qt::AlignLeft);
    m_buildingNameLabel->setAlignment(Qt::AlignLeft);
    m_alarmTimeLabel->setAlignment(Qt::AlignLeft);

    m_locationLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    m_formLayout = new QFormLayout;
    m_formLayout1 = new QFormLayout;
    m_formLayout2 = new QFormLayout;
    m_formHLayout = new QHBoxLayout;

    m_formLayout1->addRow(tr("分机号:"),m_extLabel);
    m_formLayout1->addRow(tr("回路号:"),m_loopLabel);

    m_formLayout2->addRow(tr("网络号:"),m_networkNumLabel);
    m_formLayout2->addRow(tr("地址号:"),m_addrNumLabel);

    m_formHLayout->addLayout(m_formLayout1);
    m_formHLayout->addLayout(m_formLayout2);
    m_formLayout->addRow(tr("楼层:"),m_floorLabel);
    m_formLayout->addRow(tr("设备:"),m_equipmentModelLabel);

    m_formLayout->addRow(tr("位置:"),m_locationLabel);
    m_formLayout->addRow(tr("系统:"),m_sysLabel);
    m_formLayout->addRow(tr("建筑:"),m_buildingNameLabel);
    m_formLayout->addRow(tr("时间:"),m_alarmTimeLabel);

    QVBoxLayout *vLayout = new QVBoxLayout;
    QHBoxLayout *titleHLayout = new QHBoxLayout;
    titleHLayout->addWidget(m_fireGifLabel);
    titleHLayout->addWidget(m_alarmGifLabel);
    titleHLayout->addWidget(m_titleLabel);

    vLayout->addLayout(titleHLayout);
    vLayout->addLayout(m_formHLayout);
    vLayout->addLayout(m_formLayout);


    setLayout(vLayout);

}

FirstAlarmInfoWidget::~FirstAlarmInfoWidget()
{
    m_fireMovie->stop();
    m_alarmMovie->stop();
}

void FirstAlarmInfoWidget::setFirstAlarmInfo(GraphicsItem *item)
{
    if(item!=nullptr)
    {
        m_titleLabel->setText(QString(tr("首%1信息")).arg(item->alarmType()));
        if(item->alarmType()==tr("火警"))
        {
            m_alarmGifLabel->hide();
            m_fireGifLabel->show();
            setStyleSheet("QFrame#firstAlarmFrame{border:2px solid red}");
        }
        else {
            m_alarmGifLabel->show();
            m_fireGifLabel->hide();
            setStyleSheet("QFrame#firstAlarmFrame{border:2px solid blue}");
        }


        m_extLabel->setText(item->extNum());
        m_loopLabel->setText(item->loopNum());
        m_addrNumLabel->setText(item->addrNum());
        m_networkNumLabel->setText(item->networkNum());
        m_locationLabel->setText(item->deviceLocation());
        m_buildingNameLabel->setText(item->buildingName());

        m_equipmentModelLabel->setText(item->equipmentModel());
        m_alarmTimeLabel->setText(item->alarmTime(item->alarmType()));
        m_floorLabel->setText(item->floorOfDevice());

        m_sysLabel->setText(item->sysOfDevice());
    }
}

void FirstAlarmInfoWidget::retranslate()
{


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
         buildingNameLabel->setText(tr("建筑:"));
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

void FirstAlarmInfoWidget::updateGeometry()
{
   QPoint rightPoint= QApplication::desktop()->screen()->geometry().bottomRight();
   this->setGeometry(QRect(rightPoint.x()-240,rightPoint.y()-500,width(),height()));
}

void FirstAlarmInfoWidget::showTitle()
{
    qDebug() << m_titleLabel->text();
}

QString FirstAlarmInfoWidget::extLabelInfo()
{
    return m_extLabel->text();
}



void FirstAlarmInfoWidget::setFirstAlarmInfo(const QString &extNum, const QString &loopNum, const QString &addrNum,const QString &networkNum,const QString &timeStr,const QString &alarmType)
{
    m_titleLabel->setText(QString(tr("首%1信息")).arg(alarmType));
    if(alarmType==tr("火警"))
    {
        m_alarmGifLabel->hide();
        m_fireGifLabel->show();
        setStyleSheet("QFrame#firstAlarmFrame{border:2px solid red}");
    }
    else {
        m_alarmGifLabel->show();
        m_fireGifLabel->hide();
        setStyleSheet("QFrame#firstAlarmFrame{border:2px solid blue}");
    }
    m_extLabel->setText(extNum);
    m_loopLabel->setText(loopNum);
    m_addrNumLabel->setText(addrNum);
    m_networkNumLabel->setText(networkNum);

    m_locationLabel->setText("");
    m_buildingNameLabel->setText("");

    m_equipmentModelLabel->setText(tr("未定义设备"));
    m_alarmTimeLabel->setText(timeStr);
    m_floorLabel->setText("");

    m_sysLabel->setText("");
}









