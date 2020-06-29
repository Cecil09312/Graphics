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
#include <QNetworkConfigurationManager>


CrtWidget::CrtWidget(QWidget *parent) :
    QWidget(parent),
    m_monitoringPackageNum(0),
    m_heartbeatIndex(0),
    m_tcpIsConnected(false),
    m_serialConnected(false),
    m_sendEnable(true),
    m_serialConnectState(Disconnected),
    m_tcpConnectState(false),
    m_serialCurState(false),
    m_receiveDataIsSuccess(false)

{

#ifdef Q_OS_LINUX
    setWindowFlags(Qt::FramelessWindowHint|Qt::Window|Qt::WindowStaysOnBottomHint);
#endif

    QString dbName = QCoreApplication::applicationDirPath()+"/alarmInfo.db";
    m_alarmInfoDbName = dbName;
    m_sysNameHash[0] = "";
    m_sysNameHash[1]=tr("电气火灾监控系统");
    m_sysNameHash[2]=tr("火灾探测报警系统");
    m_sysNameHash[3]=tr("消防联动系统");
    m_sysNameHash[4]=tr("自动喷水灭火系统");
    m_sysNameHash[5]=tr("水喷雾灭火系统");
    m_sysNameHash[6]=tr("消火栓系统");
    m_sysNameHash[7]=tr("气体灭火系统");
    m_sysNameHash[8]=tr("泡沫灭火系统");
    m_sysNameHash[9]=tr("干粉灭火系统");
    m_sysNameHash[10]=tr("防烟排烟系统");
    m_sysNameHash[11]=tr("防火及卷帘系统");
    m_sysNameHash[12]=tr("电梯系统");
    m_sysNameHash[13]=tr("消防电话系统");
    m_sysNameHash[14]=tr("消防应急广播系统");
    m_sysNameHash[15]=tr("消防应急照明和疏散指示系统");
    m_sysNameHash[16]=tr("消防电源系统");


    m_alarmTypeHash[0x01] = tr("火警");
    m_alarmTypeHash[0x03] = tr("故障");
    m_alarmTypeHash[0x05] = tr("启动");
    m_alarmTypeHash[0x0a] = tr("监管");
    m_alarmTypeHash[0x0b] = tr("屏蔽");
    m_alarmTypeHash[0x0d] = tr("反馈");

    m_alarmTypeHash[0x02] = tr("反馈消除");
    m_alarmTypeHash[0x04] = tr("故障恢复");
    m_alarmTypeHash[0x06] = tr("停止");
    m_alarmTypeHash[0x0c] = tr("屏蔽解除");

    m_commuStatusHash[0x12] = tr("主电");
    m_commuStatusHash[0x13] = tr("备电");
    m_commuStatusHash[0x31] = tr("手自动设置");
    m_commuStatusHash[0x32] = tr("手自动确认");
    m_faultStateHash[1] = tr("过压");
    m_faultStateHash[3] = tr("欠压");
    m_faultStateHash[5] = tr("缺相");
    m_faultStateHash[7] = tr("错相");

    m_faultStateHash[2] = tr("过压恢复");
    m_faultStateHash[4] = tr("欠压恢复");
    m_faultStateHash[6] = tr("缺相恢复");
    m_faultStateHash[8] = tr("错相恢复");

    m_faultStateHash[9] = tr("过流");
    m_faultStateHash[11] = tr("霍尔故障");
    m_faultStateHash[13] = tr("断电");
    m_faultStateHash[15] = tr("模块故障");

    m_faultStateHash[10] = tr("过流恢复");
    m_faultStateHash[12] = tr("霍尔故障恢复");
    m_faultStateHash[14] = tr("断电恢复");
    m_faultStateHash[16] = tr("模块故障恢复");



    m_emergencyStateHash[0x09]=tr("通信故障");
    m_emergencyStateHash[0x0b]=tr("光源故障");
    m_emergencyStateHash[0x0c]=tr("自检故障");
    m_emergencyStateHash[0x19]=tr("屏蔽");
    m_emergencyStateHash[0x14]=tr("应急启动");
    m_emergencyStateHash[0x9a] =tr("设备丢失");

    m_emergencyStateHash[0x89] = tr("灯具故障恢复");
    m_emergencyStateHash[0x8b] = tr("光源故障恢复");
    m_emergencyStateHash[0x8c] = tr("自检故障恢复");  //故障恢复
    m_emergencyStateHash[0x99] = tr("屏蔽解除");//屏蔽解除
    m_emergencyStateHash[0x94] = tr("应急解除");//应急解除

    m_emergencyTypeHash[0x20] =tr("输出开路");
    m_emergencyTypeHash[0x21] =tr("进入月检");
    m_emergencyTypeHash[0x22] =tr("进入年检");
    m_emergencyTypeHash[0x1a] =tr("新注册");
    m_emergencyTypeHash[0x1b] =tr("开机");
    m_emergencyTypeHash[0x1c] =tr("消音");
    m_emergencyTypeHash[0x1e] =tr("复位");
    m_emergencyTypeHash[0x1F] =tr("重码");
    m_emergencyTypeHash[0x13] =tr("输出过载");


    m_emergencyTypeHash[0xa0] =tr("开路恢复");
    m_emergencyTypeHash[0xa1] =tr("月检结束");
    m_emergencyTypeHash[0xa2] =tr("年检结束");

    m_emergencyTypeHash[0x9b] =tr("关机");
    m_emergencyTypeHash[0x9c] =tr("消音取消");
    m_emergencyTypeHash[0x9f] =tr("重码恢复");
    m_emergencyTypeHash[0x93] =tr("输出过载恢复");

    //m_emergencyTypeHash[0x9a] =tr("设备丢失");
    m_emergencyTypeHash[0x29] =tr("灯具改变方向");


    m_emergencyDeviceTypeHash[0x01] = tr("壁挂式应急照明控制器");
    m_emergencyDeviceTypeHash[0x02] = tr("入柜式应急照明控制器");
    m_emergencyDeviceTypeHash[0x03] = tr("分配电装置");
    m_emergencyDeviceTypeHash[0x04] = tr("虚拟分配电装置");
    m_emergencyDeviceTypeHash[0x05] = tr("集中电源250w");
    m_emergencyDeviceTypeHash[0x06] = tr("集中电源500w");
    m_emergencyDeviceTypeHash[0x07] = tr("集中电源1000w");
    QHash<quint8,QString>emeygencyTailHash;
    emeygencyTailHash[0] = tr("双面单向");emeygencyTailHash[1] = tr("双面双向");emeygencyTailHash[2] = tr("安全出口");
    emeygencyTailHash[3] = tr("语音安全出口");emeygencyTailHash[4] = tr("楼层标志");emeygencyTailHash[5] = tr("左向");

    emeygencyTailHash[6] = tr("右向");emeygencyTailHash[7] = tr("双向");emeygencyTailHash[8] = tr("安全出口");
    emeygencyTailHash[9] = tr("语音安全出口");emeygencyTailHash[10] = tr("楼层标志");

    emeygencyTailHash[11] = tr("左向"); emeygencyTailHash[12] = tr("右向");emeygencyTailHash[13] = tr("双向");
    emeygencyTailHash[14] = tr("安全出口");emeygencyTailHash[15] = tr("语音安全出口");emeygencyTailHash[16] = tr("楼层标志");
    for(int i=0x31;i<=0x74;i++)
    {
        if(i>=0x31&&i<=0x35)
        {
            m_emergencyDeviceTypeHash[i] = tr("小型吊挂")+emeygencyTailHash.value(i-0x31);
        }
        else if(i>=0x36&&i<=0x3b)
        {
            m_emergencyDeviceTypeHash[i] = tr("小型嵌墙")+emeygencyTailHash.value(i-0x31);
        }
        else if(i>=0x3c&&i<=0x41)
        {
            m_emergencyDeviceTypeHash[i] = tr("小型壁挂")+emeygencyTailHash.value(i-0x31);
        }

        if(i>=0x42&&i<=0x46)
        {
            m_emergencyDeviceTypeHash[i] = tr("中型吊挂")+emeygencyTailHash.value(i-0x42);
        }
        else if(i>=0x47&&i<=0x4c)
        {
            m_emergencyDeviceTypeHash[i] = tr("中型嵌墙")+emeygencyTailHash.value(i-0x42);
        }
        else if(i>=0x4d&&i<=0x52)
        {
            m_emergencyDeviceTypeHash[i] = tr("中型壁挂")+emeygencyTailHash.value(i-0x42);
        }




        if(i>=0x53&&i<=0x57)
        {
            m_emergencyDeviceTypeHash[i] = tr("大型吊挂")+emeygencyTailHash.value(i-0x53);
        }
        else if(i>=0x58&&i<=0x5d)
        {
            m_emergencyDeviceTypeHash[i] = tr("大型嵌墙")+emeygencyTailHash.value(i-0x53);
        }
        else if(i>=0x5e&&i<=0x63)
        {
            m_emergencyDeviceTypeHash[i] = tr("大型壁挂")+emeygencyTailHash.value(i-0x53);
        }

        if(i>=0x64&&i<=0x68)
        {
            m_emergencyDeviceTypeHash[i] = tr("特大型吊挂")+emeygencyTailHash.value(i-0x64);
        }
        else if(i>=0x69&&i<=0x6e)
        {
            m_emergencyDeviceTypeHash[i] = tr("特大型嵌墙")+emeygencyTailHash.value(i-0x64);
        }
        else if(i>=0x6f&&i<=0x74)
        {
            m_emergencyDeviceTypeHash[i] = tr("特大型壁挂")+emeygencyTailHash.value(i-0x64);
        }
    }
    QHash<quint8,QString> tmpHash;
    tmpHash[0] = "3W";tmpHash[1] = "5W";tmpHash[2] = "8W";

    tmpHash[3] = "12W";tmpHash[4] = "15W";tmpHash[5] = "18W";
    tmpHash[3] = "25W";tmpHash[4] = "40W";
    for(int i=0x7c;i<=0xa3;i++)
    {
        if(i>=0x7c&&i<=0x83)
        {
            m_emergencyDeviceTypeHash[i] = tr("壁挂式应急照明灯具")+tmpHash.value(i-0x7c);
        }
        else if(i>=0x84&&i<=0x8b)
        {
            m_emergencyDeviceTypeHash[i] = tr("吸顶式应急照明灯具")+tmpHash.value(i-0x84);
        }
        else if(i>=0x8c&&i<=0x93)
        {
            m_emergencyDeviceTypeHash[i] = tr("嵌入式应急照明灯具")+tmpHash.value(i-0x8c);
        }
        else if(i>=0x94&&i<=0x9b)
        {
            m_emergencyDeviceTypeHash[i] = tr("吊装式应急照明灯具")+tmpHash.value(i-0x94);
        }
        else if(i>=0x9c&&i<=0xa3)
        {
            m_emergencyDeviceTypeHash[i] = tr("应急照明灯管")+tmpHash.value(i-0x9c);
        }

    }

    m_emergencyDeviceTypeHash[0x75] = tr("Ⅰ型地埋单向");
    m_emergencyDeviceTypeHash[0x76] = tr("Ⅰ型地埋双向");
    m_emergencyDeviceTypeHash[0x77] = tr("Ⅱ型地埋单向");
    m_emergencyDeviceTypeHash[0x78] = tr("Ⅱ型地埋双向");
    m_emergencyDeviceTypeHash[0x79] = tr("Ⅲ型地埋单向");
    m_emergencyDeviceTypeHash[0x7a] = tr("Ⅲ型地埋双向");
    m_emergencyDeviceTypeHash[0x7b] = tr("Led信息屏蔽疏散标志灯");
    m_emergencyDeviceTypeHash[0xa4] = tr("红外感应开关");
    m_emergencyDeviceTypeHash[0xa5] = tr("模块开关");
    m_emergencyDeviceTypeHash[0xa6] = tr("现场电源");
    m_emergencyDeviceTypeHash[0xa7] = tr("小型分配电装置");
    m_emergencyDeviceTypeHash[0xb1] = tr("中型吊挂疏散出口");
    m_emergencyDeviceTypeHash[0xb2] = tr("中型嵌墙疏散出口");
    m_emergencyDeviceTypeHash[0xb3] = tr("中型壁挂疏散出口");



    m_typeList << tr("火警") << tr("启动") << tr("监管") << tr("故障")<< tr("反馈") << tr("屏蔽");
    // m_language = Chinese;
    initWidget();

    // m_debugLogMsg = new DebugLogMsg(nullptr);
    Controller::instance()->getOperatorInfo()->insertEvent(tr("系统开机"));
    m_sqliteManager->setDataBase("QSQLITE","alarmInfo","","","",dbName,888);
    m_sqliteManager->open();
    if(m_sqliteManager->isOpen())
    {
        QDateTime curDateTime = QDateTime::currentDateTime();
        curDateTime = curDateTime.addYears(-3);
        QString curDateTimeStr = curDateTime.toString("yyyy/MM/dd hh:mm:ss");

        QStringList tableNameList = m_sqliteManager->getTables();
        if(!tableNameList.contains("AlarmInfo"))
        {
            m_sqliteManager->executeQuery("create table AlarmInfo(分机号 text, 回路号 text,地址号 text,网络号 text,电源地址 text,设备编码 text ,设备 text,事件类型 text,状态 text,时间 text,系统 text,建筑名称 text,楼层 text,位置 text,制造商 text,有效期 text,操作员 text,备注 text)");
        }
        else
        {

            m_sqliteManager->executeQuery(QString("delete from AlarmInfo where 时间 <='%1'").arg(curDateTimeStr));

        }

        if(!tableNameList.contains("AnalogInfo"))
        {
            m_sqliteManager->executeQuery("create table AnalogInfo(分机号 text, 回路号 text,地址号 text,网络号 text,当前通道 int, 模拟量类型 text ,结果 text,时间 text)");
        }
        else
        {

            m_sqliteManager->executeQuery(QString("delete from AnalogInfo where 时间 <='%1'").arg(curDateTimeStr));

        }


    }


    m_infoTableView->hide();
    m_infoTableView->tableModel()->setDbDriver("QSQLITE");
    m_infoTableView->tableModel()->setDbName(dbName);
    m_infoTableView->tableModel()->setDbPort(888);
    m_infoTableView->tableModel()->setDbOpen(true);
    m_infoTableView->setAlternatingRowColors(true);




    QStringList alarmInfoList,valueList;
    alarmInfoList << "分机号"<<"回路号"<<"地址号"<<"网络号"<<"电源地址"<<"设备编码" << "设备" <<"事件类型"<<"状态"<<"时间"
                  <<"系统"<< "建筑名称"<<"楼层"<<"位置"<< "制造商" << "有效期" <<"操作员"<<"备注";

    for(int i=0;i<alarmInfoList.size();i++)
    {
        QString str = QString("'%%1'").arg(i+1);
        valueList.push_back(str);
    }
    m_alarmSqlInfo = QString("insert into AlarmInfo (%1) values (%2)").arg(alarmInfoList.join(",")).arg(valueList.join(","));
    m_updateAlarmSqlInfo = QString("update AlarmInfo set 时间 ='%1' where 分机号='%2' and 回路号='%3' and 地址号='%4' and 网络号='%5' and 状态!='正常'");

    // connect(m_architePlanView,&ArchitePlanView::alarmHappend,this,&CrtWidget::alarmStatistics);
    connect(m_architePlanView,&ArchitePlanView::alarmItem,this,[=](GraphicsItem *item,const QString &alarmType)
    {
        if(item!=nullptr)
        {
            QString infoRecord;
            if(!alarmType.startsWith(tr("模拟")))
            {
                infoRecord=item->alarmState(alarmType);
            }
            else
            {
                infoRecord=tr("模拟")+item->alarmState(alarmType);
            }
            m_sqliteManager->executeQuery(m_alarmSqlInfo.arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()).arg(item->powerAddr()).arg(item->deviceNum())
                                          .arg(item->equipmentModel()).arg(alarmType).arg(item->alarmState(alarmType)).arg(item->alarmTime(alarmType))
                                          .arg(item->sysOfDevice()).arg(item->buildingName())
                                          .arg(item->floorOfDevice()).arg(item->deviceLocation()).arg(item->manufacturers()).arg(item->periodOfValidity())
                                          .arg(item->deviceOperator()).arg(infoRecord));
            //alarmDataOnTable();

        }


    });

    connect(m_infoTableView,&InfoTableView::tableValue,this,[=](QSqlRecord record)
    {
        QString extNum=  record.value("分机号").toString();
        QString loopNum=  record.value("回路号").toString();
        QString addrNum=  record.value("地址号").toString();
        QString networkNum=  record.value("网络号").toString();
        QString powerAddr=  record.value("电源地址").toString();
        m_architePlanView->toArchitePlan(extNum,loopNum,addrNum,networkNum,powerAddr);

    });



    connect(m_infoTableView,&InfoTableView::fitToWiew,this,[=]()
    {
        GraphicsView *view = m_architePlanView->currentGraphicsView();
        if(view!=nullptr)
        {
            m_architePlanView->autoFitView(view);
        }

    });

    connect(m_architePlanView,&ArchitePlanView::sendAnalogValue,this,[=](GraphicsItem*curItem,quint8 channelNum){
        if(curItem!=nullptr)
        {
            quint8 networkNum =quint8(curItem->networkNum().toUShort()) ;
            quint8 loopNum = quint8(curItem->loopNum().toUShort());
            quint8 extNum = quint8(curItem->extNum().toUShort());
            quint8 addrNum = quint8(curItem->addrNum().toUShort());
            qDebug() <<"loopNum" <<loopNum;
            sendAnalogCommand(networkNum,extNum,loopNum,addrNum,channelNum,1);
        }

    });

    connect(m_architePlanView,&ArchitePlanView::clearAlarmFromTable,this,[=]()
    {
        // QString eliminateTime =  QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 状态 = '%1' where 状态 != '%1'").arg(tr("正常")));
        //m_curEventLabel->clear();
        //alarmDataOnTable();
    });

    connect(m_architePlanView,&ArchitePlanView::eliminateAlarmFromTable,this,[=](GraphicsItem *item,const QString &alarmType,const QString &eliminateTime)
    {

        QString curEnent ="";
        //QString curAlarmType = ;
        if(!alarmType.endsWith(tr("火警"),Qt::CaseInsensitive)&&!alarmType.endsWith(tr("监管"),Qt::CaseInsensitive))
        {
            if(alarmType.endsWith(tr("故障"),Qt::CaseInsensitive))
            {
                curEnent= alarmType+tr("恢复");
            }
            else if(alarmType.endsWith(tr("启动"),Qt::CaseInsensitive))
            {
                curEnent= tr("停止");
            }

            else if(alarmType.endsWith(tr("反馈"),Qt::CaseInsensitive))
            {
                curEnent= alarmType+tr("消除");
            }

            else if(alarmType.endsWith(tr("屏蔽"),Qt::CaseInsensitive))
            {
                curEnent= alarmType+tr("解除");
            }
            m_sqliteManager->executeQuery(m_alarmSqlInfo.arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()).arg(item->powerAddr()).arg(item->deviceNum())
                                          .arg(item->equipmentModel()).arg(curEnent).arg(item->alarmState(alarmType)).arg(eliminateTime)
                                          .arg(item->sysOfDevice()).arg(item->buildingName())
                                          .arg(item->floorOfDevice()).arg(item->deviceLocation()).arg(item->manufacturers()).arg(item->periodOfValidity())
                                          .arg(item->deviceOperator()).arg(item->alarmState(alarmType)));

        }
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 状态 = '%1' where 分机号='%2' and 回路号='%3' and 地址号='%4' and 网络号='%5' and 事件类型='%6' ").arg(tr("正常")).arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()).arg(alarmType));

        //alarmDataOnTable();

    });

    connect(m_architePlanView,&ArchitePlanView::clearTableAlarm,this,[=]()
    {
        clearCurrentAlarm();
        //alarmDataOnTable();
        QMetaObject::invokeMethod(m_mainPowerObj,"clearMainPowerState");
        QMetaObject::invokeMethod(m_standbyPowerObj,"clearStandbyPowerState");
        QMetaObject::invokeMethod(m_handOrAutoObj,"clearHandOrAutoState");

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


    connect(m_infoTableView,&InfoTableView::showAlalogValue,this,[=]()
    {
        m_isAnalog = false;
        m_sqliteManager->processAnalogData(false);
        m_infoTableView->tableModel()->sqlCommit("select * from AnalogInfo order by 时间 desc limit 10000");
    });

    connect(m_infoTableView,&InfoTableView::showAlarmValue,this,&CrtWidget::alarmDataOnTable);
    connect(Controller::instance()->getCommObj(),&AbstractLink::getData,m_serialDataProtocol,&SerialDataProtocol::startProcessData);

    connect(m_serialDataProtocol,&SerialDataProtocol::finishProcessData,this,&CrtWidget::serialDataProcessing);

    //    connect(m_dealDataTimer,&QTimer::timeout,this,[=](){
    //            QMutexLocker locker(&m_mutex);
    //            if(m_recesiveDataList.size()>0)
    //            {
    //                QByteArray firstArray = m_fireDataList.first();
    //                serialDataProcessing(firstArray);
    //                m_recesiveDataList.removeFirst();
    //            }
    //            else
    //            {
    //                m_dealDataTimer->stop();
    //            }
    //    });
    // connect(m_serialDataProtocol,&SerialDataProtocol::processDataFinished,this,&CrtWidget::setTransportState);
    connect(Controller::instance()->getTcpObj(),&AbstractLink::getData,this,&CrtWidget::tcpDataProcessing);

    //    connect(Controller::instance()->getTcpObj(),&AbstractLink::errorInfo,this,[=](const QString &error)
    //    {
    //        Q_UNUSED(error);
    //        if(m_sendEnable)
    //        {
    //            // setIndicatorState(false);
    //        }

    //    });

    connect(Controller::instance()->getTcpObj(),&AbstractLink::sendDataSuccess,this,&CrtWidget::setTransportState);

    connect(m_ftpManager,&FtpManager::sendFileSuccess,this,[=](bool isOk)
    {
        if(m_sendEnable)
        {
            setIndicatorState(isOk);
        }

    });

    connect(m_ftpManager,&FtpManager::ftpError,this,[=](const QString&error)
    {
        Q_UNUSED(error)
        if(m_sendEnable)
        {
            // qDebug() << "error";
            //sendAlarmInfo(0,3,4,0,QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
            //setIndicatorState(false);
        }

    });




    connect(m_architePlanView,&ArchitePlanView::alarmStateUpdate,this,[=]
            (const QString &extNum)
    {
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 状态 = '%1' where 状态 != '%1' and 分机号= '%2'").arg(tr("正常")).arg(extNum));
        QMetaObject::invokeMethod(m_mainPowerObj,"deleteMainPowerState",Q_ARG(QVariant,extNum));
        QMetaObject::invokeMethod(m_standbyPowerObj,"deleteStandbyPowerState",Q_ARG(QVariant,extNum));
        QMetaObject::invokeMethod(m_handOrAutoObj,"deleteHandOrAutoState",Q_ARG(QVariant,extNum));
        QList<QString>mainPowerStateList= m_mainPowerStateHash.keys();
        foreach(const QString&curPowerState,mainPowerStateList)
        {
            if(curPowerState.startsWith(extNum))
            {
                m_mainPowerStateHash.remove(curPowerState);
            }
        }
        if(m_mainPowerStateHash.values().contains(tr("故障")))
        {
            QMetaObject::invokeMethod(m_alarmObj,"setMainPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setMainPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }
        QList<QString>standbyPowerStateList= m_standbyPowerStateHash.keys();
        foreach(const QString&curPowerState,standbyPowerStateList)
        {
            if(curPowerState.startsWith(extNum))
            {
                m_standbyPowerStateHash.remove(curPowerState);
            }
        }

        if(m_standbyPowerStateHash.values().contains(tr("故障")))
        {
            QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        }

        foreach (const QString &extAndNetwork, m_handOrAutoStateHash.keys())
        {
            if(extAndNetwork.startsWith(extNum))
            {
                m_handOrAutoStateHash.remove(extAndNetwork);
            }
        }
        updateHandOrAutoState();
        allAlarmStatistics();


    });

    connect(m_architePlanView,&ArchitePlanView::clearAllAlarmExceptFire,this,[=]()
    {
        insertOtherAlarmInfo(tr("复位"),"","");
        clearCheckExtInfo();
        m_checkSendDataTimer->stop();
#ifdef Q_OS_LINUX
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 1 0\"");
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 2 0\"");
#endif
    });

    connect(m_architePlanView,&ArchitePlanView::keepStartState,this,[=]()
    {
        Controller::instance()->getSpeechObj()->removeAlarmText(tr("传输故障"));
        Controller::instance()->getSpeechObj()->removeAlarmText(tr("主电故障"));
        Controller::instance()->getSpeechObj()->removeAlarmText(tr("备电故障"));
        QMetaObject::invokeMethod(m_alarmObj,"setTransformColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
        QMetaObject::invokeMethod(m_alarmObj,"setMainPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,tr("自动")));
    });


    connect(m_controlCenterHeartbeatTimer,&CustomTimer::timeout,this,[=]()
    {

        QMutex mutex;
        QMutexLocker lock(&mutex);
#ifdef Q_OS_LINUX
        sigsetjmp(sigEnv,1);
#endif
        static quint8 num=0;
        if(m_sendEnable)
        {
            if(m_tcpIsConnected)
            {
                num=0;
                QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
                Controller::instance()->getSpeechObj()->removeAlarmText(tr("中心通信故障"));
                if(!m_tcpConnectState)
                {
                    updateOtherAlarmInfo(tr("中心通信故障"),tr("中心通信故障恢复"));
                    m_tcpConnectState = true;
                }

                m_tcpIsConnected = false;
                sendControlCenterHeartbeat();
            }
            else
            {
                num++;
                if(num>=18)
                {
                    num=0;
                    if(m_tcpConnectState)
                    {
                        QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));
                        if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("中心通信故障")))
                        {
                            Controller::instance()->getSpeechObj()->insertAlarmText(tr("中心通信故障"));
                        }

                        m_checkSendDataTimer->stop();
                        m_controlCenterHeartbeatTimer->stop();
                        insertOtherAlarmInfo(tr("中心通信故障"));
                        m_tcpConnectState = false;

                    }

                }
                else
                {
                    sendControlCenterHeartbeat();
                }

            }
        }
#ifdef Q_OS_LINUX
        signal(SIGSEGV, segFaultReceive);
#endif
    });


    connect(this,&CrtWidget::checkExtNum,this,[=](quint8 curExtNum,quint8 networkNum)
    {
        QString valueStr = QString("%1,%2").arg(curExtNum).arg(networkNum);
        m_extNumTimesHash[valueStr]=0;
        //        if(!m_extOnlineStateHash.contains(valueStr))
        //        {
        //            m_extOnlineStateHash[valueStr]=NoState;
        //            m_extAndNetworkStateHash[valueStr] = true;
        //            if(m_extNumObj!=nullptr)
        //            {
        //                QVariant curNetworkNum ;
        //                QMetaObject::invokeMethod(m_extNumObj,"networkNum",Q_RETURN_ARG(QVariant,curNetworkNum));
        //                if(curNetworkNum.toString()==QString::number(networkNum))
        //                {
        //                    QMetaObject::invokeMethod(m_extNumObj,"setIndicatorState",Q_ARG(QVariant,curExtNum),Q_ARG(QVariant,"green"));
        //                }
        //            }
        //        }
    });


    connect(m_serialDataProtocol,&AbstractDataProtocol::errorFrameData,this,[=](const QByteArray &errorArray)
    {
        quint8 packageNum=   m_serialDataProtocol->dataByte(errorArray,1);
        QByteArray array ;
        array.resize(3);
        array[0] = packageNum;
        array[1] = 0x03;
        array[2] = 0xef;
        QList<QByteArray> sendArrayList;
        sendArrayList.push_back(array.mid(0,1));
        sendArrayList.push_back(array.mid(1,1));
        sendArrayList.push_back(array.mid(2,1));
        QByteArray curArray = m_serialDataProtocol->dataArray(sendArrayList);
        Controller::instance()->getCommObj()->sendData(curArray);
    });





    connect(m_sqliteManager,&SqliteManager::finishedProcessData,this,[=](){

        alarmDataOnTable();

    });

    connect(m_sqliteManager,&SqliteManager::finishUpdateData,this,[=](){
        m_infoTableView->tableModel()->sqlCommit("select * from AnalogInfo order by 时间 desc limit 10000");
        m_isAnalog = false;
        m_sqliteManager->processAnalogData(false);
    });

    //    connect(m_sqliteManager,&SqliteManager::selectData,this,[=](const QSqlQuery &query)
    //    {
    //        m_infoTableView->tableModel()->sqlCommit(query);
    //        alarmDataOnTable();
    //        allAlarmStatistics();

    //    });



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
    //connect(m_alarmObj,SIGNAL(currentAlarmType(QString)),this,SLOT(alarmChanged(QString)));
    connect(m_alarmObj,SIGNAL(reset()),this,SLOT(resetLoginViewShow()));
    connect(m_architePlanView,&ArchitePlanView::toFirstPage,this,[=]()
    {
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()+1));
    });




    connect(m_architePlanView,&ArchitePlanView::reduInstruction,this,[=](bool isOk)
    {

        QMetaObject::invokeMethod(m_alarmObj,"allAlarmClear",Q_ARG(QVariant,isOk));
        if(m_sendEnable)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setTransformColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
        }
        m_receiveDataIsSuccess = false;

        QMetaObject::invokeMethod(m_extNumObj,"resetIndicatorState");
        QMetaObject::invokeMethod(m_handOrAutoObj,"clearHandOrAutoState");
        QMetaObject::invokeMethod(m_standbyPowerObj,"clearStandbyPowerState");
        QMetaObject::invokeMethod(m_mainPowerObj,"clearMainPowerState");
        m_extNumStateHash.clear();
        m_extAndNetworkStateHash.clear();
        m_extOnlineStateHash.clear();
        m_mainPowerStateHash.clear();
        m_standbyPowerStateHash.clear();
        m_handOrAutoStateHash.clear();

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
            // QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//主机通信
            //Controller::instance()->getSpeechObj()->removeAlarmText(tr("主机通信故障"));
            m_serialConnected = true;
            if(!m_mainHeartBeatTimer->isActive())
            {
                m_mainHeartBeatTimer->start(c_mainHeartBeatTime);
            }
            reSendAllCmd();

        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//
            if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("主机通信故障")))
            {
                Controller::instance()->getSpeechObj()->insertAlarmText(tr("主机通信故障"));

            }
            QString curTime=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
            insertOtherAlarmInfo(tr("主机通信故障"));
            m_serialCurState= false;
            m_serialConnectState=Disconnected;
            sendAlarmInfo(0,3,5,0,curTime);
            m_mainHeartBeatTimer->stop();
            closeAllOnlineState();
            // Controller::instance()->getCommObj()->connectLink();//重新连接
        }
    });

    connect(Controller::instance()->getTcpObj(),&TcpLink::isConnected,this,[=](bool connected)
    {
        if(m_sendEnable)
        {
            if(connected)
            {
                m_tcpIsConnected = true;
                QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//
                Controller::instance()->getSpeechObj()->removeAlarmText(tr("中心通信故障"));
                if(!m_tcpConnectState)
                {
                    updateOtherAlarmInfo(tr("中心通信故障"),tr("中心通信故障恢复"));
                    m_tcpConnectState = true;
                }

                if(!m_controlCenterHeartbeatTimer->isActive())
                {
                    m_controlCenterHeartbeatTimer->start(c_heartBeatTime);
                }

            }
            else
            {
                QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//黄色
                Controller::instance()->getSpeechObj()->insertAlarmText(tr("中心通信故障"));
                if(m_tcpConnectState)
                {
                    insertOtherAlarmInfo(tr("中心通信故障"));
                    m_tcpConnectState = false;
                }

                m_tcpIsConnected = false;
                m_controlCenterHeartbeatTimer->stop();

            }
        }

    });

    connect(m_mainHeartBeatTimer,&CustomTimer::timeout,this,[=]()
    {
        QMutex mutex;
        QMutexLocker lock(&mutex);
#ifdef Q_OS_LINUX
        sigsetjmp(sigEnv,1);
#endif
        static quint8 num =0;
        if(m_serialConnected)
        {
            num=0;
            m_serialConnected = false;
            if(m_serialConnectState==Disconnected)
            {
                m_serialConnectState = Connected;

            }
            if(!m_serialCurState)
            {
                updateOtherAlarmInfo(tr("主机通信故障"),tr("主机通信故障恢复"));
                m_serialCurState= true;
            }


            QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//主机通信
            Controller::instance()->getSpeechObj()->removeAlarmText(tr("主机通信故障"));

        }
        else
        {
            num++;
            if(num>=c_mainHeartBeatTimeOut/c_mainHeartBeatTime)
            {
                num=0;
                if(m_serialCurState)
                {
                    QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//黄色
                    if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("主机通信故障")))
                    {
                        Controller::instance()->getSpeechObj()->insertAlarmText(tr("主机通信故障"));
                    }

                    insertOtherAlarmInfo(tr("主机通信故障"));
                    m_serialCurState= false;
                    QString curTime=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
                    sendAlarmInfo(0,3,5,0,curTime);

                    closeAllOnlineState();

                }


            }
            else
            {
                if(num>=2)
                {
                    m_serialConnectState=Disconnected;
                }

            }
        }
        if(m_serialConnectState==Connected)
        {
            reSendAllCmd();
            m_serialConnectState= NoState;
        }
#ifdef Q_OS_LINUX
        signal(SIGSEGV, segFaultReceive);
#endif
    });

    connect(m_checkSendDataTimer,&CustomTimer::timeout,this,[=]()
    {
        bool sendDataState= Controller::instance()->getTcpObj()->writeDataSuccess();
        setIndicator(sendDataState);
    });

    connect(m_updateTimer,&CustomTimer::timeout,this,&CrtWidget::updateTime);

    connect(this,&CrtWidget::getSerialData,this,[=]()
    {

        if(!m_serialCurState)
        {
            updateOtherAlarmInfo(tr("主机通信故障"),tr("主机通信故障恢复"));
            //insertOtherAlarmInfo(tr("主机通信故障恢复"));
            //m_serialConnectState=Connected;
        }
        if(!m_receiveDataIsSuccess)
        {
            m_serialCurState=true;
            m_serialConnectState= NoState;
            QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//主机通信
            Controller::instance()->getSpeechObj()->removeAlarmText(tr("主机通信故障"));
            m_receiveDataIsSuccess = true;
        }


    });

    connect(this,&CrtWidget::getTcpData,this,[=]()
    {
        if(!m_tcpConnectState)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//中心通信
            Controller::instance()->getSpeechObj()->removeAlarmText(tr("中心通信故障"));
            m_tcpConnectState = true;
            insertOtherAlarmInfo(tr("中心通信故障恢复"));
        }
    });


    connect(m_setVisibleTimer,&CustomTimer::timeout,this,[=]()
    {
        QMetaObject::invokeMethod(m_toolBarObj,"setProgramTxtVisible",Q_ARG(QVariant,false));
    });



    bool controlCenterEnable= Controller::instance()->getTransportInfo()->transportInfoFromJson(tr("控制中心权限")).toBool();
    QMetaObject::invokeMethod(m_settingObj,"startEnableControlCenter",Q_ARG(QVariant,controlCenterEnable));
    if(m_sendEnable)
    {
        Controller::instance()->getSpeechObj()->insertAlarmText(tr("中心通信故障"));
        insertOtherAlarmInfo(tr("中心通信故障"));
        Controller::instance()->getTcpObj()->setConfiguration();
        Controller::instance()->getTcpObj()->connectLink();
        // m_controlCenterSocket =dynamic_cast<QTcpSocket*> (Controller::instance()->getTcpObj()->device());
    }
    //    connect(Controller::instance()->getTcpObj(),&AbstractLink::sendAllData,this,[=]()
    //    {
    //        QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,true));
    //        setIndicatorState(true);
    //    });

#ifdef Q_OS_LINUX
    startProcess("sh -c \"echo rpdzkj|sudo -S su\"");
    startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 0 1\"");
#elif defined (Q_OS_WIN)
    m_architePlanView->itemIconSetting();
    QMetaObject::invokeMethod(m_settingObj,"setSysArchiteInfo");
#endif


}

CrtWidget::~CrtWidget()
{

    m_controlCenterHeartbeatTimer->stop();
    m_controlCenterHeartbeatTimer->deleteLater();
    m_mainHeartBeatTimer->stop();
    m_mainHeartBeatTimer->deleteLater();
    m_checkSendDataTimer->stop();
    m_checkSendDataTimer->deleteLater();
    m_checkExtNumTimer->stop();
    m_checkExtNumTimer->deleteLater();
    m_updateTimer->stop();
    m_updateTimer->deleteLater();
    m_sqliteManager->close();
    m_sqliteManager->deleteLater();
    m_infoQueryView->close();
    m_infoQueryView->deleteLater();
    m_extNumStateView->deleteLater();
    m_setVisibleTimer->stop();
    m_setVisibleTimer->deleteLater();


    //qApp->removeTranslator(m_translator );
    //m_translator->deleteLater();

    m_resetLoginQuickView->deleteLater();

    m_mainPowerView->deleteLater();
    m_standbyPowerView->deleteLater();
    m_handOrAutoView->deleteLater();

    clearCheckExtInfo();
    delete m_alarmContainer;
    delete m_toolBarContainer;
    m_loginQuickView->close();
    m_loginQuickView->deleteLater();
    m_closeLoginView->close();
    m_closeLoginView->deleteLater();
    m_settingView->close();
    m_settingView->deleteLater();
    //m_serialDataProtocol->quit();
    m_serialDataProtocol->deleteLater() ;
    m_monitoringProtocol->deleteLater();
    // delete m_indicatorProtocol;
    m_ftpManager->deleteLater();
    // m_debugLogMsg->deleteLater();

    closeSys();
    //m_process.close();
}

QString CrtWidget::alarmInfoDbName()
{
    return m_alarmInfoDbName;
}

void CrtWidget::queryViewShow()
{
    m_infoQueryView->close();//关闭以前的打开
    m_infoQueryView->show();
}

void CrtWidget::transportIndicator(bool isOk)
{
#ifdef Q_OS_LINUX
    if(isOk)
    {
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 2 1\"");
    }
    else
    {
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 2 0\"");
    }
#elif defined Q_OS_WIN
    Q_UNUSED(isOk);
#endif

}

bool CrtWidget::isEnglish()
{
    return Controller::instance()->getSpeechObj()->isEnglish();
}

void CrtWidget::setIsEnglish(bool isEnglish)
{
    Controller::instance()->getSpeechObj()->setIsEnglish(isEnglish);
}



void CrtWidget::setControlCenterEnable(bool enable)
{
    m_sendEnable = enable;
    if(!enable)
    {
        m_controlCenterHeartbeatTimer->stop();
        Controller::instance()->getTcpObj()->disconnectLink();
        QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));//中心通信
        QMetaObject::invokeMethod(m_alarmObj,"setTransformColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));//传输指示
        Controller::instance()->getSpeechObj()->removeAlarmText(tr("中心通信故障"));
        Controller::instance()->getSpeechObj()->removeAlarmText(tr("传输故障"));
#ifdef Q_OS_LINUX
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 1 0\"");
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 2 0\"");
#endif
        //        updateOtherAlarmInfo(tr("中心通信故障"));
        //        updateOtherAlarmInfo(tr("传输故障"));

    }
    else
    {
        QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//中心通信
        Controller::instance()->getTcpObj()->connectLink();
        if(!m_controlCenterHeartbeatTimer->isActive())
        {
            m_controlCenterHeartbeatTimer->start(c_heartBeatTime);
        }
    }
}

void CrtWidget::closeControlCenterHeartbeat()
{
    m_controlCenterHeartbeatTimer->stop();
}

void CrtWidget::reSendAllCmd()
{
    reSendCmd();
}

void CrtWidget::closeAll()
{
    clearCurrentAlarm();
    close();
}

void CrtWidget::showDeviceOnlineSetting()
{
    if(m_deviceOnlineSettingView!=nullptr)
    {
        m_deviceOnlineSettingView->show();
    }
}

void CrtWidget::clearCurrentAlarm()
{
    m_sqliteManager->executeQuery(QString("update AlarmInfo set 状态 = '%1' where 状态 != '%1'").arg(tr("正常")));
}

void CrtWidget::setDeviceOnlineState(QString sNetworkNum)
{
    if(sNetworkNum.isEmpty())
    {
        return;
    }
    if(m_extNumStateView!=nullptr)
    {
        QMetaObject::invokeMethod(m_extNumObj,"resetVisibleAll");
        QList<QString>extAndNetworkList=   m_extAndNetworkStateHash.keys();
        foreach (QString extNetworkStr, m_extEnableHash.keys())
        {
            if(extNetworkStr.endsWith(sNetworkNum))
            {
                QStringList curValueList = extNetworkStr.split(",");
                if(curValueList.size()>0)
                {
                    QString curExtNum = curValueList.at(0);
                    QMetaObject::invokeMethod(m_extNumObj,"setVisible",Q_ARG(QVariant,curExtNum.toInt()),Q_ARG(QVariant,true));


                }

            }
        }
        foreach (QString curValue, extAndNetworkList)
        {
            QStringList valueList = curValue.split(",");
            QString networkNum,extNum;
            if(valueList.size()>1)
            {
                extNum = valueList.at(0);
                networkNum=valueList.at(1);
            }
            if(sNetworkNum==networkNum)
            {
                QString colorName="gray";

                if(m_extAndNetworkStateHash.contains(curValue))
                {
                    if(m_extAndNetworkStateHash.value(curValue))
                    {
                        colorName = "green";
                    }
                    else
                    {
                        colorName = "yellow";
                    }
                }


                QMetaObject::invokeMethod(m_extNumObj,"setIndicatorState",Q_ARG(QVariant,extNum.toInt()),Q_ARG(QVariant,colorName));
            }


        }



    }
}

void CrtWidget::noItemInfoSetting(const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum, const QString &alarmType, const QString &alarmState, const QString &time, const QString &remarks, const QString&powerAddr)
{

    Controller::instance()->getDataStore()->insertTypeNoItem(alarmType,extNum,loopNum,addrNum,networkNum);

    m_sqliteManager->executeQuery(QString("insert into AlarmInfo (分机号,回路号,地址号,网络号,电源地址,事件类型,状态,设备,时间,备注) values ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10')").arg(extNum)
                                  .arg(loopNum).arg(addrNum).arg(networkNum).arg(powerAddr).arg(alarmType).arg(alarmState).arg(tr("未定义设备")).arg(time).arg(remarks));
    QString curAlarmSpeechText =alarmType+";"+QString("%1,%2,%3,%4").arg(extNum).arg(loopNum).arg(addrNum).arg(networkNum);
    Controller::instance()->getSpeechObj()->insertAlarmText(curAlarmSpeechText);

    //alarmDataOnTable();
    //alarmStatistics(alarmType);
}

void CrtWidget::noItemUpdate(const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &networkNum, const QString &oldAlarmType, const QString &newAlarmType, const QString &newAlarmState, const QString &time, const QString &remarks, const QString &powerAddr)
{

    m_sqliteManager->executeQuery(QString("insert into AlarmInfo (分机号,回路号,地址号,网络号,电源地址,事件类型,状态,设备,时间,备注) values ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10')").arg(extNum)
                                  .arg(loopNum).arg(addrNum).arg(networkNum).arg(powerAddr).arg(newAlarmType).arg(newAlarmState).arg(tr("未定义设备")).arg(time).arg(remarks));
    m_sqliteManager->executeQuery(QString("update AlarmInfo set 状态 = '%1' where 分机号='%2' and 回路号='%3' and 地址号='%4' and 网络号='%5' and 电源地址 ='%6' and 事件类型='%7' ").arg(tr("正常")).arg(extNum)
                                  .arg(loopNum).arg(addrNum).arg(networkNum).arg(powerAddr).arg(oldAlarmType));
    Controller::instance()->getDataStore()->deleteType(oldAlarmType,extNum,loopNum,addrNum,networkNum);
    //alarmDataOnTable();
    //alarmStatistics(oldAlarmType);
    QString speechTxt = oldAlarmType+";"+QString("%1,%2,%3,%4").arg(extNum).arg(loopNum).arg(addrNum).arg(networkNum);
    Controller::instance()->getSpeechObj()->removeAlarmText(speechTxt);
}



void CrtWidget::resetLoginViewShow()
{
    QMetaObject::invokeMethod(m_resetLoginObj,"clearLoginInfo");
    //m_resetLoginQuickView->close();
    m_resetLoginQuickView->show();

}

void CrtWidget::resetLoginViewClose()
{
    m_resetLoginQuickView->close();
}

void CrtWidget::closeQuickView()
{
    m_loginQuickView->close();
    m_alarmQuickView->close();
    m_settingView->close();
    m_infoQueryView->close();
    m_architePlanView->closeQuickView();
    m_resetLoginQuickView->close();
    m_closeLoginView->close();
    m_extNumStateView->close();
    m_mainPowerView->close();
    m_standbyPowerView->close();
    m_handOrAutoView->close();
    m_deviceOnlineSettingView->close();

}

void CrtWidget::retranslate()
{
    m_architePlanView->retranslate();
    m_infoTableView->retranslate();
    m_mainPowerView->setTitle(tr("主电状态"));
    m_standbyPowerView->setTitle(tr("备电状态"));
    m_handOrAutoView->setTitle(tr("手自动状态"));
    m_extNumStateView->setTitle(tr("主机在线状态"));
    m_loginQuickView->setTitle(tr("用户登陆界面"));
    m_closeLoginView->setTitle(tr("用户登陆之后关闭"));
    m_settingView->setTitle(tr("信息设置界面"));
    m_resetLoginQuickView->setTitle(tr("复位权限登陆"));
    m_infoQueryView->setTitle(tr("信息查询界面"));

    QMetaObject::invokeMethod(m_alarmObj,"retranslate");
    QMetaObject::invokeMethod(m_resetLoginObj,"retranslate");
    QMetaObject::invokeMethod(m_loginObj,"retranslate");
    QMetaObject::invokeMethod(m_closeLoginObj,"retranslate");
    QMetaObject::invokeMethod(m_toolBarObj,"retranslate");
    QMetaObject::invokeMethod(m_extNumObj,"retranslate");
    QMetaObject::invokeMethod(m_settingObj,"retranslate");
    QMetaObject::invokeMethod(m_infoQueryObj,"retranslate");
    QMetaObject::invokeMethod(m_deviceOnleSettingObj,"retranslate");

}

void CrtWidget::initArchiteView()
{
    m_architePlanView->setGlobalArchiteFromJson();
    m_architePlanView->initFromJsonFile();
}


void CrtWidget::readDeviceOnlineInfoFromJson()
{
    QmlForJson qmlForJson;
    QVariant value=  qmlForJson.readFile(QApplication::applicationDirPath()+"/deviceOnline.json");
    QHash<QString,QVariant>valueHash = value.toHash();

    if(!valueHash.isEmpty())
    {
        foreach(const QString& network,valueHash.keys())
        {
            QString extStr=  valueHash.value(network).toString();
            if(extStr=="all")
            {
                for(int i=0;i<256;i++)
                {
                    QString extAndNetwork = QString("%1,%2").arg(i).arg(network);
                    m_extEnableHash[extAndNetwork] = true;
                    m_allCheckedHash[network.toInt()] = true;
                    m_extNumTimesHash[extAndNetwork]=0;
                }
            }
            else
            {
                QStringList extList= extStr.split(",");
                foreach(const QString &ext,extList)
                {
                    QString extAndNetwork = QString("%1,%2").arg(ext).arg(network);
                    m_extEnableHash[extAndNetwork] = true;
                    m_extNumTimesHash[extAndNetwork]=0;
                }
            }

        }
    }

    if(!m_checkExtNumTimer->isActive())
    {
        m_checkExtNumTimer->start(1000);
    }

}


void CrtWidget::closeEvent(QCloseEvent *event)
{

    Controller::instance()->getSerialConfigurationManager()->saveConfiguration();
    Controller::instance()->getTcpConfigurationManager()->saveConfiguration();
    Controller::instance()->getTransportInfo()->saveTransportInfoToJson();
    Controller::instance()->getOperatorInfo()->insertEvent(tr("系统关机"));
    Controller::instance()->getSpeechObj()->saveSpeechInfoToJson();
    m_architePlanView->saveInfo();
    m_sqliteManager->quitThread();
    saveOnlineInfoToJson();

#ifdef Q_OS_LINUX
    startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 1 0\"");
    startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 2 0\"");
#endif

    closeQuickView();

    event->accept();
}


void CrtWidget::widgetExit()
{

#ifdef Q_OS_LINUX
    m_closeLoginView->show();
    if(m_closeLoginObj!=nullptr)
    {
        QMetaObject::invokeMethod(m_closeLoginObj,"clearLoginInfo");
        QMetaObject::invokeMethod(m_closeLoginObj,"initToGeneralUser");
    }
#elif defined (Q_OS_WIN)
    closeAll();
#endif

}

void CrtWidget::closeLogInView()
{
    m_closeLoginView->close();//关闭以前打开的
}

void CrtWidget::loginWidgetShow()
{
    // m_loginQuickView->close();//关闭以前打开的
    m_loginQuickView->show();
    if(m_loginObj!=nullptr)
    {
        QMetaObject::invokeMethod(m_loginObj,"clearLoginInfo");
    }
}

void CrtWidget::settingWindowShow()
{

    UserManager::UserRight userRight=   Controller::instance()->getUserRight();
    if(userRight!=UserManager::User)
    {
        QMetaObject::invokeMethod(m_settingObj,"setPassword");
        m_settingView->close();//关闭以前的打开
        m_settingView->show();
    }
    else
    {
        m_settingView->close();
        QMessageBox::critical(nullptr,tr("警告"),tr("此权限不允许打开，请重新登陆到其它权限。"));
    }

}


void CrtWidget::logWidgetClose()
{
    m_loginQuickView->close();
}

void CrtWidget::alarmChanged(QString alarm)
{
    m_architePlanView->setCurrentAlarmType(alarm);
    int currentPage = m_architePlanView->currentPage();
    int totalPage = m_architePlanView->totalPage();
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

    if(type.endsWith(tr("火警"),Qt::CaseInsensitive))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        }

        QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmText",Q_ARG(QVariant,typeNum));
    }
    else if(type.endsWith(tr("启动"),Qt::CaseInsensitive))
    {
        if(typeNum>0)
        {

            QMetaObject::invokeMethod(m_alarmObj,"setLinkageAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        }

        QMetaObject::invokeMethod(m_alarmObj,"setLinkageText",Q_ARG(QVariant,typeNum));
    }
    else if(type.endsWith(tr("监管"),Qt::CaseInsensitive))
    {

        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setSuperviseAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setSuperviseAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
        }
        QMetaObject::invokeMethod(m_alarmObj,"setSuperviseText",Q_ARG(QVariant,typeNum));
    }
    else if(type.endsWith(tr("故障"),Qt::CaseInsensitive))
    {
        if(typeNum>0)
        {

            QMetaObject::invokeMethod(m_alarmObj,"setfaultAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setfaultAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
        }
        QMetaObject::invokeMethod(m_alarmObj,"setFaultText",Q_ARG(QVariant,typeNum));
    }
    else if(type.endsWith(tr("反馈"),Qt::CaseInsensitive))
    {
        if(typeNum>0)
        {

            QMetaObject::invokeMethod(m_alarmObj,"setRespondColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setRespondColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
        }
        QMetaObject::invokeMethod(m_alarmObj,"setRespondText",Q_ARG(QVariant,typeNum));
    }
    else if(type.endsWith(tr("屏蔽"),Qt::CaseInsensitive))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setShieldAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"orange"));//橘黄色
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setShieldAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
        }
        QMetaObject::invokeMethod(m_alarmObj,"setShieldText",Q_ARG(QVariant,typeNum));
    }
}

void CrtWidget::communicationStatus(const QString &status, bool isOK,const QString&extNum,const QString&networkNum)
{
    Q_ASSERT(m_alarmObj);
    if(status.endsWith(tr("主电"),Qt::CaseInsensitive))
    {
        if(isOK)
        {
            updateOtherAlarmInfo(tr("主电故障"),tr("主电正常"),extNum,networkNum);
            QMetaObject::invokeMethod(m_mainPowerObj,"addMainPowerState",Q_ARG(QVariant,extNum),Q_ARG(QVariant,networkNum),Q_ARG(QVariant,""),Q_ARG(QVariant,tr("主电正常")));
            m_mainPowerStateHash[QString("%1,%2").arg(extNum).arg(networkNum)]=tr("正常");

            if(!m_mainPowerStateHash.values().contains(tr("故障")))
            {
                QMetaObject::invokeMethod(m_alarmObj,"setMainPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
                Controller::instance()->getSpeechObj()->removeAlarmText(tr("主电故障"));
            }
        }
        else
        {
            m_mainPowerStateHash[QString("%1,%2").arg(extNum).arg(networkNum)]=tr("故障");
            QMetaObject::invokeMethod(m_alarmObj,"setMainPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//主电故障
            Controller::instance()->getSpeechObj()->insertAlarmText(tr("主电故障"));
            updateOtherAlarmInfo(tr("主电正常"),tr("主电故障"),extNum,networkNum);
            // m_mainPowerHash[extNum] = status+tr("故障");
            QMetaObject::invokeMethod(m_mainPowerObj,"addMainPowerState",Q_ARG(QVariant,extNum),Q_ARG(QVariant,networkNum),Q_ARG(QVariant,""),Q_ARG(QVariant,tr("主电故障")));

        }

    }
    else if(status.endsWith(tr("备电"),Qt::CaseInsensitive))
    {
        if(isOK)
        {

            updateOtherAlarmInfo(tr("备电故障"),tr("备电正常"),extNum,networkNum);
            QMetaObject::invokeMethod(m_standbyPowerObj,"addStandbyPowerState",Q_ARG(QVariant,extNum),Q_ARG(QVariant,networkNum),Q_ARG(QVariant,""),Q_ARG(QVariant,tr("备电正常")));
            m_standbyPowerStateHash[QString("%1,%2").arg(extNum).arg(networkNum)]=tr("正常");

            if(!m_standbyPowerStateHash.values().contains(tr("故障")))
            {
                Controller::instance()->getSpeechObj()->removeAlarmText(tr("备电故障"));
                QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
            }
        }
        else
        {
            m_standbyPowerStateHash[QString("%1,%2").arg(extNum).arg(networkNum)]=tr("故障");
            QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//备电故障
            //QMetaObject::invokeMethod(m_alarmObj,"startStandbyPowerAnimation",Q_ARG(QVariant,true));
            Controller::instance()->getSpeechObj()->insertAlarmText(tr("备电故障"));
            updateOtherAlarmInfo(tr("备电正常"),tr("备电故障"),extNum,networkNum);
            QMetaObject::invokeMethod(m_standbyPowerObj,"addStandbyPowerState",Q_ARG(QVariant,extNum),Q_ARG(QVariant,networkNum),Q_ARG(QVariant,""),Q_ARG(QVariant,tr("备电故障")));

        }
    }

}

void CrtWidget::emergencyPowerState(const QString &status, bool isOK, const QString &extNum, const QString &loopNum, const QString &addrNum, const QString &powerAddr, const QString &networkNum, const QString &reMark)
{
    Q_ASSERT(m_alarmObj);
    QString curTime=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    if(status.endsWith(tr("主电"),Qt::CaseInsensitive))
    {
        if(isOK)
        {
            QMetaObject::invokeMethod(m_mainPowerObj,"addMainPowerState",Q_ARG(QVariant,extNum),Q_ARG(QVariant,networkNum),Q_ARG(QVariant,powerAddr),Q_ARG(QVariant,tr("主电正常")));
            m_mainPowerStateHash[QString("%1,%2,%3").arg(extNum).arg(networkNum).arg(powerAddr)]=tr("正常");
            if(!m_mainPowerStateHash.values().contains(tr("故障")))
            {
                QMetaObject::invokeMethod(m_alarmObj,"setMainPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
                Controller::instance()->getSpeechObj()->removeAlarmText(tr("主电故障"));
            }

            m_sqliteManager->executeQuery(QString("insert into AlarmInfo (分机号,回路号,地址号,电源地址,网络号,事件类型,状态,时间,备注) values ('%1','%2','%3','%4','%5','%6','%7','%8','%9') ").arg(extNum).arg(loopNum).arg(addrNum).arg(powerAddr).arg(networkNum).arg(tr("主电正常")).arg(tr("主电正常")).arg(curTime).arg(reMark));
            m_sqliteManager->executeQuery(QString("update AlarmInfo set 状态='%1' where 状态 != '%1' and 分机号= '%2' and 回路号='%3' and 地址号='%4' and 电源地址='%5' and 网络号= '%6'  and 事件类型 ='%7' ").arg(tr("正常")).arg(extNum).arg(loopNum).arg(addrNum).arg(powerAddr).arg(networkNum).arg(tr("主电故障")));

        }
        else
        {
            m_mainPowerStateHash[QString("%1,%2,%3").arg(extNum).arg(networkNum).arg(powerAddr)]=tr("故障");
            QMetaObject::invokeMethod(m_alarmObj,"setMainPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//主电故障
            Controller::instance()->getSpeechObj()->insertAlarmText(tr("主电故障"));
            QMetaObject::invokeMethod(m_mainPowerObj,"addMainPowerState",Q_ARG(QVariant,extNum),Q_ARG(QVariant,networkNum),Q_ARG(QVariant,powerAddr),Q_ARG(QVariant,tr("主电故障")));
            m_sqliteManager->executeQuery(QString("insert into AlarmInfo (分机号,回路号,地址号,电源地址,网络号,事件类型,状态,时间,备注) values ('%1','%2','%3','%4','%5','%6','%7','%8','%9') ").arg(extNum).arg(loopNum).arg(addrNum).arg(powerAddr).arg(networkNum).arg(tr("主电故障")).arg(tr("主电故障")).arg(curTime).arg(reMark));
            m_sqliteManager->executeQuery(QString("update AlarmInfo set 状态='%1' where 状态 != '%1' and 分机号= '%2' and 回路号='%3' and 地址号='%4' and 电源地址='%5' and 网络号= '%6'  and 事件类型 ='%7' ").arg(tr("正常")).arg(extNum).arg(loopNum).arg(addrNum).arg(powerAddr).arg(networkNum).arg(tr("主电正常")));
        }

    }
    else if(status.endsWith(tr("备电"),Qt::CaseInsensitive))
    {
        if(isOK)
        {
            QMetaObject::invokeMethod(m_standbyPowerObj,"addStandbyPowerState",Q_ARG(QVariant,extNum),Q_ARG(QVariant,networkNum),Q_ARG(QVariant,powerAddr),Q_ARG(QVariant,tr("备电正常")));
            m_standbyPowerStateHash[QString("%1,%2,%3").arg(extNum).arg(networkNum).arg(powerAddr)]=tr("正常");
            if(!m_standbyPowerStateHash.values().contains(tr("故障")))
            {
                Controller::instance()->getSpeechObj()->removeAlarmText(tr("备电故障"));
                QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
            }
            m_sqliteManager->executeQuery(QString("insert into AlarmInfo (分机号,回路号,地址号,电源地址,网络号,事件类型,状态,时间,备注) values ('%1','%2','%3','%4','%5','%6','%7','%8','%9') ").arg(extNum).arg(loopNum).arg(addrNum).arg(powerAddr).arg(networkNum).arg(tr("备电正常")).arg(tr("备电正常")).arg(curTime).arg(reMark));
            m_sqliteManager->executeQuery(QString("update AlarmInfo set 状态='%1' where 状态 != '%1' and 分机号= '%2' and 回路号='%3' and 地址号='%4' and 电源地址='%5' and 网络号= '%6'  and 事件类型 ='%7' ").arg(tr("正常")).arg(extNum).arg(loopNum).arg(addrNum).arg(powerAddr).arg(networkNum).arg(tr("备电故障")));
        }
        else
        {
            m_standbyPowerStateHash[QString("%1,%2,%3").arg(extNum).arg(networkNum).arg(powerAddr)]=tr("故障");
            QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//备电故障
            Controller::instance()->getSpeechObj()->insertAlarmText(tr("备电故障"));
            m_sqliteManager->executeQuery(QString("insert into AlarmInfo (分机号,回路号,地址号,电源地址,网络号,事件类型,状态,时间,备注) values ('%1','%2','%3','%4','%5','%6','%7','%8','%9') ").arg(extNum).arg(loopNum).arg(addrNum).arg(powerAddr).arg(networkNum).arg(tr("备电故障")).arg(tr("备电故障")).arg(curTime).arg(reMark));
            m_sqliteManager->executeQuery(QString("update AlarmInfo set 状态='%1' where 状态 != '%1' and 分机号= '%2' and 回路号='%3' and 地址号='%4' and 电源地址='%5' and 网络号= '%6'  and 事件类型 ='%7' ").arg(tr("正常")).arg(extNum).arg(loopNum).arg(addrNum).arg(powerAddr).arg(networkNum).arg(tr("备电正常")));
            QMetaObject::invokeMethod(m_standbyPowerObj,"addStandbyPowerState",Q_ARG(QVariant,extNum),Q_ARG(QVariant,networkNum),Q_ARG(QVariant,powerAddr),Q_ARG(QVariant,tr("备电故障")));

        }
    }


    //alarmDataOnTable();

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
    //m_indicatorProtocol = new IndicatorDataProtocol;
    m_ftpManager = new FtpManager();
    qmlRegisterType<QmlTableModel>("qmlTableModel",1,0,"QmlTableModel");
    qmlRegisterType<QmlForJson>("qmlForJson",1,0,"QmlForJson");
    qmlRegisterType<ItemIconInfoToJson>("itemIconInfoToJson",1,0,"ItemIconInfoToJson");
    QVBoxLayout *globalVLayout = new QVBoxLayout;
    QQuickView *toolBarQuickView = new QQuickView;
    toolBarQuickView->setSource(QUrl("qrc:/qml/toolBar/ToolBarWindow.qml"));
    toolBarQuickView->rootContext()->setContextProperty("CrtWidget",this);
    m_toolBarObj = toolBarQuickView->rootObject();
    toolBarQuickView->setMinimumWidth(1200);
    // toolBarQuickView->setWidth(this->width());
    m_toolBarContainer = QWidget::createWindowContainer(toolBarQuickView, this);
    m_toolBarContainer->setMinimumHeight(60);
    m_toolBarContainer->setMaximumHeight(60);
    m_toolBarContainer->setMinimumWidth(1200);
    // m_toolBarContainer->setStyleSheet("QWidget{border: 1px solid black}");
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

    m_controlCenterHeartbeatTimer = new CustomTimer;
    m_mainHeartBeatTimer = new CustomTimer;
    m_updateTimer = new CustomTimer;
    m_setVisibleTimer = new CustomTimer;
    m_checkExtNumTimer = new CustomTimer;
    m_checkSendDataTimer = new CustomTimer;
    m_checkSendDataTimer->setSingleShot(true);
    m_infoTableView = new InfoTableView(this);
    m_infoTableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_sqliteManager = new SqliteManager;
    m_infoTableView->setMaximumHeight(170);
    // m_showDataTimer->setInterval(20);

    m_setVisibleTimer->setSingleShot(true);
    m_alarmQuickView = new QQuickView;
    m_alarmQuickView->setSource(QUrl("qrc:/qml/alarmItem/AlarmItem.qml"));
    m_alarmQuickView->rootContext()->setContextProperty("CrtWidget",this);
    m_alarmQuickView->rootContext()->setContextProperty("ArchitePlanView",m_architePlanView);
    m_alarmObj = m_alarmQuickView->rootObject();

    m_alarmContainer = QWidget::createWindowContainer(m_alarmQuickView, this) ;
    m_alarmContainer->setMinimumHeight(100);
    m_alarmContainer->setMinimumWidth(154);
    m_alarmContainer->setMaximumWidth(154);
    m_alarmContainer->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);

    m_loginQuickView = new QQuickView;
    m_loginQuickView->setSource(QUrl("qrc:/qml/logInWindow/LoginWindow.qml"));
    m_loginQuickView->setGeometry(500,50,m_loginQuickView->width(),m_loginQuickView->height());
    m_loginQuickView->rootContext()->setContextProperty("CrtWidget",this);
    m_loginQuickView->setTitle(tr("用户登陆界面"));
    m_loginObj = m_loginQuickView->rootObject();

    m_loginQuickView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
    m_closeLoginView = new QQuickView;
    m_closeLoginView->setSource(QUrl("qrc:/qml/logInWindow/LoginWindow.qml"));
    m_closeLoginView->rootContext()->setContextProperty("CrtWidget",this);
    m_closeLoginView->setGeometry(500,50,m_closeLoginView->width(),m_closeLoginView->height());
    m_closeLoginView->setTitle(tr("用户登陆之后关闭"));
    m_closeLoginObj= m_closeLoginView->rootObject();
    m_closeLoginView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
    m_settingView = new QQuickView;
    m_settingView->setSource(QUrl("qrc:/qml/infoSetting/SettingView.qml"));
    m_settingView->setGeometry(300,50,m_settingView->width(),m_settingView->height());
    m_settingView->setTitle(tr("信息设置界面"));
    m_settingObj = m_settingView->rootObject();

    m_settingView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
    m_resetLoginQuickView = new QQuickView;
    m_resetLoginQuickView->setSource(QUrl("qrc:/qml/logInWindow/ResetLoginItem.qml"));
    m_resetLoginQuickView->setTitle(tr("复位权限登陆"));
    m_resetLoginQuickView->setMinimumSize(QSize(320,180));
    m_resetLoginQuickView->setMaximumSize(QSize(320,180));
    m_resetLoginQuickView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);

    m_resetLoginObj = m_resetLoginQuickView->rootObject();




    m_infoQueryView = new QQuickView;
    m_infoQueryView->setSource(QUrl("qrc:/qml/infoSetting/InfoQuery.qml"));
    m_infoQueryView->setGeometry(300,50,m_infoQueryView->width(),m_infoQueryView->height());
    m_infoQueryView->setTitle(tr("信息查询界面"));
    m_infoQueryView->setResizeMode(QQuickView::SizeRootObjectToView);
    m_infoQueryView->setFlags(Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint);
    m_infoQueryObj = m_infoQueryView->rootObject();

    m_extNumStateView = new QQuickView;
    m_extNumStateView->setSource(QUrl("qrc:/qml/deviceOnline/DeviceOnline.qml"));
    m_extNumStateView->setTitle(tr("主机在线状态"));
    m_extNumObj = m_extNumStateView->rootObject();
    m_extNumStateView->setGeometry(300,120,660,640);


    m_extNumStateView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);

    m_mainPowerView = new QQuickView;
    m_mainPowerView->setSource(QUrl("qrc:/qml/alarmItem/MainPowerItem.qml"));
    m_mainPowerView->setTitle(tr("主电状态"));
    m_mainPowerView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
    m_mainPowerObj = m_mainPowerView->rootObject();
    m_mainPowerView->setGeometry(300,300,240,300);
    m_standbyPowerView= new QQuickView;
    m_standbyPowerView->setSource(QUrl("qrc:/qml/alarmItem/StandbyPowerItem.qml"));
    m_standbyPowerView->setTitle(tr("备电状态"));
    m_standbyPowerView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
    m_standbyPowerObj = m_standbyPowerView->rootObject();
    m_standbyPowerView->setGeometry(550,300,240,300);
    m_handOrAutoView= new QQuickView;
    m_handOrAutoView->setSource(QUrl("qrc:/qml/alarmItem/HandOrAutoStateItem.qml"));
    m_handOrAutoView->setTitle(tr("手自动状态"));
    m_handOrAutoView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
    m_handOrAutoObj=m_handOrAutoView->rootObject();
    m_handOrAutoView->setGeometry(800,300,240,300);
    m_deviceOnlineSettingView = new QQuickView;
    m_deviceOnlineSettingView->setSource(QUrl("qrc:/qml/deviceOnline/DeviceOnlineSetting.qml"));
    m_deviceOnlineSettingView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);

    m_deviceOnleSettingObj = m_deviceOnlineSettingView->rootObject();
    // m_translator = new QTranslator;
    // m_translator->load("D:/program/GraphicsDisplay/en_EN.qm");

    QHBoxLayout *globalHLayout = new QHBoxLayout;
    globalHLayout->addWidget(m_alarmContainer);
    QSplitter *splitter = new QSplitter(Qt::Vertical,this);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(m_architePlanView);
    splitter->addWidget(m_infoTableView);
    //splitter->addWidget(m_curEventLabel);

    // splitter->setStretchFactor(splitter->indexOf(m_architePlanView), 80);
    // splitter->setStretchFactor(splitter->indexOf(m_infoTableView), 20);
    globalHLayout->addWidget(splitter);
    globalVLayout->addLayout(globalHLayout);
    globalVLayout->setSpacing(0);
    globalVLayout->setContentsMargins(QMargins(0,0,0,0));
    setLayout(globalVLayout);

    m_selectAlarmInfoList <<"网络号"<< "分机号"<<"回路号"<<"地址号"<<"电源地址"<< "设备"
                         <<"事件类型"<<"时间"<<"系统"<< "建筑名称"<<"楼层"<<"位置"<<"备注";
    m_updateTimer->start(1000);


    connect(Controller::instance()->getSpeechObj(),&SpeechObj::languageChangeToEnglish,this,[=](bool isEnglish)
    {
        Q_UNUSED(isEnglish);
        QMetaObject::invokeMethod(m_settingObj,"initSpeechSetting");
    });


    connect(m_checkExtNumTimer,&CustomTimer::timeout,this,[=]()
    {
        QMutex mutex;
        QMutexLocker lock(&mutex);
#ifdef Q_OS_LINUX
        sigsetjmp(sigEnv,1);
#endif

        foreach(const QString &valueStr,m_extNumTimesHash.keys())
        {
            int extNumTimes =m_extNumTimesHash.value(valueStr);
            m_extNumTimesHash[valueStr]=extNumTimes+1;
            bool curState = m_extNumStateHash.value(valueStr);
            // QStringList curValueList = valueStr.split(",");
            int networkNum=0;
            int curExtNum=0;
            curExtNum = valueStr.section(",",0,0).toInt();
            networkNum = valueStr.section(",",1,1).toInt();
            QVariant curNetworkNum ;
            QMetaObject::invokeMethod(m_extNumObj,"networkNum",Q_RETURN_ARG(QVariant,curNetworkNum));
            // qDebug() << valueStr<< curState << m_extNumTimesHash;
            if(!curState)
            {
                if(m_extNumTimesHash.value(valueStr)>=c_mainHeartBeatTimeOut/1000)
                {
                    m_extNumTimesHash[valueStr]=0;
                    m_extAndNetworkStateHash[valueStr] = false;
                    if(m_extOnlineStateHash.value(valueStr)!=Disconnected)
                    {
                        insertOtherAlarmInfo(tr("主机通信故障"),QString::number(curExtNum),QString::number(networkNum));
                        m_extOnlineStateHash[valueStr]=Disconnected;
                    }

                    if(m_extNumObj!=nullptr)
                    {
                        if(curNetworkNum.toString()==QString::number(networkNum))
                        {
                            QMetaObject::invokeMethod(m_extNumObj,"setIndicatorState",Q_ARG(QVariant,curExtNum),Q_ARG(QVariant,"yellow"));
                        }
                    }
                }
            }
            else
            {
                m_extNumStateHash[valueStr] = false;
                m_extNumTimesHash[valueStr]=0;
                m_extAndNetworkStateHash[valueStr] = true;
                if(m_extOnlineStateHash.value(valueStr)!=Connected)
                {
                    m_extOnlineStateHash[valueStr]=Connected;

                    if(m_extNumObj!=nullptr)
                    {
                        if(curNetworkNum.toString()==QString::number(networkNum))
                        {
                            QMetaObject::invokeMethod(m_extNumObj,"setIndicatorState",Q_ARG(QVariant,curExtNum),Q_ARG(QVariant,"green"));
                        }
                    }

                    updateOtherAlarmInfo(tr("主机通信故障"),tr("主机通信故障恢复"),QString::number(curExtNum),QString::number(networkNum));
                }
            }
        }
#ifdef Q_OS_LINUX
        signal(SIGSEGV, segFaultReceive);
#endif

    });
    // setMySqlInfo();
    Q_ASSERT(m_alarmObj);
    connect(m_alarmObj,SIGNAL(clearVoice()),this,SLOT(clearVoice()));
    connect(m_alarmObj,SIGNAL(mainPowerViewShow()),this,SLOT(showMainPowerView()));
    connect(m_alarmObj,SIGNAL(standbyPowerViewShow()),this,SLOT(showStandbyPowerView()));
    connect(m_alarmObj,SIGNAL(handOrAutoStateViewShow()),this,SLOT(showHandOrAutoView()));
    connect(m_alarmObj,SIGNAL(showOnlineState()),this,SLOT(showOnlineView()));

    Q_ASSERT(m_resetLoginObj);
    connect(m_resetLoginObj,SIGNAL(curLoginState(bool)),this,SLOT(resetLoginState(bool)));
    Q_ASSERT(m_settingObj);
    connect(m_settingObj,SIGNAL(controlCenterChecked(bool)),this,SLOT(setControlCenterEnable(bool)));
    connect(m_settingObj,SIGNAL(heartbeatClose()),this,SLOT(closeControlCenterHeartbeat()));
    Q_ASSERT(m_loginObj);
    connect(m_loginObj,SIGNAL(closeView()),this,SLOT(logWidgetClose()));
    Q_ASSERT(m_closeLoginObj);
    connect(m_closeLoginObj,SIGNAL(startClose()),this,SLOT(closeAll()));
    connect(m_closeLoginObj,SIGNAL(closeView()),this,SLOT(closeLogInView()));
    Q_ASSERT(m_toolBarObj);

    connect(m_toolBarObj,SIGNAL(checkTheVersion()),this,SLOT(checkCurrentVersion()));

    connect(m_toolBarObj,SIGNAL(chAndEnSwitch()),this,SLOT(chAndEnSwitch()));
    Q_ASSERT(m_extNumObj);
    connect(m_extNumObj,SIGNAL(editIndicatorState(QString)),this,SLOT(setDeviceOnlineState(QString)));
    //connect(m_extNumObj,SIGNAL(shieldIndicatorState(QString,QString,bool)),this,SLOT(enableExtNum(QString,QString,bool)));

    connect(m_extNumObj,SIGNAL(onlineStateSetting()),this,SLOT(showDeviceOnlineSetting()));

    Q_ASSERT(m_deviceOnleSettingObj);
    connect(m_deviceOnleSettingObj,SIGNAL(checkedAll(int,bool)),this,SLOT(updateAllOnlineState(int,bool)));
    connect(m_deviceOnleSettingObj,SIGNAL(checkedOne(int,int,bool)),this,SLOT(updateOneOnlineState(int,int,bool)));
    connect(m_deviceOnleSettingObj,SIGNAL(networkChanged(int)),this,SLOT(networkChangedOnlineState(int)));
    Q_ASSERT(m_infoQueryObj);
    connect(m_infoQueryObj,SIGNAL(setCurrentAlarm(QString)),this,SLOT(alarmChanged(QString)));
}

void CrtWidget::alarmDataOnTable()
{


    m_infoTableView->tableModel()->sqlCommit(QString("select %1 from AlarmInfo where 状态!='%2' order by 时间 desc limit 10000").arg(m_selectAlarmInfoList.join(",")).arg(tr("正常")));
    if(m_curLanguage==English)
    {
        if(m_firstEnter)
        {
            m_infoTableView->setTableHeader();
            m_firstEnter = false;
        }

    }

}

void CrtWidget::showMainPowerView()
{
    m_mainPowerView->close();
    m_mainPowerView->show();
}

void CrtWidget::showStandbyPowerView()
{
    m_standbyPowerView->close();
    m_standbyPowerView->show();
}

void CrtWidget::showHandOrAutoView()
{
    m_handOrAutoView->close();
    m_handOrAutoView->show();
}

void CrtWidget::checkCurrentVersion()
{
    QList<QByteArray>arrayList;
    QByteArray curArray;
    curArray.resize(3);
    curArray[0] = 0x01;
    curArray[1] = 0x03;
    curArray[2] = 0xcc;
    arrayList.push_back(curArray.mid(0,1));
    arrayList.push_back(curArray.mid(1,1));
    arrayList.push_back(curArray.mid(2,1));
    QByteArray sendArray= m_serialDataProtocol->dataArray(arrayList);
    // qDebug() << "sendArray.toHex()"<<sendArray.toHex();
    //serialDataProcessing(sendArray);
    Controller::instance()->getCommObj()->sendData(sendArray);
}

void CrtWidget::updateTime()
{
    QMetaObject::invokeMethod(m_toolBarObj,"setTime");
}

void CrtWidget::enableExtNum(QString extNum,QString networkNum,bool state)
{
    QString curExtNetwork=  QString("%1,%2").arg(extNum).arg(networkNum);
    m_extEnableHash[curExtNetwork]=state;
    m_extAndNetworkStateHash.remove(curExtNetwork);

}



void CrtWidget::updateHandOrAutoState()
{
    QString curHandOrAutoState =tr("自动");
    foreach (const QString&curState, m_handOrAutoStateHash.values())
    {
        if(curState!=tr("默认"))
        {
            curHandOrAutoState = curState;
        }
    }
    if(curHandOrAutoState==tr("自动"))
    {
        QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
    }
    else if(curHandOrAutoState==tr("手动"))
    {
        QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"blue"));
    }
    else if(curHandOrAutoState==tr("默认"))
    {
        QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"purple"));
    }

    QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,curHandOrAutoState));
}

void CrtWidget::updateAllOnlineState(int network, bool state)
{
    // qDebug() << "state"<<state;

    QVariant curNetworkValue;
    QMetaObject::invokeMethod(m_extNumObj,"networkNum",Q_RETURN_ARG(QVariant,curNetworkValue));
    if(network==curNetworkValue.toInt())
    {
        for(int i=0;i<256;i++)
        {
            QMetaObject::invokeMethod(m_extNumObj,"setVisible",Q_ARG(QVariant,i),Q_ARG(QVariant,state));

        }
    }

    if(!state)
    {
        m_extEnableHash.clear();
        m_extAndNetworkStateHash.clear();
        for(int i=0;i<256;i++)
        {
            QString valueStr = QString("%1,%2").arg(i).arg(network);
            if(m_extNumTimesHash.contains(valueStr))
            {
                m_extNumTimesHash.remove(valueStr);
            }
        }

    }
    else
    {
        for(int i=0;i<256;i++)
        {
            emit checkExtNum(quint8(i),quint8(network));
            enableExtNum(QString::number(i),QString::number(network),true);
        }
    }


    m_allCheckedHash[network]=state;
}

void CrtWidget::updateOneOnlineState(int network, int index,bool state)
{

    QVariant curNetworkValue;
    QMetaObject::invokeMethod(m_extNumObj,"networkNum",Q_RETURN_ARG(QVariant,curNetworkValue));
    if(network==curNetworkValue.toInt())
    {
        QMetaObject::invokeMethod(m_extNumObj,"setVisible",Q_ARG(QVariant,index),Q_ARG(QVariant,state));

    }

    if(state)
    {
        emit checkExtNum(quint8(index),quint8(network));
    }
    else
    {
        QString valueStr = QString("%1,%2").arg(index).arg(network);
        //m_extNumTimesHash[valueStr]=0;
        if(m_extNumTimesHash.contains(valueStr))
        {
            m_extNumTimesHash.remove(valueStr);
        }
    }
    enableExtNum(QString::number(index),QString::number(network),state);
}

void CrtWidget::networkChangedOnlineState(int network)
{
    QMetaObject::invokeMethod(m_deviceOnleSettingObj,"clearState");
    QList<QString>extAndNetworkList=  m_extEnableHash.keys();
    foreach(QString value,extAndNetworkList)
    {
        QList<QString>valueList= value.split(",");
        if(valueList.size()>1)
        {
            QString curExtValue=  valueList.at(0);
            QString curNetworkValue=  valueList.at(1);
            if(curNetworkValue.toInt() ==network)
            {
                QMetaObject::invokeMethod(m_deviceOnleSettingObj,"setChecked",Q_ARG(QVariant,curExtValue.toInt()),Q_ARG(QVariant,m_extEnableHash.value(value)));
            }
        }
    }

    if(m_allCheckedHash.contains(network))
    {
        QMetaObject::invokeMethod(m_deviceOnleSettingObj,"setAllRadioBtn",Q_ARG(QVariant,m_allCheckedHash.value(network)));
    }
    else
    {
        QMetaObject::invokeMethod(m_deviceOnleSettingObj,"setAllRadioBtn",Q_ARG(QVariant,false));
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

#elif defined Q_OS_LINUX
    startProcess(QString("sh -c \"echo rpdzkj|sudo -S date -s '%1'&&sudo hwclock --systohc\"").arg(dateTime.toString("yyyy-MM-dd hh:mm:ss")));
    isSuccess = true;
#endif
    return isSuccess;
}

void CrtWidget::sendFireInfo(quint8 extNum, quint8 loopNum, quint8 addrNum,const QString &dateTimeStr)
{
    if(m_sendEnable)
    {
        m_monitoringPackageNum++;
        m_monitoringPackageNum=m_monitoringPackageNum%10000;
        m_sendDataResult =false;
        QList<QByteArray> valueList;
        valueList.push_back(QString("%1").arg(44,4,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(m_monitoringPackageNum,4,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QByteArray("000"));
        QString extNumStr=QString("%1").arg(extNum,4,10,QChar('0'));
        valueList.push_back(extNumStr.toLocal8Bit());
        valueList.push_back(QString("%1").arg(loopNum,3,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(addrNum,4,10,QChar('0')).toLocal8Bit());

        valueList.push_back(dateTimeStr.toLocal8Bit());
        Controller::instance()->getTcpObj()->sendData(m_monitoringProtocol->dataArray(valueList));
        QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,true));
        //setTransportState();
    }

}



void CrtWidget::serialDataProcessing(const QByteArray &array)
{

    QMutex mutex;
    QMutexLocker lock(&mutex);
#ifdef Q_OS_LINUX
    sigsetjmp(sigEnv,1);
#endif
    m_serialConnected = true;
    emit getSerialData();

    quint8 eventNum =  m_serialDataProtocol->dataByte(array,0);//事件
    quint8 loopNum =  m_serialDataProtocol->dataByte(array,1);//回路
    quint8 addrNum =  m_serialDataProtocol->dataByte(array,2);//地址
    quint8 type = m_serialDataProtocol->dataByte(array,3);
    quint8 curExtNum = (type>>2)&0x3f;
    quint8 networkNum=0;
    quint8 minuteValue = m_serialDataProtocol->dataByte(array,8);
    quint8 minute= minuteValue&0x7f;
    quint8 date = m_serialDataProtocol->dataByte(array,6);
    QString extNum = QString("%1").arg((type>>2)&0x3f);
    quint8 month= m_serialDataProtocol->dataByte(array,5);
    quint8 second= m_serialDataProtocol->dataByte(array,9);

    quint8 year= m_serialDataProtocol->dataByte(array,4);
    //    quint8 month= m_serialDataProtocol->dataByte(array,5);
    //    quint8 date = m_serialDataProtocol->dataByte(array,6);
    //    // quint8 hour = m_serialDataProtocol->dataByte(array,7);
    //    quint8 minute= minuteValue&0x7f;


    if(eventNum==0x37)
    {
        curExtNum = type;
    }
    if((minuteValue&0x80)!=0)
    {
        networkNum = minute;
    }
    QString extAndNetwork;


    if(eventNum!=0xbb&&eventNum!=0x35&&eventNum!=0xcc&&eventNum!=0x23&&eventNum!=0x31&&eventNum!=0xdd&&eventNum!=0xde&&eventNum!=0x20)
    {
        extAndNetwork = QString("%1,%2").arg(curExtNum).arg(networkNum);
        if(m_extEnableHash.contains(extAndNetwork)&&m_extEnableHash.value(extAndNetwork)==true)
        {
            m_extNumStateHash[extAndNetwork] = true;
            m_extAndNetworkStateHash[extAndNetwork]=true;
            // emit checkExtNum(curExtNum,networkNum);

            processViewsData(array);
            if(eventNum!=0x99&&eventNum!=0xaa && eventNum!=0x12 && eventNum!=0x13 && eventNum!=0x32)
            {
                allAlarmStatistics();
            }


        }

    }
    else if(eventNum==0x35)
    {
        extAndNetwork = QString("%1,%2").arg(date).arg(month);
        if(m_extEnableHash.contains(extAndNetwork)&&m_extEnableHash.value(extAndNetwork)==true)
        {
            m_extNumStateHash[extAndNetwork] = true;
            m_extAndNetworkStateHash[extAndNetwork]=true;
            processViewsData(array);
            allAlarmStatistics();
            // emit checkExtNum(date,month);
        }

    }

    //    else if(eventNum==0xdd)//重传应答
    //    {

    //    }
    else if(eventNum==0xde)//单机重传应答
    {
        if(m_extEnableHash.contains(extAndNetwork)&&m_extEnableHash.value(extAndNetwork)==true)
        {
            m_architePlanView->clearAlarmFromExtNum(extNum);
        }

    }

    else if(eventNum== 0x20)
    {
        if((loopNum|addrNum)==0x00)
        {
            if(m_extEnableHash.contains(extAndNetwork)&&m_extEnableHash.value(extAndNetwork)==true)
            {
                insertOtherAlarmInfo(tr("本机复位"),QString("%1").arg(extNum));
                // Controller::instance()->getOperatorInfo()->insertEvent(tr("本机复位"));
                m_architePlanView->clearAlarmFromExtNum(extNum);
            }

        }
        else if((loopNum&addrNum)==0x01)
        {
            insertOtherAlarmInfo(tr("网络复位"),QString("%1").arg(extNum));
            //Controller::instance()->getOperatorInfo()->insertEvent(tr("网络复位"));
            m_architePlanView->clearExcptFireAlarm();

        }
    }

    else if(eventNum==0xcc)
    {
        QMetaObject::invokeMethod(m_toolBarObj,"setProgramVer",Q_ARG(QVariant,tr("通讯板程序版本:")+"V"+QString(array.mid(1,4)).insert(3,'.')));
        if(!m_setVisibleTimer->isActive())
        {
            m_setVisibleTimer->start(10000);
        }

    }
    else if(eventNum==0xbb)//模拟量
    {
        int curChannel=0;
        double analogValue=0.0;
        QString analogType;
        if(second==0xa0)
        {
            curChannel = (year>>5)&0x07;
            quint16 curData =((date<<8)&0xff00)+month;
            if(curData&0x8000)
            {
                analogType+=tr("交流");
            }
            else
            {
                analogType+=tr("直流");
            }
            if(curData&0x4000)
            {
                analogType+=tr("电压(V)");
            }
            else
            {
                analogType+=tr("电流(mA)");
            }

            analogValue = (curData&0x3fff)*1.0;
            if(analogType.startsWith(tr("直流电流")))
            {
                analogValue = analogValue*10.0;
            }
            else if(analogType.startsWith(tr("直流电压")))
            {
                analogValue = analogValue*0.1;
            }

        }
        else
        {
            switch ((type&0x03))
            {
            case 1:
            {
                analogType = tr("剩余电流(mA)");
                analogValue = (year*256+second)*1.0;
            }
                break;
            case 2:
            {
                analogType = tr("温度(℃)");
                analogValue = (year*256+second)*1.0;
            }
                break;
            default:
                break;
            }

        }
        m_isAnalog = true;
        m_sqliteManager->processAnalogData(true);
        m_sqliteManager->executeQuery(QString("insert into AnalogInfo values('%1','%2','%3','%4','%5','%6','%7','%8')").arg(extNum).arg(loopNum).arg(addrNum).arg(networkNum).arg(curChannel).arg(analogType).arg(analogValue).arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));

    }



#ifdef Q_OS_LINUX
    signal(SIGSEGV, segFaultReceive);
#endif


}

void CrtWidget::tcpDataProcessing(const QByteArray &arrayValue)
{


    QMutex mutex;
    QMutexLocker lock(&mutex);
#ifdef Q_OS_LINUX
    sigsetjmp(sigEnv,1);
#endif
    if(m_sendEnable)
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
        if(dataArrayList.size()<=0)
        {
#ifdef Q_OS_LINUX
            signal(SIGSEGV, segFaultReceive);
#endif
            return;
        }
        m_tcpIsConnected = true;
        emit getTcpData();

        foreach (QByteArray dataArray, dataArrayList)
        {
            int dataSize = dataArray.size();
            QByteArray indexArray=m_monitoringProtocol->dataBytes(dataArray,4,7);
            int indexNum = indexArray.toInt();
            if(dataSize==8)
            {
                //控制中心故障、手自动、主备电信息接收成功
                m_sendDataResult = true;
            }
            else if(dataSize==14)
            {
                QList<QByteArray> sendArrayList;
                sendArrayList.push_back(QString("%1").arg(16,4,10,QChar('0')).toLocal8Bit());
                sendArrayList.push_back(indexArray);
                Controller::instance()->getTcpObj()->sendData(m_monitoringProtocol->dataArray(sendArrayList));
                QByteArray typeArray=   dataArray.right(3);
                QString infoPath=  Controller::instance()->getTransportInfo()->transportInfo(typeInfoHash.value(typeArray.toInt()));
                if(!infoPath.isEmpty())
                {
                    m_ftpManager->uploadFile(infoPath);
                    QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,true));
                }

            }
            else if(dataSize==11)
            {
                if(m_heartbeatIndex == indexNum)
                {
                    m_heartbeatIndex++;
                    m_tcpIsConnected = true;
                    //Controller::instance()->delayMs(5000);
                    // sendControlCenterHeartbeat();
                }
            }
        }
    }
#ifdef Q_OS_LINUX
    signal(SIGSEGV, segFaultReceive);
#endif

}

void CrtWidget::openHelpFile()
{
    QUrl url =   QUrl(QCoreApplication::applicationDirPath()+"/help/help.html");
    if(!url.isEmpty())
    {
        QDesktopServices::openUrl(url);
    }
}

void CrtWidget::sendAnalogCommand(quint8 networkNum,quint8 extNum,quint8 loopNum,quint8 addrNum,quint8 channelNum,quint8 analogType)
{
    //QHash<quint8,QString>analogTypeHash;
    //    analogTypeHash[tr("无")]=0;
    //    analogTypeHash[tr("剩余电流(mA)")]=1;
    //    analogTypeHash[tr("温度(℃)")]=2;

    //    analogTypeHash[tr("交流电流(mA)")] = 0;
    //    analogTypeHash[tr("直流电流(mA)")]=1;
    //    analogTypeHash[tr("交流电压(V)")] =2 ;
    //    analogTypeHash[tr("直流电压(V)")] = 3;

    //    analogTypeHash[0] = tr("无"), analogTypeHash[1] = tr("高度(m)");
    //    analogTypeHash[2] = tr("温度(℃)"),analogTypeHash[3] = tr("压力(Mpa)");
    //    analogTypeHash[4] = tr("压力(Kpa)"),analogTypeHash[5] = tr("气体浓度(%LEL)");
    //    analogTypeHash[6] = tr("气体浓度(PPM)"),analogTypeHash[7] = tr("气体浓度(%V/V)");
    //    analogTypeHash[8] = tr("气体浓度(KPPM)"),analogTypeHash[9] = tr("气体浓度(Mg/m^3)");
    //    analogTypeHash[10] = tr("时间(s)"),analogTypeHash[11] = tr("电压(V)");
    //    analogTypeHash[12] = tr("电流(A)"),analogTypeHash[13] = tr("流量(L/s)");
    //    analogTypeHash[14] = tr("风量(m^3/min)"), analogTypeHash[15] = tr("风速(m/s)");
    //    analogTypeHash[16] = tr("剩余电流(mA)"),analogTypeHash[17]=tr("烟参量");
    //    analogTypeHash[18] = tr("距离(m)"), analogTypeHash[24] = tr("交流电流(mA)");
    //    analogTypeHash[25] = tr("直流电流(mA)"),analogTypeHash[26] = tr("交流电压(V)");
    //    analogTypeHash[27] = tr("直流电压(V)");
    //qDebug() << analogTypeHash.value(analogType);

    quint8 curChannel= ((channelNum<<5)&0xe0)|(analogType&0x1f);
    // qDebug() << "analogTypeHash.key(analogType)" << analogTypeHash.key(analogType);
    QByteArray packageArray;
    QList<QByteArray>dataArrayList;
    if(networkNum==0)
    {
        packageArray.resize(7);
        packageArray[0] = 1;
        packageArray[1] = 0x07;
        packageArray[2] = 0xbb;
        packageArray[3] = extNum;
        packageArray[4] = loopNum;
        packageArray[5] = addrNum;
        packageArray[6] = curChannel;

        dataArrayList.push_back(packageArray.mid(0,1));
        dataArrayList.push_back(packageArray.mid(1,1));
        dataArrayList.push_back(packageArray.mid(2,1));
        dataArrayList.push_back(packageArray.mid(3,1));
        dataArrayList.push_back(packageArray.mid(4,1));
        dataArrayList.push_back(packageArray.mid(5,1));
        dataArrayList.push_back(packageArray.mid(6,1));
    }
    else {

        packageArray.resize(8);
        packageArray[0] = 1;
        packageArray[1] = 0x08;
        packageArray[2] = 0xbc;
        packageArray[3] = networkNum;
        packageArray[4] = extNum;
        packageArray[5] = loopNum;
        packageArray[6] = addrNum;
        packageArray[7] = curChannel;

        dataArrayList.push_back(packageArray.mid(0,1));
        dataArrayList.push_back(packageArray.mid(1,1));
        dataArrayList.push_back(packageArray.mid(2,1));
        dataArrayList.push_back(packageArray.mid(3,1));
        dataArrayList.push_back(packageArray.mid(4,1));
        dataArrayList.push_back(packageArray.mid(5,1));
        dataArrayList.push_back(packageArray.mid(6,1));
        dataArrayList.push_back(packageArray.mid(7,1));


    }

    QByteArray sendDataArray= m_serialDataProtocol->dataArray(dataArrayList);
    qDebug() << sendDataArray.toHex();
    Controller::instance()->getCommObj()->sendData(sendDataArray);
}

void CrtWidget::startReset()
{
    m_checkSendDataTimer->stop();
    m_serialConnected = false;
    resetAllState();
    clearCheckExtInfo();
    insertOtherAlarmInfo(tr("复位"),"","");
    Controller::instance()->getOperatorInfo()->insertEvent(tr("复位"));
    reSendCmd();

}

void CrtWidget::clearVoice()
{
    insertOtherAlarmInfo(tr("消音"),"","");
    Controller::instance()->getSpeechObj()->stopSpeech();
    Controller::instance()->getOperatorInfo()->insertEvent(tr("消音"));
    Controller::instance()->getSpeechObj()->clearAlarmText();
}

void CrtWidget::setIndicatorState(bool isOk)
{
    Controller::instance()->delayMs(1000);
    setIndicator(isOk);
}

void CrtWidget::resetLoginState(bool isOk)
{
    if(isOk)
    {
        startReset();
        m_resetLoginQuickView->close();
    }
    else
    {
        m_resetLoginQuickView->close();
    }
}

void CrtWidget::sendSeralData()
{

    //    QByteArray array;
    //    array.resize(10);
    //    array[0] = 0x04;
    //    array[1] = 0x00;

    //    array[2] =0x02;//地址
    //    array[3] =0x05;//属性
    //    array[4] =0x20;//年
    //    array[5] =0xa6;//月
    //    array[6] =0x11;//日
    //    array[7] =0x00;//时
    //    array[8] =0x00;//分
    //    array[9] =0x10;//秒
    //    processViewsData(array);

    m_architePlanView->clearAlarmFromExtNum("0");

}

void CrtWidget::chAndEnSwitch()
{
    QHash<QString,QVariant> fileHash;
    if(m_curLanguage==Chinese)
    {
        m_curLanguage = English;
        setIsEnglish(true);
        fileHash["language"]="english";
        if(m_translator.isEmpty())
        {
            m_translatorToEnglish= m_translator.load(QCoreApplication::applicationDirPath()+"/en_EN.qm");
        }
        if(m_translatorToEnglish)
        {

            if(qApp->installTranslator(&m_translator))
            {
                retranslate();
            }
        }

    }
    else
    {
        m_curLanguage = Chinese;
        setIsEnglish(false);
        fileHash["language"]="chinese";
        if(qApp->removeTranslator(&m_translator))
        {
            retranslate();
        }
    }
    QFile langueFile(QCoreApplication::applicationDirPath()+"/language.json");
    bool fileIsOpen = langueFile.open(QIODevice::WriteOnly);
    if(fileIsOpen)
    {
        langueFile.write(QJsonDocument::fromVariant(fileHash).toJson());
    }
    langueFile.close();
}

void CrtWidget::allAlarmStatistics()
{
    Q_ASSERT(m_alarmObj);
    foreach(const QString &type,m_typeList)
    {
        int typeNum = m_architePlanView->numOfTypeAlarm(type);

        if(type.endsWith(tr("火警"),Qt::CaseInsensitive))
        {
            if(typeNum>0)
            {
                QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            }

            QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmText",Q_ARG(QVariant,typeNum));
        }
        else if(type.endsWith(tr("启动"),Qt::CaseInsensitive))
        {
            if(typeNum>0)
            {

                QMetaObject::invokeMethod(m_alarmObj,"setLinkageAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            }

            QMetaObject::invokeMethod(m_alarmObj,"setLinkageText",Q_ARG(QVariant,typeNum));
        }
        else if(type.endsWith(tr("监管"),Qt::CaseInsensitive))
        {

            if(typeNum>0)
            {
                QMetaObject::invokeMethod(m_alarmObj,"setSuperviseAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            }
            else
            {
                QMetaObject::invokeMethod(m_alarmObj,"setSuperviseAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
            }
            QMetaObject::invokeMethod(m_alarmObj,"setSuperviseText",Q_ARG(QVariant,typeNum));
        }
        else if(type.endsWith(tr("故障"),Qt::CaseInsensitive))
        {
            if(typeNum>0)
            {

                QMetaObject::invokeMethod(m_alarmObj,"setfaultAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));
            }
            else
            {
                QMetaObject::invokeMethod(m_alarmObj,"setfaultAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
            }
            QMetaObject::invokeMethod(m_alarmObj,"setFaultText",Q_ARG(QVariant,typeNum));
        }
        else if(type.endsWith(tr("反馈"),Qt::CaseInsensitive))
        {
            if(typeNum>0)
            {

                QMetaObject::invokeMethod(m_alarmObj,"setRespondColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            }
            else
            {
                QMetaObject::invokeMethod(m_alarmObj,"setRespondColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
            }
            QMetaObject::invokeMethod(m_alarmObj,"setRespondText",Q_ARG(QVariant,typeNum));
        }
        else if(type.endsWith(tr("屏蔽"),Qt::CaseInsensitive))
        {
            if(typeNum>0)
            {
                QMetaObject::invokeMethod(m_alarmObj,"setShieldAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"orange"));//橘黄色
            }
            else
            {
                QMetaObject::invokeMethod(m_alarmObj,"setShieldAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
            }
            QMetaObject::invokeMethod(m_alarmObj,"setShieldText",Q_ARG(QVariant,typeNum));
        }
    }
}



void CrtWidget::closeSys()
{
    if(Controller::instance()->getUserRight()!=UserManager::Super)//为了调试方便
    {
#ifdef Q_OS_WIN
        // startProcess("shutdown -s -t 0");
#elif defined Q_OS_LINUX
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 0 0\"");
        startProcess("poweroff");
#endif
    }
    else
    {
#ifdef Q_OS_LINUX
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 0 0\"");
#endif
    }
}



void CrtWidget::reSendCmd()
{

    QByteArray sendArray;
    sendArray.resize(5);
    sendArray[0]=1;
    sendArray[1]=0x05;
    sendArray[2]=0x22;
    sendArray[3]=0x80;
    sendArray[4]=0xff;
    QList<QByteArray> sendArrayList;
    sendArrayList.push_back(sendArray.mid(0,1));
    sendArrayList.push_back(sendArray.mid(1,1));
    sendArrayList.push_back(sendArray.mid(2,1));
    sendArrayList.push_back(sendArray.mid(3,1));
    sendArrayList.push_back(sendArray.mid(4,1));

    //qDebug() << packageNum;
    QByteArray curArray = m_serialDataProtocol->dataArray(sendArrayList);
    Controller::instance()->getCommObj()->sendData(curArray);
}

void CrtWidget::reSendUnicastCmd(quint8 extNum, quint8 networkNum)
{
    // static quint8 packageNum=0;
    QByteArray sendArray;
    sendArray.resize(5);
    sendArray[0]=1;
    sendArray[1]=0x05;
    sendArray[2]=0x23;
    sendArray[3]=networkNum;
    sendArray[4]=extNum;
    QList<QByteArray> sendArrayList;
    sendArrayList.push_back(sendArray.mid(0,1));
    sendArrayList.push_back(sendArray.mid(1,1));
    sendArrayList.push_back(sendArray.mid(2,1));
    sendArrayList.push_back(sendArray.mid(3,1));
    sendArrayList.push_back(sendArray.mid(4,1));
    //    packageNum++;
    //    packageNum=packageNum%256;
    //qDebug() << packageNum;
    QByteArray curArray = m_serialDataProtocol->dataArray(sendArrayList);
    Controller::instance()->getCommObj()->sendData(curArray);

    //qDebug() <<"curArray" <<curArray.toHex();
}

void CrtWidget::startProcess(const QString &cmd)
{
    m_process.execute(cmd);
}

void CrtWidget::resetAllState()
{
    m_architePlanView->clearAlarm(true);
    m_architePlanView->stopCheckAlarmTimer();
#ifdef Q_OS_LINUX
    startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 1 0\"");
    startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 2 0\"");
#endif


    Controller::instance()->getSpeechObj()->startSpeech();

}

void CrtWidget::sendControlCenterHeartbeat()
{
    QList<QByteArray>dataArrayList;
    dataArrayList.push_back(QString("%1").arg(19,4,10,QChar('0')).toLocal8Bit());
    dataArrayList.push_back(QString("%1").arg(m_heartbeatIndex,4,10,QChar('0')).toLocal8Bit());
    dataArrayList.push_back(QString("%1").arg(999).toLocal8Bit());
    QByteArray sendArray=  m_monitoringProtocol->dataArray(dataArrayList);
    Controller::instance()->getTcpObj()->writeOneFramData(sendArray);
    //    QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//中心通信
    //    Controller::instance()->getSpeechObj()->removeAlarmText(tr("中心通信故障"));
}




void CrtWidget::sendAlarmInfo(quint8 sysNum, quint8 alarmType, quint8 alarmState, quint8 runState, const QString &timeStr)
{
    if(m_sendEnable)
    {
        m_monitoringPackageNum++;
        m_monitoringPackageNum=m_monitoringPackageNum%10000;
        m_sendDataResult = false;
        QList<QByteArray> valueList;
        valueList.push_back(QString("%1").arg(44,4,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(m_monitoringPackageNum,4,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QByteArray("001"));
        valueList.push_back(QString("%1").arg(sysNum,3,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(alarmType,3,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(alarmState,3,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(runState,2,10,QChar('0')).toLocal8Bit());
        QString curDateTime= QDateTime::fromString(timeStr,"yyyy/MM/dd hh:mm:ss").toString("yyyyMMddhhmmss");
        valueList.push_back(curDateTime.toLocal8Bit());
        QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,true));
        Controller::instance()->getTcpObj()->sendData(m_monitoringProtocol->dataArray(valueList));
        //setTransportState();

    }

}

void CrtWidget::sendPowerState(quint8 sysNum, quint8 mainPowerState, quint8 reservationState, quint8 runState, const QString &timeStr)
{
    if(m_sendEnable)
    {
        m_monitoringPackageNum++;
        m_monitoringPackageNum=m_monitoringPackageNum%10000;
        m_sendDataResult = false;
        QList<QByteArray> valueList;
        valueList.push_back(QString("%1").arg(42,4,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(m_monitoringPackageNum,4,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QByteArray("002"));
        valueList.push_back(QString("%1").arg(sysNum,3,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(mainPowerState,2,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(reservationState,2,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(runState,2,10,QChar('0')).toLocal8Bit());
        QString curDateTime= QDateTime::fromString(timeStr,"yyyy/MM/dd hh:mm:ss").toString("yyyyMMddhhmmss");
        valueList.push_back(curDateTime.toLocal8Bit());
        QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,true));
        Controller::instance()->getTcpObj()->sendData(m_monitoringProtocol->dataArray(valueList));
        //setTransportState();
    }

}

void CrtWidget::sendHandOrAutoState(quint8 sysNum, quint8 handOrAutoState, quint8 runState, const QString &timeStr)
{
    if(m_sendEnable)
    {
        m_monitoringPackageNum++;
        m_monitoringPackageNum=m_monitoringPackageNum%10000;
        m_sendDataResult = false;
        QList<QByteArray> valueList;
        valueList.push_back(QString("%1").arg(40,4,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(m_monitoringPackageNum,4,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QByteArray("003"));
        valueList.push_back(QString("%1").arg(sysNum,3,10,QChar('0')).toLocal8Bit());
        valueList.push_back(QString("%1").arg(handOrAutoState,2,10,QChar('0')).toLocal8Bit());

        valueList.push_back(QString("%1").arg(runState,2,10,QChar('0')).toLocal8Bit());
        QString curDateTime= QDateTime::fromString(timeStr,"yyyy/MM/dd hh:mm:ss").toString("yyyyMMddhhmmss");
        valueList.push_back(curDateTime.toLocal8Bit());
        QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,true));
        Controller::instance()->getTcpObj()->sendData(m_monitoringProtocol->dataArray(valueList));
        //setTransportState();
    }

}

void CrtWidget::setTransportState()
{
    if(!m_checkSendDataTimer->isActive())
    {
        m_checkSendDataTimer->start(1000);
    }
}

void CrtWidget::setIndicator(bool state)
{
    QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,false));
    if(state)
    {
#ifdef Q_OS_LINUX
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 1 0\"");
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 2 1\"");
#endif
        Controller::instance()->getSpeechObj()->removeAlarmText(tr("传输故障"));
        QMetaObject::invokeMethod(m_alarmObj,"setTransformColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
        updateOtherAlarmInfo(tr("传输故障"),tr("传输故障恢复"));
    }
    else
    {
#ifdef Q_OS_LINUX
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 2 0\"");
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 1 1\"");
#endif
        QMetaObject::invokeMethod(m_alarmObj,"setTransformColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));
        Controller::instance()->getSpeechObj()->insertAlarmText(tr("传输故障"));
        insertOtherAlarmInfo(tr("传输故障"));

    }
}

void CrtWidget::closeAllOnlineState()
{
    QList<QString>extNumKeyList=m_extNumStateHash.keys();
    foreach (QString curValue, extNumKeyList)
    {
        m_extNumStateHash[curValue] = false;
        m_extNumTimesHash[curValue] = 0;
        m_extOnlineStateHash[curValue]=NoState;
        QStringList strList=  curValue.split(",");
        QString networkNum,extNum;
        if(strList.size()>1)
        {
            extNum =strList.at(0);
            networkNum = strList.at(1);
        }
        if(m_extNumObj!=nullptr)
        {
            QMetaObject::invokeMethod(m_extNumObj,"resetIndicatorState");
            QVariant networkNumValue;
            QMetaObject::invokeMethod(m_extNumObj,"networkNum",Q_RETURN_ARG(QVariant,networkNumValue));
            if(networkNumValue.toString()==networkNum)
            {
                QMetaObject::invokeMethod(m_extNumObj,"setIndicatorState",Q_ARG(QVariant,extNum.toInt()),Q_ARG(QVariant,"yellow"));
            }

        }

    }
    //    if(!m_extNumStateView->isActive())
    //    {
    //        m_extNumStateView->show();
    //    }
}

void CrtWidget::insertOtherAlarmInfo(const QString &info, const QString &extNum, const QString &networkNum)
{
    QString curTime=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    m_sqliteManager->executeQuery(QString("insert into AlarmInfo (分机号,网络号,事件类型,状态,时间) values ('%1','%2','%3','%4','%5') ").arg(extNum).arg(networkNum).arg(info).arg(info).arg(curTime));
    //alarmDataOnTable();
}

void CrtWidget::updateOtherAlarmInfo(const QString &alarmType,const QString &curState,const QString &extNum,const QString &networkNum)
{
    QString curTime=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");

    m_sqliteManager->executeQuery(QString("update AlarmInfo set 状态='%1' where 状态 != '%1' and 分机号= '%2' and 网络号= '%3' and 事件类型 like '%4%'").arg(tr("正常")).arg(extNum).arg(networkNum).arg(alarmType));
    m_sqliteManager->executeQuery(QString("insert into AlarmInfo (分机号,网络号,事件类型,状态,时间) values ('%1','%2','%3','%4','%5') ").arg(extNum).arg(networkNum).arg(curState).arg(curState).arg(curTime));

    //alarmDataOnTable();
}

void CrtWidget::clearCheckExtInfo()
{

    m_extNumStateHash.clear();
    m_extAndNetworkStateHash.clear();
    //m_extNumTimesHash.clear();
    m_extOnlineStateHash.clear();
}

void CrtWidget::startTranslate()
{
    QFile langueFile(QCoreApplication::applicationDirPath()+"/language.json");

    // qDebug() << QCoreApplication::applicationDirPath();
    if(langueFile.exists())
    {
        if(langueFile.open(QIODevice::ReadOnly))
        {
            QVariant value = QJsonDocument::fromJson(langueFile.readAll()).toVariant();
            QHash<QString,QVariant> valueHash= value.toHash();
            if(valueHash.value("language").toString()=="english")
            {
                m_curLanguage = English;
                setIsEnglish(true);
                if(m_translator.isEmpty())
                {
                    m_translatorToEnglish= m_translator.load(QCoreApplication::applicationDirPath()+"/en_EN.qm");
                }
                if(m_translatorToEnglish)
                {
                    if(qApp->installTranslator(&m_translator))
                    {
                        retranslate();
                    }

                }
            }
            else
            {
                m_curLanguage = Chinese;
                setIsEnglish(false);
            }
            langueFile.close();

        }
    }
}

void CrtWidget::setItemIconInfo()
{
    m_architePlanView->itemIconSetting();
    QMetaObject::invokeMethod(m_settingObj,"setSysArchiteInfo");

}

void CrtWidget::saveOnlineInfoToJson()
{
    QHash<QString,QVariant>valueHash;
    QHash<QString,QStringList >extAndNetworkHash;
    foreach(const QString &key,m_extEnableHash.keys())
    {
        QList<QString> list  = key.split(",");

        QString extNum,networkNum;
        if(list.size()>1)
        {
            extNum = list.at(0);
            networkNum = list.at(1);
        }

        if(m_allCheckedHash.contains(networkNum.toInt())&&m_allCheckedHash.value(networkNum.toInt())==true)
        {
            valueHash[networkNum] = "all";
        }
        else
        {
            if(!extAndNetworkHash.value(networkNum).contains(extNum))
            {
                extAndNetworkHash[networkNum].push_back(extNum);
            }
        }


    }

    foreach(const QString &key,extAndNetworkHash.keys())
    {
        QStringList strList = extAndNetworkHash.value(key);

        valueHash[key] = strList.join(",");
    }
    //qDebug() << valueHash;

    QmlForJson qmlForJson;
    qmlForJson.writeFile(valueHash,QApplication::applicationDirPath()+"/deviceOnline.json");
}

void CrtWidget::processViewsData(const QByteArray &array)
{
    //m_faultStateHash[17] = tr("回路故障");
    QString networkNum="0";
    //int packageNum=0;
    QString timeStr;
    quint8 powerStateNum=0;
    quint8 eventNum =  m_serialDataProtocol->dataByte(array,0);//事件
    quint8 loopNum =  m_serialDataProtocol->dataByte(array,1);//回路
    quint8 addrNum =  m_serialDataProtocol->dataByte(array,2);//地址
    quint8 type = m_serialDataProtocol->dataByte(array,3);
    quint8 minuteValue = m_serialDataProtocol->dataByte(array,8);
    quint8 year= m_serialDataProtocol->dataByte(array,4);
    quint8 month= m_serialDataProtocol->dataByte(array,5);
    quint8 date = m_serialDataProtocol->dataByte(array,6);
    // quint8 hour = m_serialDataProtocol->dataByte(array,7);
    quint8 minute= minuteValue&0x7f;
    quint8 second= m_serialDataProtocol->dataByte(array,9);

    QString extNum = QString("%1").arg((type>>2)&0x3f);

    int bcd_year = m_serialDataProtocol->dataBytes(array,4,4).toHex().toInt();
    // int bcd_month = m_serialDataProtocol->dataBytes(array,5,5).toHex().toInt();
    int bcd_date = m_serialDataProtocol->dataBytes(array,6,6).toHex().toInt();
    int bcd_hour = m_serialDataProtocol->dataBytes(array,7,7).toHex().toInt();
    int bcd_minute = m_serialDataProtocol->dataBytes(array,8,8).toHex().toInt();
    int bcd_second = m_serialDataProtocol->dataBytes(array,9,9).toHex().toInt();

    quint8 curMonthValue =m_serialDataProtocol->dataBytes(array,5,5).toHex().toUInt(nullptr,16)&0xff;
    QByteArray monthValueArray;
    monthValueArray.resize(1);
    monthValueArray[0] = curMonthValue&0x1f;
    int bcd_month= monthValueArray.toHex().toInt();
    //报警主机
    if((minuteValue&0x80)==0)
    {
        if(second<=0x60)
        {
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
    quint8 sysNum = m_sysNameHash.key(sysName);
    switch (eventNum)
    {
    case 0x99://心跳
        m_serialConnected = true;
        break;
    case 0x01:
    case 0x03:
    case 0x05:
    case 0x0a:
    case 0x0b:
    case 0x0d:
    {
        QString currentState = m_alarmTypeHash.value(eventNum);
        quint8 alarmState =1;
        quint8 sysNum = m_sysNameHash.key(sysName);


        if(eventNum==0x03)
        {
            if((month&0xe0)>0)
            {
                quint8 fireDoor=  ((month&0xe0)>>5)&0x0f;
                alarmState=fireDoor;
                QString fireProofDoor ="";

                switch (fireDoor)
                {
                case 0x03:
                    fireProofDoor=tr("设备类型:常闭防火门");
                    break;
                case 0x04:
                    fireProofDoor=tr("设备类型:常开防火门");
                    break;
                case 0x05:
                    fireProofDoor=tr("设备类型:输入模块");
                    break;
                default:
                    break;
                }
                switch (type&0x03)
                {
                case 0:
                {
                    fireProofDoor += tr(",事件:门故障");
                    alarmState+=0xb0;
                }
                    break;
                case 1:
                {
                    fireProofDoor += tr(",事件:模块故障");
                    alarmState+=0xc0;
                }
                    break;
                case 2:
                {
                    fireProofDoor += tr(",事件:线路故障");
                    alarmState+=0xd0;
                }
                    break;
                default:
                    break;
                }
                currentState = fireProofDoor;
            }


        }
        else if(eventNum==0x0d)
        {
            if((type&0x03)==0x01)
            {
                currentState = tr("常开门关闭");
                alarmState=6;
            }
        }
        else if(eventNum==0x01)
        {
            QDateTime sendDateTime =QDateTime::fromString(timeStr,"yyyy/MM/dd hh:mm:ss");
            QString dateTimeStr= sendDateTime.toString("yyyyMMddhhmmss");
            sendFireInfo(extNum.toShort()&0xff,loopNum,addrNum,dateTimeStr);
        }


        if(second==0xa0)
        {
            if(m_faultStateHash.keys().contains(month))
            {
                if(year==0xa0)
                {
                    currentState = tr("模块");
                }
                else if(year>=0xa1&&year<=0xa3)
                {
                    currentState = tr("通道");
                }
                currentState = currentState+m_faultStateHash.value(month);
                alarmState = year+month+3;

            }

        }

        sendAlarmInfo(sysNum,eventNum,alarmState,0,timeStr);
        if(item!=nullptr)
        {

            m_architePlanView->createAlarm(item,m_alarmTypeHash.value(eventNum),currentState,timeStr);

        }
        else
        {
            QString curType = m_alarmTypeHash.value(eventNum);
            noItemInfoSetting(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,curType,currentState,timeStr,currentState);

            if(!Controller::instance()->getDataStore()->containsFireAlarm(tr("火警"))&&Controller::instance()->getDataStore()->containsFireAlarmNoItem(tr("火警")))
            {
                if(!m_architePlanView->firstAlarmViewIsVisible())
                {
                    m_architePlanView->startCheckAlarmTimer(500);
                    m_architePlanView->getFirstFireAlarmWidget()->setFirstFireInfo(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,timeStr);
                    m_architePlanView->getFirstFireAlarmWidget()->updateGeometry();
                    m_architePlanView->getFirstFireAlarmWidget()->show();

                }
            }



        }

        QString analogType;
        quint16 analogValue=0;
        if(eventNum==0x0a)
        {
            switch((type&0x03))
            {
            case 0:
                break;
            case 1:
            case 2:
            {
                if((type&0x03)==1)
                {
                    analogType = tr("电流(mA)");
                }
                else if((type&0x03)==2)
                {
                    analogType = tr("温度(℃)");
                }
                analogValue = year*256+second;
                m_sqliteManager->executeQuery(QString("insert into AnalogInfo values('%1','%2','%3','%4','%5','%6','%7','%8')").arg(extNum).arg(QString::number(loopNum)).arg(QString::number(addrNum)).arg(networkNum).arg(0).arg(analogType).arg(analogValue).arg(timeStr));
            }
                break;
            default:
                break;

            }
        }
    }
        break;
    case 0x02:
    case 0x04:
    case 0x06:
    case 0x0c:
    {

        QString eliminateAlarmType,currentState;
        currentState = m_alarmTypeHash.value(eventNum);
        if(eventNum==0x04)
        {

            if((month&0xe0)>0)
            {
                quint8 fireDoor=  (month&0xe0)>>5;
                QString fireProofDoorState ="";
                switch (fireDoor)
                {
                case 0x03:
                    fireProofDoorState=tr("设备类型:常闭防火门");
                    break;
                case 0x04:
                    fireProofDoorState=tr("设备类型:常开防火门");
                    break;
                case 0x05:
                    fireProofDoorState=tr("设备类型:输入模块");
                    break;
                default:
                    break;
                }
                switch ((type&0x03))
                {
                case 0:
                    fireProofDoorState += tr(",事件:门故障恢复");
                    break;
                case 1:
                    fireProofDoorState += tr(",事件:模块故障恢复");
                    break;
                case 2:
                    fireProofDoorState += tr(",事件:线路故障恢复");
                    break;
                default:

                    break;
                }
                currentState = fireProofDoorState;
            }


            eliminateAlarmType=tr("故障");

        }
        else if(eventNum==0x02)
        {
            eliminateAlarmType = tr("反馈");
            if((type&0x03)==0x01)
            {
                currentState =tr("常开门打开");
            }
        }
        else if(eventNum==0x06)
        {
            eliminateAlarmType=tr("启动");
        }
        else if(eventNum==0x0c)
        {
            eliminateAlarmType=tr("屏蔽");
        }

        if(second==0xa0)
        {

            if(m_faultStateHash.keys().contains(month))
            {
                if(year==0xa0)
                {
                    currentState = tr("模块");
                }
                else if(year>=0xa1&&year<=0xa3)
                {
                    currentState = tr("通道");
                }

                currentState = currentState+m_faultStateHash.value(month);
                //alarmState = year+month+3;

            }

        }

        if(item!=nullptr)
        {
            item->setAlarmState(eliminateAlarmType,currentState);
            m_architePlanView->eliminateAlarm(item,eliminateAlarmType,timeStr);
            sendAlarmInfo(sysNum,eventNum,0,0,timeStr);
        }
        else
        {
            sendAlarmInfo(0,eventNum,0,0,timeStr);
            noItemUpdate(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,eliminateAlarmType,m_alarmTypeHash.value(eventNum),currentState,timeStr,currentState);

        }
    }
        break;

    case 0x12:
    case 0x13:
    {
        QString powerState = "";

        if(loopNum==0)
        {
            communicationStatus(m_commuStatusHash.value(eventNum),true,extNum,networkNum);
            powerState = tr("正常");
            powerStateNum =0;
        }
        else if(loopNum==1)
        {
            communicationStatus(m_commuStatusHash.value(eventNum),false,extNum,networkNum);
            powerState = tr("故障");
            powerStateNum =1;

        }

        if(eventNum== 0x12)
        {
            sendPowerState(sysNum,powerStateNum,0,0,timeStr);
        }
        else
        {
            sendPowerState(sysNum,0,powerStateNum,0,timeStr);
        }

    }
        break;

    case 0x32:
    {
        QString handOrAutoState = tr("自动");
        quint8 handOrAutoStateNum =2;
        switch (type)
        {
        case 0:
        {
            handOrAutoState = tr("默认");
            Controller::instance()->getOperatorInfo()->insertEvent(m_commuStatusHash.value(eventNum),handOrAutoState);

            handOrAutoStateNum=0;
            //QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"purple"));
            //QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
        }
            break;
        case 1:
        {
            handOrAutoState = tr("手动");
            Controller::instance()->getOperatorInfo()->insertEvent(m_commuStatusHash.value(eventNum),handOrAutoState);

            handOrAutoStateNum=1;
            //QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"blue"));
            // QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
        }
            break;
        case 2:
        {
            handOrAutoState = tr("自动");
            Controller::instance()->getOperatorInfo()->insertEvent(m_commuStatusHash.value(eventNum),handOrAutoState);

            handOrAutoStateNum =2;
            //QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
            // QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
        }
            break;
        default:
        {
            switch (loopNum)
            {

            case 0:
            {
                handOrAutoState = tr("默认");
                Controller::instance()->getOperatorInfo()->insertEvent(m_commuStatusHash.value(eventNum),handOrAutoState);
                handOrAutoStateNum=0;
                //QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"purple"));
                //QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
            }
                break;
            case 1:
            {
                handOrAutoState = tr("手动");
                Controller::instance()->getOperatorInfo()->insertEvent(m_commuStatusHash.value(eventNum),handOrAutoState);
                handOrAutoStateNum=1;
                // QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"blue"));
                // QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
            }
                break;
            case 2:
            {
                handOrAutoState = tr("自动");
                Controller::instance()->getOperatorInfo()->insertEvent(m_commuStatusHash.value(eventNum),handOrAutoState);
                handOrAutoStateNum =2;
                //                QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
                //                QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
            }
                break;
            default:
                break;
            }
        }
            break;

        }

        quint8 sysNum = m_sysNameHash.key(sysName);
        sendHandOrAutoState(sysNum,handOrAutoStateNum,networkNum.toUShort()&0xff,timeStr);

        insertOtherAlarmInfo(handOrAutoState,extNum,networkNum);
        QMetaObject::invokeMethod(m_handOrAutoObj,"addHandOrAutoState",Q_ARG(QVariant,extNum),Q_ARG(QVariant,networkNum),Q_ARG(QVariant,handOrAutoState));
        m_handOrAutoStateHash[QString("%1,%2").arg(extNum).arg(networkNum)]= handOrAutoState;
        updateHandOrAutoState();

    }
        break;
    case 0x35:
    {
        QString currentTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        GraphicsItem*currentItem  = m_architePlanView->itemFormInfo(QString::number(date),QString::number(addrNum),QString::number(month),"0");
        QString curAlarmState=QString::number(type*256+year)+tr("米")+tr("位置");
        if(currentItem!=nullptr)
        {
            curAlarmState =currentItem->deviceLocation()+curAlarmState;
        }

        switch (loopNum) {
        case 0x01:
        {

            if(currentItem!=nullptr)
            {
                m_architePlanView->createAlarm(currentItem,tr("火警"),tr("光纤火警")+curAlarmState);
                QString str= currentItem->sysOfDevice();
                quint8 alarmSysNum= m_sysNameHash.key(str);
                sendAlarmInfo(alarmSysNum,1,2,0,currentTime);//光纤火警

            }
            else
            {
                noItemInfoSetting(QString::number(date),QString::number(addrNum),QString::number(month),"0",tr("火警"),tr("光纤火警"),currentTime,tr("光纤火警")+curAlarmState);
                sendAlarmInfo(0,1,2,0,currentTime);//光纤火警
                if(!Controller::instance()->getDataStore()->containsFireAlarm(tr("火警"))&&Controller::instance()->getDataStore()->containsFireAlarmNoItem(tr("火警")))
                {
                    if(!m_architePlanView->firstAlarmViewIsVisible())
                    {
                        m_architePlanView->getFirstFireAlarmWidget()->setFirstFireInfo(QString::number(date),QString::number(addrNum),QString::number(month),"0",currentTime);
                        m_architePlanView->getFirstFireAlarmWidget()->show();
                    }
                }
            }
        }
            break;
        case 0x02:
        {
            if(currentItem!=nullptr)
            {
                // currentItem->deviceLocation() = QString::number(type*256+year)+tr("米");
                m_architePlanView->createAlarm(currentItem,tr("故障"),tr("光纤故障")+curAlarmState);
                sendAlarmInfo(sysNum,3,3,0,currentTime);
            }
            else
            {
                noItemInfoSetting(QString::number(date),QString::number(addrNum),QString::number(month),"0",tr("故障"),tr("光纤故障"),currentTime,tr("光纤故障")+curAlarmState);
                sendAlarmInfo(0,3,3,0,currentTime);

            }
        }
            break;
        case 0x03:
        {
            sendAlarmInfo(sysNum,4,0,0,currentTime);
            if(currentItem!=nullptr)
            {
                m_architePlanView->eliminateAlarm(QString::number(date),QString::number(addrNum),QString::number(month),"0",tr("故障"));

            }
            else
            {

                noItemUpdate(QString::number(date),QString::number(addrNum),QString::number(month),"0",tr("故障"),tr("故障恢复"),tr("光纤故障恢复"),currentTime);
            }

        }
            break;
        case 0x04:
        {
            if(addrNum==0)
            {
                communicationStatus(tr("光纤主电"),true,QString::number(date));
            }
            else if(addrNum==1)
            {
                communicationStatus(tr("光纤主电"),false,QString::number(date));
            }
        }
            break;
        case 0x05:
        {
            if(addrNum==0)
            {
                communicationStatus(tr("光纤备电"),true,QString::number(date));
            }
            else if(addrNum==1)
            {
                communicationStatus(tr("光纤备电"),false,QString::number(date));
            }
        }
            break;
        default:
            break;
        }
    }
        break;
    case 0x37:
    {




        QString emergencyExtNum,emergencyLoopNum,emergencyAddrNum,emergencyPowerAddr;
        QString emergencyAlarmType="";
        QString emergencyAlarmReply="";
        emergencyExtNum = QString::number(type);
        emergencyLoopNum = QString::number(loopNum);
        emergencyAddrNum = QString::number(addrNum);
        emergencyPowerAddr = QString::number(year);
        GraphicsItem*curGraphicsItem =m_architePlanView->itemFormInfo(emergencyExtNum,emergencyLoopNum,emergencyAddrNum,networkNum,emergencyPowerAddr);
        QString curAlarmTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        switch (month)
        {

        case 0x03://主电故障
            // communicationStatus(tr("主电"),true,emergencyExtNum,networkNum);
            emergencyPowerState(tr("主电"),false,emergencyExtNum,emergencyLoopNum,emergencyAddrNum,emergencyPowerAddr,networkNum,m_emergencyDeviceTypeHash.value(date)+tr("主电故障"));

            break;
        case 0x83://主电故障恢复
            emergencyPowerState(tr("主电"),true,emergencyExtNum,emergencyLoopNum,emergencyAddrNum,emergencyPowerAddr,networkNum,m_emergencyDeviceTypeHash.value(date)+tr("主电正常"));
            //communicationStatus(tr("主电"),false,emergencyExtNum,networkNum);

            break;
        case 0x06://备电故障
            emergencyPowerState(tr("备电"),false,emergencyExtNum,emergencyLoopNum,emergencyAddrNum,emergencyPowerAddr,networkNum,m_emergencyDeviceTypeHash.value(date)+tr("备电故障"));
            //communicationStatus(tr("备电"),true,emergencyExtNum,networkNum);
            //insertOtherAlarmInfo(tr("备电")+tr("故障"),emergencyExtNum);

            break;
        case 0x86://备电故障恢复
            emergencyPowerState(tr("备电"),true,emergencyExtNum,emergencyLoopNum,emergencyAddrNum,emergencyPowerAddr,networkNum,m_emergencyDeviceTypeHash.value(date)+tr("备电正常"));
            //communicationStatus(tr("备电"),false,emergencyExtNum,networkNum);

            break;
        case 0x09://故障
        case 0x0b:
        case 0x0c:
        case 0x19://屏蔽
        case 0x14://应急启动
        case 0x9a://设备丢失（也算故障）
        {
            quint8 emergencyAlarmValue=0;
            if(month==0x19)
            {
                emergencyAlarmType = tr("屏蔽");
                emergencyAlarmValue=0x0b;
            }
            else if(month==0x14)
            {
                emergencyAlarmType = tr("启动");
                emergencyAlarmValue=0x05;
            }
            else
            {
                emergencyAlarmType= tr("故障");
                emergencyAlarmValue=0x03;
            }

            sendAlarmInfo(15,emergencyAlarmValue,month,0,curAlarmTime);

            if(curGraphicsItem!=nullptr)
            {
                m_architePlanView->createAlarm(curGraphicsItem,emergencyAlarmType,m_emergencyDeviceTypeHash.value(date)+ m_emergencyStateHash.value(month),curAlarmTime);
            }
            else
            {
                noItemInfoSetting(emergencyExtNum,emergencyLoopNum,emergencyAddrNum,networkNum,emergencyAlarmType,m_emergencyStateHash.value(month),curAlarmTime,m_emergencyDeviceTypeHash.value(date)+ m_emergencyStateHash.value(month),emergencyPowerAddr);
            }
        }
            break;
        case 0x89:
        case 0x8b:
        case 0x8c://故障恢复
        case 0x99://屏蔽解除
        case 0x94://应急解除
        {
            quint8 emergencyAlarmValue =0;
            if(month==0x99)
            {
                emergencyAlarmReply = tr("屏蔽");
                emergencyAlarmValue=0x0c;
            }
            else if(month==0x94)
            {
                emergencyAlarmReply = tr("启动");
                emergencyAlarmValue=0x06;
            }
            else
            {
                emergencyAlarmReply = tr("故障");
                emergencyAlarmValue=0x04;
            }
            sendAlarmInfo(15,emergencyAlarmValue,month,0,curAlarmTime);

            if(curGraphicsItem!=nullptr)
            {
                m_architePlanView->eliminateAlarm(curGraphicsItem,emergencyAlarmReply);

            }
            else
            {
                noItemUpdate(emergencyExtNum,emergencyLoopNum,emergencyAddrNum,networkNum,emergencyAlarmReply,m_emergencyStateHash.value(month),m_emergencyStateHash.value(month),curAlarmTime,m_emergencyDeviceTypeHash.value(date)+ m_emergencyStateHash.value(month),emergencyPowerAddr);
            }
        }
            break;
        case 0x13://输出过载
        case 0x1a://新注册
        case 0x1b://开机
        case 0x1c://消音
        case 0x1f://重码
        case 0x20://输出开路
        case 0x21://进入月检
        case 0x22://进入年检
        case 0x29://灯具改变方向
            // case 0x9a://设备丢失
        {
            if(curGraphicsItem!=nullptr)
            {
                m_sqliteManager->executeQuery(m_alarmSqlInfo.arg(emergencyExtNum).arg(emergencyLoopNum).arg(emergencyAddrNum).arg(networkNum).arg(emergencyPowerAddr).arg(curGraphicsItem->deviceNum())
                                              .arg(curGraphicsItem->equipmentModel()).arg(m_emergencyTypeHash.value(month)).arg(m_emergencyTypeHash.value(month)).arg(curAlarmTime).arg(curGraphicsItem->sysOfDevice()).arg(curGraphicsItem->buildingName()).arg(curGraphicsItem->floorOfDevice()).arg(curGraphicsItem->deviceLocation())
                                              .arg(curGraphicsItem->manufacturers()).arg(curGraphicsItem->periodOfValidity()).arg(curGraphicsItem->deviceOperator()).arg(m_emergencyDeviceTypeHash.value(date)+m_emergencyTypeHash.value(month)));
            }
            else
            {

                m_sqliteManager->executeQuery(QString("insert into AlarmInfo (分机号,回路号,地址号,网络号,电源地址,事件类型,状态,设备,时间,备注) values ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10')").arg(emergencyExtNum).arg(emergencyLoopNum).arg(emergencyAddrNum).arg(networkNum).arg(emergencyPowerAddr)
                                              .arg(m_emergencyTypeHash.value(month)).arg(m_emergencyTypeHash.value(month)).arg(tr("未定义设备")).arg(curAlarmTime).arg(m_emergencyDeviceTypeHash.value(date)+ m_emergencyStateHash.value(month)));
            }

            //alarmDataOnTable();
        }
            break;

        case 0x93://输出过载恢复
        case 0x9b://关机
        case 0x9c://消音取消
        case 0x9f://重码恢复
        case 0xa0://开路恢复
        case 0xa1://结束月检
        case 0xa2://结束年检
        {
            if(curGraphicsItem!=nullptr)
            {
                m_sqliteManager->executeQuery(m_alarmSqlInfo.arg(emergencyExtNum).arg(emergencyLoopNum).arg(emergencyAddrNum).arg(networkNum).arg(emergencyPowerAddr).arg(curGraphicsItem->deviceNum())
                                              .arg(curGraphicsItem->equipmentModel()).arg(m_emergencyTypeHash.value(month)).arg(m_emergencyTypeHash.value(month)).arg(curAlarmTime).arg(curGraphicsItem->sysOfDevice()).arg(curGraphicsItem->buildingName()).arg(curGraphicsItem->floorOfDevice()).arg(curGraphicsItem->deviceLocation())
                                              .arg(curGraphicsItem->manufacturers()).arg(curGraphicsItem->periodOfValidity()).arg(curGraphicsItem->deviceOperator()).arg(m_emergencyDeviceTypeHash.value(date)+ m_emergencyTypeHash.value(month)));
            }
            else
            {
                m_sqliteManager->executeQuery(QString("insert into AlarmInfo (分机号,回路号,地址号,网络号,电源地址,事件类型,状态,设备,时间,备注) values ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10')").arg(emergencyExtNum).arg(emergencyLoopNum).arg(emergencyAddrNum).arg(networkNum).arg(emergencyPowerAddr)
                                              .arg(m_emergencyTypeHash.value(month)).arg(m_emergencyTypeHash.value(month)).arg(tr("未定义设备")).arg(curAlarmTime).arg(m_emergencyDeviceTypeHash.value(date)+ m_emergencyTypeHash.value(month)));
            }

            m_sqliteManager->executeQuery(QString("update AlarmInfo set 状态 = '%1' where 分机号='%2' and 回路号='%3' and 地址号='%4' and 网络号='%5' and 电源地址='%6' and 事件类型='%7' ").arg(tr("正常")).arg(emergencyExtNum).arg(emergencyLoopNum).arg(emergencyAddrNum).arg(networkNum).arg(emergencyPowerAddr).arg(m_emergencyTypeHash.value(month-0x80)));
            //alarmDataOnTable();
        }
            break;
        case 0x1e://复位
        {
            m_architePlanView->clearAlarmFromExtNum(emergencyExtNum);
            insertOtherAlarmInfo(tr("复位"),emergencyExtNum,networkNum);
        }
            break;
        default:
            break;
        }
    }
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
        //    case 0xbb://模拟量
        //    {
        //        int curChannel=0;
        //        double analogValue=0.0;
        //        QString analogType;
        //        if(second==0xa0)
        //        {
        //            curChannel = (year>>5)&0x07;
        //            quint16 curData =((date<<8)&0xff00)+month;
        //            if(curData&0x8000)
        //            {
        //                analogType+=tr("交流");
        //            }
        //            else
        //            {
        //                analogType+=tr("直流");
        //            }
        //            if(curData&0x4000)
        //            {
        //                analogType+=tr("电压(V)");
        //            }
        //            else
        //            {
        //                analogType+=tr("电流(mA)");
        //            }

        //            analogValue = (curData&0x3fff)*1.0;
        //            if(analogType.startsWith(tr("直流电流")))
        //            {
        //                analogValue = analogValue*10.0;
        //            }
        //            else if(analogType.startsWith(tr("直流电压")))
        //            {
        //                analogValue = analogValue*0.1;
        //            }

        //        }
        //        else
        //        {
        //            switch ((type&0x03))
        //            {
        //            case 1:
        //            {
        //                analogType = tr("剩余电流(mA)");
        //                analogValue = (year*256+second)*1.0;
        //            }
        //                break;
        //            case 2:
        //            {
        //                analogType = tr("温度(℃)");
        //                analogValue = (year*256+second)*1.0;
        //            }
        //                break;
        //            default:
        //                break;
        //            }

        //        }
        //        m_isAnalog = true;
        //        qDebug() << "****************";
        //        m_sqliteManager->executeQuery(QString("insert into AnalogInfo values('%1','%2','%3','%4','%5','%6','%7','%8')").arg(extNum).arg(loopNum).arg(addrNum).arg(networkNum).arg(curChannel).arg(analogType).arg(analogValue).arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));

        //    }

        //        break;
    default:
        break;
    }



}

void CrtWidget::showOnlineView()
{
    if(m_extNumStateView!=nullptr)
    {
        QMetaObject::invokeMethod(m_extNumObj,"resetVisibleAll");
        QVariant curNetworkNum;
        QMetaObject::invokeMethod(m_extNumObj,"networkNum",Q_RETURN_ARG(QVariant,curNetworkNum));
        QList<QString>extAndNetworkList=   m_extEnableHash.keys();
        foreach (QString curValue, extAndNetworkList)
        {
            QStringList valueList = curValue.split(",");
            QString networkNum,extNum;
            if(valueList.size()>1)
            {
                extNum = valueList.at(0);
                networkNum=valueList.at(1);
            }
            if(curNetworkNum.toString()==networkNum)
            {
                QString colorName = "gray";

                if(m_extAndNetworkStateHash.contains(curValue))
                {
                    if(m_extAndNetworkStateHash.value(curValue))
                    {
                        colorName = "green";
                    }
                    else
                    {
                        colorName = "yellow";
                    }
                }

                QMetaObject::invokeMethod(m_extNumObj,"setIndicatorState",Q_ARG(QVariant,extNum.toInt()),Q_ARG(QVariant,colorName));
                QMetaObject::invokeMethod(m_extNumObj,"setVisible",Q_ARG(QVariant,extNum.toInt()),Q_ARG(QVariant,m_extEnableHash.value(curValue)));
                QMetaObject::invokeMethod(m_deviceOnleSettingObj,"setChecked",Q_ARG(QVariant,extNum.toInt()),Q_ARG(QVariant,m_extEnableHash.value(curValue)));
            }
        }

        foreach(int curNetworkValue, m_allCheckedHash.keys())
        {
            if(curNetworkNum.toString() == QString::number(curNetworkValue))
            {

                QMetaObject::invokeMethod(m_deviceOnleSettingObj,"setAllRadioBtn",Q_ARG(QVariant,true));
            }
        }
        // m_extNumStateView->close();
        m_extNumStateView->show();
    }



}


