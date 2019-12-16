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
    m_serialCurState(false)

{

    setWindowFlags(Qt::FramelessWindowHint/*|Qt::Window*/);
    QString dbName = QCoreApplication::applicationDirPath()+"/alarmInfo.db";
    m_alarmInfoDbName = dbName;

    m_sysNameHash[0] = "";
    m_sysNameHash[1]="火灾探测报警系统";
    m_sysNameHash[2]="消防联动系统";
    m_sysNameHash[3]="自动喷水灭火系统";
    m_sysNameHash[4]="水喷雾灭火系统";
    m_sysNameHash[5]="消火栓系统";
    m_sysNameHash[6]="气体灭火系统";
    m_sysNameHash[7]="泡沫灭火系统";
    m_sysNameHash[8]="干粉灭火系统";
    m_sysNameHash[9]="防烟排烟系统";
    m_sysNameHash[10]="防火及卷帘系统";
    m_sysNameHash[11]="电梯系统";
    m_sysNameHash[12]="消防电话系统";
    m_sysNameHash[13]="消防应急广播系统";
    m_sysNameHash[14]="消防应急照明和疏散指示系统";
    m_sysNameHash[15]="消防电源系统";


    initWidget();
    // m_debugLogMsg = new DebugLogMsg(nullptr);
    Controller::instance()->getOperatorInfo()->insertEvent(tr("系统开机"));
    m_sqliteManager->setDataBase("QSQLITE","alarmInfo","","","",dbName,888);
    m_sqliteManager->open();
    if(m_sqliteManager->isOpen())
    {
        // QDateTime curDateTime = QDateTime::currentDateTime();
        // curDateTime = curDateTime.addMonths(-6);
        // QString curDateTimeStr = curDateTime.toString("yyyy/MM/dd hh:mm:ss");

        QStringList tableNameList = m_sqliteManager->getTables();
        if(!tableNameList.contains("AlarmInfo"))
        {
            m_sqliteManager->executeQuery("create table AlarmInfo(分机号 text, 回路号 text,地址号 text,网络号 text,设备编码 text ,设备 text,类型 text,状态 text,报警时间 text,报警恢复时间 text,系统 text,建筑名称 text,楼层 text,位置 text,制造商 text,有效期 text,操作员 text)");
        }
        else
        {
            // m_sqliteManager->executeQuery(QString("delete from AlarmInfo where 报警时间 <='%1'").arg(curDateTimeStr));

        }

        if(!tableNameList.contains("AnalogInfo"))
        {
            m_sqliteManager->executeQuery("create table AnalogInfo(分机号 text, 回路号 text,地址号 text,网络号 text,当前通道 int, 模拟量类型 text ,结果 text,时间 text)");
        }
        else
        {
            // m_sqliteManager->executeQuery(QString("delete from AnalogInfo where 时间 <='%1'").arg(curDateTimeStr));

        }


    }


    m_infoTableView->hide();
    m_infoTableView->tableModel()->setDbDriver("QSQLITE");
    m_infoTableView->tableModel()->setDbName(dbName);
    m_infoTableView->tableModel()->setDbPort(888);
    m_infoTableView->tableModel()->setDbOpen(true);
    m_infoTableView->setAlternatingRowColors(true);
    QStringList alarmInfoList,valueList;
    alarmInfoList << "分机号"<<"回路号"<<"地址号"<<"网络号"<<"设备编码" << "设备" <<"类型"<<"状态"<<"报警时间"
                  <<"报警恢复时间"<<"系统"<< "建筑名称"<<"楼层"<<"位置"<< "制造商" << "有效期" <<"操作员";

    for(int i=0;i<alarmInfoList.size();i++)
    {
        QString str = QString("'%%1'").arg(i+1);
        valueList.push_back(str);
    }
    m_alarmSqlInfo = QString("insert into AlarmInfo (%1) values (%2)").arg(alarmInfoList.join(",")).arg(valueList.join(","));
    m_updateAlarmSqlInfo = QString("update AlarmInfo set 报警时间 ='%1' where 分机号='%2' and 回路号='%3' and 地址号='%4' and 网络号='%5' and 状态!='正常'");
    //    m_socket = dynamic_cast<QTcpSocket*>(Controller::instance()->getTcpObj()->device());
    //    if(m_socket!=nullptr)
    //    {
    //        connect(m_socket,&QTcpSocket::stateChanged,this,[=](QAbstractSocket::SocketState state)
    //        {
    //            qDebug() << state;
    //        });
    //    }


    connect(m_architePlanView,&ArchitePlanView::alarmHappend,this,&CrtWidget::alarmStatistics);
    connect(m_architePlanView,&ArchitePlanView::alarmItem,this,[=](GraphicsItem *item,const QString &alarmType)
    {
        if(item!=nullptr)
        {
            m_sqliteManager->executeQuery(m_alarmSqlInfo.arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()).arg(item->deviceNum())
                                          .arg(item->equipmentModel()).arg(alarmType).arg(item->alarmState(alarmType)).arg(item->alarmTime(alarmType))
                                          .arg(item->alarmReplyTime(alarmType)).arg(item->sysOfDevice()).arg(item->buildingName())
                                          .arg(item->floorOfDevice()).arg(item->deviceLocation()).arg(item->manufacturers()).arg(item->periodOfValidity()).arg(item->deviceOperator()));

        }

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

    connect(m_infoTableView,&InfoTableView::setAlarmNum,this,[=](int totalNum,int currentNum)
    {
        QMetaObject::invokeMethod(m_alarmObj,"setNum",Q_ARG(QVariant,totalNum),Q_ARG(QVariant,currentNum));
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
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复时间 ='%1',状态 = '正常' where 状态 != '正常'").arg(eliminateTime));
        //m_curEventLabel->clear();
        alarmDataOnTable();
    });

    connect(m_architePlanView,&ArchitePlanView::eliminateAlarmFromTable,this,[=](GraphicsItem *item,const QString &alarmState)
    {
        QString eliminateTime =  QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复时间 ='%1',状态 = '正常', 类型 ='%2' where 分机号='%3' and 回路号='%4' and 地址号='%5' and 网络号='%6' and 类型='%7' ").arg(eliminateTime).arg(alarmState+tr("消除")).arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()).arg(alarmState));
        //m_curEventLabel->setText(QString("最新事件:%1消除,分机号:%2,回路号:%3,地址号:%4,网络号:%5"));
        alarmDataOnTable();
    });

    connect(m_architePlanView,&ArchitePlanView::clearTableAlarm,this,[=]()
    {
        QString clearTime =  QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复时间 ='%1',状态 = '正常', 类型 =类型||'消除' where 状态 != '正常' and 类型 !='中心通信故障' and 类型 !='主机通信故障' and 类型 !='传输故障'").arg(clearTime));
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

    connect(Controller::instance()->getTcpObj(),&AbstractLink::errorInfo,this,[=](const QString &error)
    {
        Q_UNUSED(error);
        if(m_sendEnable)
        {
            // setIndicatorState(false);
        }

    });

    connect(m_ftpManager,&FtpManager::sendFileSuccess,this,[=](bool isOk)
    {
        if(m_sendEnable)
        {
            setIndicatorState(isOk);
            if(!isOk)
            {
                sendAlarmInfo(0,4,12,0,QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
            }

        }

    });

    connect(m_ftpManager,&FtpManager::ftpError,this,[=](const QString&error)
    {
        Q_UNUSED(error)
        if(m_sendEnable)
        {
            setIndicatorState(false);
            sendAlarmInfo(0,4,12,0,QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
        }

    });


    //    connect(m_ftpManager,&FtpManager::uploadProgress,this,[=](qint64 bytesSent, qint64 bytesTotal)
    //    {
    //        setIndicatorState(bytesSent<bytesTotal);

    //    });

    connect(m_architePlanView,&ArchitePlanView::alarmStateUpdate,this,[=]
            (const QString &extNum, const QString &alarmReplyTime)
    {
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复时间 ='%1',状态 = '正常', 类型 =类型||'消除' where 状态 != '正常' and 分机号= '%2'").arg(alarmReplyTime).arg(extNum));
        alarmDataOnTable();
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
        static quint8 num=0;
        if(m_sendEnable)
        {
            if(m_tcpIsConnected)
            {
                num=0;
                QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
                Controller::instance()->getSpeechObj()->removeAlarmText(tr("中心通信故障"));
                m_tcpIsConnected = false;
                m_tcpConnectState = true;
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
                        m_tcpConnectState = false;
                        m_checkSendDataTimer->stop();
                        m_controlCenterHeartbeatTimer->stop();
                        m_sqliteManager->executeQuery(QString("insert into AlarmInfo (类型,状态,报警时间) values ('%1','%2','%3') ").arg(tr("中心通信故障")).arg(tr("中心通信故障")).arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));

                    }

                }
                else
                {
                    sendControlCenterHeartbeat();
                }

            }
        }
    });


    connect(this,&CrtWidget::checkExtNum,this,[=](quint8 curExtNum,quint8 networkNum)
    {
        if(!m_checkExtNumHash.contains(curExtNum))
        {
            CustomTimer *curTimer = new CustomTimer;
            m_checkExtNumHash[curExtNum] = curTimer;
            curTimer->start(c_mainHeartBeatTime);
            m_extNumTimesHash[curExtNum]=0;
            m_extOnlineStateHash[curExtNum]=NoState;

            connect(curTimer,&CustomTimer::timeout,this,[=]()
            {
                bool curState = m_extNumStateHash.value(curExtNum);

                if(curState)
                {
                    m_extNumTimesHash[curExtNum]=0;
                    if(m_extNumObj!=nullptr)
                    {
                        QMetaObject::invokeMethod(m_extNumObj,"setIndicatorState",Q_ARG(QVariant,curExtNum),Q_ARG(QVariant,"green"));
                    }
                    m_extNumStateHash[curExtNum] = false;
                    if(m_extOnlineStateHash.value(curExtNum)==Disconnected)
                    {
                        m_extOnlineStateHash[curExtNum]=Connected;
                    }

                }
                else
                {
                    int extNumTimes =m_extNumTimesHash.value(curExtNum);
                    if(m_extNumTimesHash.contains(curExtNum))
                    {
                        m_extNumTimesHash[curExtNum]=extNumTimes+1;
                    }
                    if(m_extNumTimesHash.value(curExtNum)>=c_mainHeartBeatTimeOut/c_mainHeartBeatTime)
                    {
                        m_extNumTimesHash[curExtNum]=0;
                        if(m_extNumObj!=nullptr)
                        {
                            QMetaObject::invokeMethod(m_extNumObj,"setIndicatorState",Q_ARG(QVariant,curExtNum),Q_ARG(QVariant,"yellow"));
                            //m_extNumStateView->close();
                            if(!m_extNumStateView->isActive())
                            {
                                m_extNumStateView->show();
                            }
                          //   m_sqliteManager->executeQuery(QString("insert into AlarmInfo (分机号,网络号,类型,状态,报警时间) values ('%1','%2','%3','%4','%5') ").arg(QString("%1").arg(curExtNum)).arg(QString("%1").arg(networkNum)).arg(QString("%1号主机离线").arg(curExtNum)).arg("主机通信故障").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));
                            curTimer->stop();
                        }
                    }
                    else
                    {
                        if(m_extNumTimesHash.value(curExtNum)>=2)
                        {
                            m_extOnlineStateHash[curExtNum]=Disconnected;
                        }
                    }

                }
                if(m_extOnlineStateHash.value(curExtNum)==Connected)
                {
                    m_extOnlineStateHash[curExtNum]=NoState;
                    m_architePlanView->clearAlarmFromExtNum(QString("%1").arg(curExtNum),QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
                    reSendUnicastCmd(curExtNum,networkNum);
                }
            });
        }
        else
        {
            bool curState = m_extNumStateHash.value(curExtNum);
            CustomTimer *timer = m_checkExtNumHash.value(curExtNum);
            if(timer!=nullptr)
            {
                if(!timer->isActive())
                {
                    if(curState)
                    {
                        timer->start(c_mainHeartBeatTime);
                    }
                }
            }
        }
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
        QByteArray curArray = m_serialDataProtocol->dataPackage(sendArrayList);
        Controller::instance()->getCommObj()->sendData(curArray);
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
    connect(m_alarmObj,SIGNAL(reset()),this,SLOT(resetLoginViewShow()));
    connect(m_architePlanView,&ArchitePlanView::toFirstPage,this,[=]()
    {
        QMetaObject::invokeMethod(m_alarmObj,"enableToNextPageBtn",Q_ARG(QVariant,true));
        QMetaObject::invokeMethod(m_alarmObj,"enableToPreviousPageBtn",Q_ARG(QVariant,false));
        QMetaObject::invokeMethod(m_alarmObj,"setPage",Q_ARG(QVariant,m_architePlanView->totalPage()),Q_ARG(QVariant,m_architePlanView->currentPage()+1));
    });


    connect(m_architePlanView,&ArchitePlanView::findAlarmNum,this,[=](int totalNum,int currentNum)
    {
        QMetaObject::invokeMethod(m_alarmObj,"setNum",Q_ARG(QVariant,totalNum),Q_ARG(QVariant,currentNum));
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
            // QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//主机通信
            //Controller::instance()->getSpeechObj()->removeAlarmText(tr("主机通信故障"));
            m_serialConnected = true;
            if(!m_mainHeartBeatTimer->isActive())
            {
                m_mainHeartBeatTimer->start(c_mainHeartBeatTime);
            }

        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//
            if(!Controller::instance()->getSpeechObj()->alarmTextList().contains(tr("主机通信故障")))
            {
                Controller::instance()->getSpeechObj()->insertAlarmText(tr("主机通信故障"));

            }
            QString curTime=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
            m_sqliteManager->executeQuery(QString("insert into AlarmInfo (类型,状态,报警时间) values ('%1','%2','%3') ").arg("主机通信故障").arg("主机通信故障").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));
            sendAlarmInfo(0,4,11,0,curTime);
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
                m_tcpConnectState = true;
                if(!m_controlCenterHeartbeatTimer->isActive())
                {
                    m_controlCenterHeartbeatTimer->start(c_heartBeatTime);
                }
                //sendControlCenterHeartbeat();
                //    m_debugLogMsg->writeLog(LOG_LEVEL_DEBUG,"连接成功");
            }
            else
            {
                QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//黄色
                Controller::instance()->getSpeechObj()->insertAlarmText(tr("中心通信故障"));
                m_sqliteManager->executeQuery(QString("insert into AlarmInfo (类型,状态,报警时间) values ('%1','%2','%3') ").arg(tr("中心通信故障")).arg(tr("中心通信故障")).arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));
                m_tcpIsConnected = false;
                m_tcpConnectState = false;
                m_controlCenterHeartbeatTimer->stop();
                // m_debugLogMsg->writeLog(LOG_LEVEL_DEBUG,"连接失败");
                //Controller::instance()->getTcpObj()->connectLink();
            }



        }




    });

    connect(m_mainHeartBeatTimer,&CustomTimer::timeout,this,[=]()
    {
        static quint8 num =0;
        if(m_serialConnected)
        {
            num=0;
            m_serialConnected = false;
            if(m_serialConnectState==Disconnected)
            {
                m_serialConnectState = Connected;
            }
            m_serialCurState= true;

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
                    m_sqliteManager->executeQuery(QString("insert into AlarmInfo (类型,状态,报警时间) values ('%1','%2','%3') ").arg("主机通信故障").arg("主机通信故障").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));
                    m_serialCurState= false;
                    QString curTime=QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
                    sendAlarmInfo(0,4,11,0,curTime);

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
            m_architePlanView->clearExcptFireAlarm();
            reSendAllCmd();
            m_serialConnectState= NoState;
        }
    });

    connect(m_checkSendDataTimer,&CustomTimer::timeout,this,[=]()
    {
        bool sendDataState= Controller::instance()->getTcpObj()->writeDataSuccess();
        setIndicator(sendDataState);
    });

    connect(m_updateViewTimer,&QTimer::timeout,this,&CrtWidget::processViewsData);



    connect(this,&CrtWidget::getSerialData,this,[=]()
    {
        if(!m_serialCurState)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setEquiComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//主机通信
            Controller::instance()->getSpeechObj()->removeAlarmText(tr("主机通信故障"));
            m_serialCurState=true;
            //m_serialConnectState=Connected;
        }
    });

    connect(this,&CrtWidget::getTcpData,this,[=]()
    {
        if(!m_tcpConnectState)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//中心通信
            Controller::instance()->getSpeechObj()->removeAlarmText(tr("中心通信故障"));
            m_tcpConnectState = true;
        }
    });



    connect(m_architePlanView,&ArchitePlanView::eliminateNoItemAlarm,this,[=](const QString &extNum,const QString &loopNum,const QString &addrNum,const QString &networkNum,const QString &type,const QString &time)
    {
        m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复时间 ='%1',状态 = '正常',类型='%2' where 状态 != '正常' and 分机号 = '%3' and 回路号 = '%4' and 地址号 = '%5' and 网络号 = '%6' and 类型='%7'").arg(time).arg(type+tr("消除")).arg(extNum).arg(loopNum).arg(addrNum).arg(networkNum).arg(type));
        alarmStatistics(type);
        alarmDataOnTable();

    });

    Controller::instance()->getCommObj()->connectLink();
    if(m_sendEnable)
    {
        Controller::instance()->getSpeechObj()->insertText(tr("中心通信故障"));
        m_sqliteManager->executeQuery(QString("insert into AlarmInfo (类型,状态,报警时间) values ('%1','%2','%3') ").arg(tr("中心通信故障")).arg(tr("中心通信故障")).arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));
        Controller::instance()->getTcpObj()->setConfiguration();
        Controller::instance()->getTcpObj()->connectLink();
        // m_controlCenterSocket =dynamic_cast<QTcpSocket*> (Controller::instance()->getTcpObj()->device());
    }
    connect(Controller::instance()->getTcpObj(),&AbstractLink::sendAllData,this,[=]()
    {
        QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,true));
        setIndicatorState(true);
    });


#ifdef Q_OS_LINUX
    startProcess("sh -c \"echo rpdzkj|sudo -S su\"");
    startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 0 1\"");
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
    m_updateViewTimer->stop();
    m_updateViewTimer->deleteLater();

    m_sqliteManager->close();
    m_sqliteManager->deleteLater();
    m_infoQueryView->close();
    m_infoQueryView->deleteLater();
    m_extNumStateView->deleteLater();

    foreach (CustomTimer*curTimer, m_checkExtNumHash.values()) {
        if(curTimer!=nullptr)
        {
            curTimer->stop();
            curTimer->deleteLater();
        }
    }
    m_checkExtNumHash.clear();
    delete m_alarmContainer;
    delete m_toolBarContainer;
    m_loginQuickView->close();
    m_loginQuickView->deleteLater();
    m_closeLoginView->close();
    m_closeLoginView->deleteLater();
    m_settingView->close();
    m_settingView->deleteLater();
    delete m_serialDataProtocol ;
    delete m_monitoringProtocol;
    delete m_indicatorProtocol;
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
    m_architePlanView->saveArchiteInfoToDb();
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

#endif

}

void CrtWidget::setControlCenterEnable(bool enable)
{
    m_sendEnable = enable;
    if(!enable)
    {
        m_controlCenterHeartbeatTimer->stop();
        Controller::instance()->getTcpObj()->disconnectLink();
        QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));//中心通信
        Controller::instance()->getSpeechObj()->removeAlarmText(tr("中心通信故障"));
        Controller::instance()->getSpeechObj()->removeAlarmText(tr("传输故障"));

    }

    //QMetaObject::invokeMethod()
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
    QString clearTime =  QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复时间 ='%1',状态 = '正常', 类型 =类型||'消除' where 状态 != '正常' ").arg(clearTime));
    close();
}

void CrtWidget::resetLoginViewShow()
{
    QMetaObject::invokeMethod(m_resetLoginObj,"clearLoginInfo");
    m_resetLoginQuickView->close();
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

}


void CrtWidget::closeEvent(QCloseEvent *event)
{

    Controller::instance()->getSerialConfigurationManager()->saveConfiguration();
    Controller::instance()->getTcpConfigurationManager()->saveConfiguration();
    Controller::instance()->getTransportInfo()->saveTransportInfoToJson();
    Controller::instance()->getOperatorInfo()->insertEvent(tr("系统关机"));

    m_architePlanView->saveInfo();
#ifdef Q_OS_LINUX
    startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 1 0\"");
    startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 2 0\"");
#endif

    closeQuickView();

    event->accept();
}


void CrtWidget::widgetExit()
{
    m_closeLoginView->close();//关闭以前打开的
    m_closeLoginView->show();
    if(m_closeLoginObj!=nullptr)
    {
        QMetaObject::invokeMethod(m_closeLoginObj,"clearLoginInfo");
    }
    // close();
}

void CrtWidget::closeLogInView()
{
    m_closeLoginView->close();//关闭以前打开的
}

void CrtWidget::loginWidgetShow()
{
    m_loginQuickView->close();//关闭以前打开的
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
        m_settingView->close();//关闭以前的打开
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
    if(alarm==tr("全部"))
    {
        QMetaObject::invokeMethod(m_alarmObj,"setNum",Q_ARG(QVariant,0),Q_ARG(QVariant,0));
    }
    else
    {
        int totalNum= DataStore::numOfTypeItem(alarm);
        QMetaObject::invokeMethod(m_alarmObj,"setNum",Q_ARG(QVariant,totalNum),Q_ARG(QVariant,0));
    }

}

void CrtWidget::alarmStatistics(const QString &type)
{
    Q_ASSERT(m_alarmObj);
    int typeNum = m_architePlanView->numOfTypeAlarm(type);
    if(m_architePlanView->currentAlarmType()==type)
    {
        QMetaObject::invokeMethod(m_alarmObj,"setNum",Q_ARG(QVariant,typeNum),Q_ARG(QVariant,0));
    }

    if(type.endsWith(tr("火警")))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            // QMetaObject::invokeMethod(m_alarmObj,"startFireAnimation",Q_ARG(QVariant,true));
        }
        //        else
        //        {
        //            QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
        //            //QMetaObject::invokeMethod(m_alarmObj,"startFireAnimation",Q_ARG(QVariant,false));
        //        }

        QMetaObject::invokeMethod(m_alarmObj,"setFireAlarmText",Q_ARG(QVariant,typeNum));
    }
    else if(type.endsWith(tr("启动")))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setLinkageAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            //QMetaObject::invokeMethod(m_alarmObj,"startLinkageAnimation",Q_ARG(QVariant,true));
        }
        //        else
        //        {
        //            QMetaObject::invokeMethod(m_alarmObj,"setLinkageAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
        //            //QMetaObject::invokeMethod(m_alarmObj,"startLinkageAnimation",Q_ARG(QVariant,false));
        //        }

        QMetaObject::invokeMethod(m_alarmObj,"setLinkageText",Q_ARG(QVariant,typeNum));
    }
    else if(type.endsWith(tr("监管")))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setSuperviseAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            //QMetaObject::invokeMethod(m_alarmObj,"startSuperviseAnimation",Q_ARG(QVariant,true));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setSuperviseAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
            // QMetaObject::invokeMethod(m_alarmObj,"startSuperviseAnimation",Q_ARG(QVariant,false));
        }
        QMetaObject::invokeMethod(m_alarmObj,"setSuperviseText",Q_ARG(QVariant,typeNum));
    }
    else if(type.endsWith(tr("故障")))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setfaultAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));
            //QMetaObject::invokeMethod(m_alarmObj,"startFaultAnimation",Q_ARG(QVariant,true));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setfaultAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
            //QMetaObject::invokeMethod(m_alarmObj,"startFaultAnimation",Q_ARG(QVariant,false));
        }
        QMetaObject::invokeMethod(m_alarmObj,"setFaultText",Q_ARG(QVariant,typeNum));
    }
    else if(type.endsWith(tr("反馈")))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setFeedbackColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"red"));
            // QMetaObject::invokeMethod(m_alarmObj,"startFeedbackAnimation",Q_ARG(QVariant,true));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setFeedbackColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
            //QMetaObject::invokeMethod(m_alarmObj,"startFeedbackAnimation",Q_ARG(QVariant,false));
        }
        QMetaObject::invokeMethod(m_alarmObj,"setFeedbackText",Q_ARG(QVariant,typeNum));
    }
    else if(type.endsWith(tr("屏蔽")))
    {
        if(typeNum>0)
        {
            QMetaObject::invokeMethod(m_alarmObj,"setShieldAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"orange"));//橘黄色
            //QMetaObject::invokeMethod(m_alarmObj,"startShieldAnimation",Q_ARG(QVariant,true));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setShieldAlarmColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"gray"));
            //QMetaObject::invokeMethod(m_alarmObj,"startShieldAnimation",Q_ARG(QVariant,false));
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

            QMetaObject::invokeMethod(m_alarmObj,"setMainPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
            // QMetaObject::invokeMethod(m_alarmObj,"startMainPowerAnimation",Q_ARG(QVariant,false));
        }
        else
        {
            //QMetaObject::invokeMethod(m_alarmObj,"startMainPowerAnimation",Q_ARG(QVariant,true));
            QMetaObject::invokeMethod(m_alarmObj,"setMainPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//主电故障
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
            // QMetaObject::invokeMethod(m_alarmObj,"startStandbyPowerAnimation",Q_ARG(QVariant,false));
        }
        else
        {
            QMetaObject::invokeMethod(m_alarmObj,"setStandbyPowerColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));//备电故障
            //QMetaObject::invokeMethod(m_alarmObj,"startStandbyPowerAnimation",Q_ARG(QVariant,true));
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
    m_toolBarObj = toolBarQuickView->rootObject();
    toolBarQuickView->setMinimumWidth(1200);
    // toolBarQuickView->setWidth(this->width());
    m_toolBarContainer = QWidget::createWindowContainer(toolBarQuickView, this);
    m_toolBarContainer->setMinimumHeight(60);
    m_toolBarContainer->setMaximumHeight(60);
    m_toolBarContainer->setMinimumWidth(1200);

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

    m_controlCenterHeartbeatTimer = new CustomTimer();
    m_mainHeartBeatTimer = new CustomTimer();
    m_updateViewTimer = new QTimer;
    m_checkSendDataTimer = new CustomTimer;
    m_checkSendDataTimer->setSingleShot(true);
    m_infoTableView = new InfoTableView(this);
    m_infoTableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_sqliteManager = SqlManager::fromDriver("QSQLITE");
    m_infoTableView->setMaximumHeight(170);


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
    m_loginQuickView->setMaximumSize(QSize(480,240));
    m_loginQuickView->setMinimumSize(QSize(480,240));

    m_closeLoginView = new QQuickView;
    m_closeLoginView->setSource(QUrl("qrc:/qml/logInWindow/LoginWindow.qml"));
    m_closeLoginView->rootContext()->setContextProperty("CrtWidget",this);
    m_closeLoginView->setGeometry(500,50,m_closeLoginView->width(),m_closeLoginView->height());
    m_closeLoginView->setTitle(tr("用户登陆之后关闭"));
    m_closeLoginObj= m_closeLoginView->rootObject();
    m_closeLoginView->setMaximumSize(QSize(480,240));
    m_closeLoginView->setMinimumSize(QSize(480,240));

    m_settingView = new QQuickView;
    m_settingView->setSource(QUrl("qrc:/qml/infoSetting/SettingWindow.qml"));
    m_settingView->setGeometry(300,50,m_settingView->width(),m_settingView->height());
    m_settingView->setTitle(tr("信息设置界面"));
    m_settingObj = m_settingView->rootObject();
    m_settingView->setMinimumSize(QSize(900,640));
    m_settingView->setMaximumSize(QSize(900,640));
    //m_settingView->setResizeMode(QQuickView::SizeRootObjectToView);

    m_resetLoginQuickView = new QQuickView;
    m_resetLoginQuickView->setSource(QUrl("qrc:/qml/logInWindow/ResetLoginItem.qml"));
    m_resetLoginQuickView->setTitle(tr("复位权限登陆"));
    m_resetLoginQuickView->setMinimumSize(QSize(320,180));
    m_resetLoginQuickView->setMaximumSize(QSize(320,180));

    m_resetLoginObj = m_resetLoginQuickView->rootObject();

    m_infoQueryView = new QQuickView;
    m_infoQueryView->setSource(QUrl("qrc:/qml/infoSetting/InfoQuery.qml"));
    m_infoQueryView->setGeometry(300,50,m_infoQueryView->width(),m_infoQueryView->height());
    m_infoQueryView->setTitle(tr("信息查询界面"));
    m_infoQueryView->setResizeMode(QQuickView::SizeRootObjectToView);

    m_extNumStateView = new QQuickView;
    m_extNumStateView->setSource(QUrl("qrc:/qml/deviceOnline/DeviceOnline.qml"));
    m_extNumStateView->setTitle(tr("主机在线状态"));
    m_extNumObj = m_extNumStateView->rootObject();
    m_extNumStateView->setMaximumSize(QSize(540,360));
    m_extNumStateView->setMinimumSize(QSize(540,360));


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
    // setMySqlInfo();
    Q_ASSERT(m_alarmObj);
    connect(m_alarmObj,SIGNAL(clearVoice()),this,SLOT(clearVoice()));
    //connect(m_alarmObj,SIGNAL(reSendCmd()),this,SLOT(reSendAllCmd()));
    Q_ASSERT(m_resetLoginObj);
    connect(m_resetLoginObj,SIGNAL(curLoginState(bool)),this,SLOT(resetLoginState(bool)));
    Q_ASSERT(m_settingObj);
    connect(m_settingObj,SIGNAL(setControlCenter(bool)),this,SLOT(setControlCenterEnable(bool)));
    connect(m_settingObj,SIGNAL(closeHeartbeat()),this,SLOT(closeControlCenterHeartbeat()));
    Q_ASSERT(m_loginObj);
    connect(m_loginObj,SIGNAL(closeView()),this,SLOT(logWidgetClose()));
    Q_ASSERT(m_closeLoginObj);
    connect(m_closeLoginObj,SIGNAL(startClose()),this,SLOT(closeAll()));
    connect(m_closeLoginObj,SIGNAL(closeView()),this,SLOT(closeLogInView()));
    Q_ASSERT(m_toolBarObj);
    connect(m_toolBarObj,SIGNAL(showOnlineState()),this,SLOT(showOnlineView()));
}

void CrtWidget::alarmDataOnTable()
{


    QStringList alarmInfoList;
    alarmInfoList <<"网络号"<< "分机号"<<"回路号"<<"地址号"<< "设备"
                 <<"类型"<<"报警时间"<<"系统"<< "建筑名称"<<"楼层"<<"位置";
    //    if(m_architePlanView->currentAlarmType()==tr("全部"))
    //    {
    m_infoTableView->tableModel()->sqlCommit(QString("select %1 from AlarmInfo order by 报警时间 desc limit 1000").arg(alarmInfoList.join(",")));

    // }
    //    else
    //    {
    //        m_infoTableView->tableModel()->sqlCommit(QString("select %1 from AlarmInfo where 类型 like '%%2' and 状态 != '正常' ").arg(alarmInfoList.join(",")).arg(m_architePlanView->currentAlarmType()));
    //        int totalNum = DataStore::numOfTypeItem(m_architePlanView->currentAlarmType());
    //        QMetaObject::invokeMethod(m_alarmObj,"setTotalNum",Q_ARG(QVariant,totalNum));
    //    }
    // m_infoTableView->toMaxPosition();
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
        Controller::instance()->getTcpObj()->sendData(m_monitoringProtocol->dataPackage(valueList));
        QMetaObject::invokeMethod(m_alarmObj,"startTransformAnimation",Q_ARG(QVariant,true));
        setTransportState();
    }

}



void CrtWidget::serialDataProcessing(const QByteArray &arrayValue)
{
    QMutexLocker locker(&m_mutex);
    QList<QByteArray>dataArrayList=  m_serialDataProtocol->frameData(arrayValue);
    if(dataArrayList.isEmpty())
    {
        return;
    }

    m_serialConnected = true;
    emit getSerialData();

    foreach (QByteArray array, dataArrayList)
    {
        quint8 eventNum =  m_serialDataProtocol->dataByte(array,0);//事件
        quint8 loopNum =  m_serialDataProtocol->dataByte(array,1);//回路
        quint8 addrNum =  m_serialDataProtocol->dataByte(array,2);//地址
        quint8 type = m_serialDataProtocol->dataByte(array,3);
        quint8 curExtNum = (type>>2)&0x3f;
        quint8 networkNum=0;
        quint8 minuteValue = m_serialDataProtocol->dataByte(array,8);
        quint8 minute= minuteValue&0x7f;
        if((minuteValue&0x80)!=0)
        {
            networkNum = minute;
        }
        m_extNumStateHash[curExtNum] = true;
        QString extNum = QString("%1").arg((type>>2)&0x3f);
        if(eventNum==0x99)//心跳
        {
           // m_serialConnected = true;

            emit checkExtNum(curExtNum,networkNum);
        }
        else if(eventNum== 0x20)
        {
            if((loopNum|addrNum)==0x00)
            {
                Controller::instance()->getOperatorInfo()->insertEvent(tr("本机复位"));
                m_architePlanView->clearAlarmFromExtNum(extNum,QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
            }
            else if((loopNum&addrNum)==0x01)
            {
                Controller::instance()->getOperatorInfo()->insertEvent(tr("网络复位"));
                m_architePlanView->clearExcptFireAlarm();

            }
        }
        else if(eventNum==0x01)
        {
            m_fireDataList.push_back(array);
            if(!m_updateViewTimer->isActive())
            {
                m_updateViewTimer->start(c_updateTime);
            }
        }
        else
        {


            m_readSerialDataList.push_back(array);
            if(!m_updateViewTimer->isActive())
            {
                m_updateViewTimer->start(c_updateTime);
            }

        }

    }

}

void CrtWidget::tcpDataProcessing(const QByteArray &arrayValue)
{

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
            return;
        }
        emit getTcpData();
        m_tcpIsConnected = true;
        foreach (QByteArray dataArray, dataArrayList)
        {
            int dataSize = dataArray.size();
            // qDebug() << "dataSize" << dataSize;
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
                Controller::instance()->getTcpObj()->sendData(m_monitoringProtocol->dataPackage(sendArrayList));
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
    quint8 curChannel= ((channelNum&0xe0)|(analogTypeHash.key(analogType)&0x1f));

    QByteArray packageArray;
    packageArray.resize(8);
    QList<QByteArray>dataArrayList;
    packageArray[0] = num;
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

    QByteArray sendDataArray= m_serialDataProtocol->dataPackage(dataArrayList);
    Controller::instance()->getCommObj()->sendData(sendDataArray);
}

void CrtWidget::startReset()
{
    m_checkSendDataTimer->stop();
    Controller::instance()->getOperatorInfo()->insertEvent(tr("复位"));
    resetAllState();
}

void CrtWidget::clearVoice()
{
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
//    static quint8 extNum=0;
//    QByteArray array;
//    array.resize(15);
//    array[0] =0x7e;
//    array[1] =0x01;
//    array[2] =0x0a;
//    array[3] =0x99;//类型
//    array[4] =0x00;
//    array[5] =0x00;
//    array[6] =extNum<<2;
//    array[7] =0x15;
//    array[8] =0x06;
//    array[9] =0x12;
//    array[10] =0x18;
//    array[11] =0x19;
//    array[12] =0x09;
//    quint8 sum = 0;
//    for(int i=3;i<=12;i++)
//    {
//        sum += (quint8)array.at(i);
//    }
//    array[13] =sum;
//    array[14] =0x7e;
//    extNum++;
//    Controller::instance()->getCommObj()->sendData(array);

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
    static quint8 packageNum=0;
    QByteArray sendArray;
    sendArray.resize(5);
    sendArray[0]=packageNum;
    sendArray[1]=0x05;
    sendArray[2]=0x22;
    sendArray[3]=0x80;
    sendArray[4]=0x00;
    QList<QByteArray> sendArrayList;
    sendArrayList.push_back(sendArray.mid(0,1));
    sendArrayList.push_back(sendArray.mid(1,1));
    sendArrayList.push_back(sendArray.mid(2,1));
    sendArrayList.push_back(sendArray.mid(3,1));
    sendArrayList.push_back(sendArray.mid(4,1));
    packageNum++;
    packageNum=packageNum%256;
    //qDebug() << packageNum;
    QByteArray curArray = m_serialDataProtocol->dataPackage(sendArrayList);
    Controller::instance()->getCommObj()->sendData(curArray);

    //qDebug() <<"curArray" <<curArray.toHex();

}

void CrtWidget::reSendUnicastCmd(quint8 extNum, quint8 networkNum)
{
    static quint8 packageNum=0;
    QByteArray sendArray;
    sendArray.resize(5);
    sendArray[0]=packageNum;
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
    packageNum++;
    packageNum=packageNum%256;
    //qDebug() << packageNum;
    QByteArray curArray = m_serialDataProtocol->dataPackage(sendArrayList);
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
    // m_architePlanView->eliminateNoItemAlarm(
#ifdef Q_OS_LINUX
    startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 1 0\"");
    startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 2 0\"");
#endif

    reSendCmd();
    Controller::instance()->getSpeechObj()->startSpeech();

}

void CrtWidget::sendControlCenterHeartbeat()
{
    QList<QByteArray>dataArrayList;
    dataArrayList.push_back(QString("%1").arg(19,4,10,QChar('0')).toLocal8Bit());
    dataArrayList.push_back(QString("%1").arg(m_heartbeatIndex,4,10,QChar('0')).toLocal8Bit());
    dataArrayList.push_back(QString("%1").arg(999).toLocal8Bit());
    QByteArray sendArray=  m_monitoringProtocol->dataPackage(dataArrayList);
    Controller::instance()->getTcpObj()->sendData(sendArray);
    QMetaObject::invokeMethod(m_alarmObj,"setCenterComColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));//中心通信
    Controller::instance()->getSpeechObj()->removeAlarmText(tr("中心通信故障"));
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
        Controller::instance()->getTcpObj()->sendData(m_monitoringProtocol->dataPackage(valueList));
        setTransportState();

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
        Controller::instance()->getTcpObj()->sendData(m_monitoringProtocol->dataPackage(valueList));
        setTransportState();
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
        Controller::instance()->getTcpObj()->sendData(m_monitoringProtocol->dataPackage(valueList));
        setTransportState();
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
    }
    else
    {
#ifdef Q_OS_LINUX
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 2 0\"");
        startProcess("sh -c \"echo rpdzkj|sudo -S setled.sh 1 1\"");
#endif
        Controller::instance()->getSpeechObj()->insertAlarmText(tr("传输故障"));
        m_sqliteManager->executeQuery(QString("insert into AlarmInfo (类型,状态,报警时间) values ('%1','%2','%3') ").arg("传输故障").arg("传输故障").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));
        QMetaObject::invokeMethod(m_alarmObj,"setTransformColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"yellow"));
    }
}

void CrtWidget::closeAllOnlineState()
{
    QList<quint8>extNumKeyList=m_extNumStateHash.keys();
    foreach (quint8 curValue, extNumKeyList)
    {
        m_extNumStateHash[curValue] = false;
        m_extNumTimesHash[curValue] = 0;
        m_extOnlineStateHash[curValue]=NoState;
        if(m_extNumObj!=nullptr)
        {
            QMetaObject::invokeMethod(m_extNumObj,"setIndicatorState",Q_ARG(QVariant,curValue),Q_ARG(QVariant,"yellow"));
        }

    }
    if(!m_extNumStateView->isActive())
    {
        m_extNumStateView->show();
    }
}

void CrtWidget::processViewsData()
{
    QMutexLocker locker(&m_mutex);
    if(m_readSerialDataList.size()<=0 &&m_fireDataList.size()<=0)
    {
        m_updateViewTimer->stop();
        return;
    }
    QByteArray array;
    if(m_fireDataList.size()>0)
    {
        array = m_fireDataList.takeAt(0);
    }
    else if(m_readSerialDataList.size()>0)
    {
        array = m_readSerialDataList.takeAt(0);
    }

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
    quint8 hour = m_serialDataProtocol->dataByte(array,7);
    quint8 minute= minuteValue&0x7f;
    quint8 second= m_serialDataProtocol->dataByte(array,9);

    QString extNum = QString("%1").arg((type>>2)&0x3f);

    //报警主机
    if((minuteValue&0x80)==0)
    {
        if(second<=0x60)
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
    quint8 sysNum = m_sysNameHash.key(sysName);
    if(sysNum<0)
    {
        sysNum=0;
    }
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
        QString currentState = alarmTypeHash.value(eventNum);
        quint8 alarmState =1;
        int sysNum = m_sysNameHash.key(sysName);
        if(sysNum<0)
        {
            sysNum=0;
        }
        if(eventNum==0x03)
        {
            switch ((type&0x03))
            {
            case 1:
                currentState = tr("常开门故障");
                alarmState=2;
                break;
            case 2:
                currentState = tr("常闭门打开");
                alarmState=3;
                break;
            default:
                currentState = tr("故障");
                alarmState=1;
                break;
            }

        }
        else if(eventNum==0x0d)
        {
            if((type&0x03)==0x01)
            {
                currentState = tr("常开门关闭");
                alarmState=4;

            }

        }
        else if(eventNum==0x01)
        {
            QDateTime sendDateTime =QDateTime::fromString(timeStr,"yyyy/MM/dd hh:mm:ss");
            QString dateTimeStr= sendDateTime.toString("yyyyMMddhhmmss");
            sendFireInfo(extNum.toShort(),loopNum,addrNum,dateTimeStr);
        }


        if(second==0xa0)
        {
            if(faultStateHash.keys().contains(month))
            {
                if(year==0xa0)
                {
                    currentState = tr("A0模块")+ faultStateHash[month];
                }
                else if(year==0xa1)
                {
                    currentState = tr("A1通道")+ faultStateHash[month];
                }
                else if(year==0xa2)
                {
                    currentState = tr("A2通道")+ faultStateHash[month];
                }
                else if(year==0xa3)
                {
                    currentState =tr("A3通道")+ faultStateHash[month];
                }

                alarmState = year+month;

            }

        }

        quint8 alarmTypeNum = 6;
        QString curAlarmType = alarmTypeHash.value(eventNum);

        if(curAlarmType==tr("火警"))
        {
            alarmTypeNum=0;
        }
        else if(curAlarmType==tr("监管"))
        {
            alarmTypeNum=1;
        }
        else if(curAlarmType==tr("启动"))
        {
            alarmTypeNum=2;
        }
        else if(curAlarmType==tr("反馈"))
        {
            alarmTypeNum=3;
        }
        else if(curAlarmType==tr("故障"))
        {
            alarmTypeNum=4;
        }
        else if(curAlarmType==tr("屏蔽"))
        {
            alarmTypeNum=5;
        }

        sendAlarmInfo(sysNum,alarmTypeNum,alarmState,0,timeStr);
        if(item!=nullptr)
        {
            if(!DataStore::getTypeItemList(alarmTypeHash.value(eventNum)).contains(item))
            {
                m_architePlanView->createAlarm(item,alarmTypeHash.value(eventNum),currentState,timeStr);
            }
            else
            {
                if(timeStr!=item->alarmTime(alarmTypeHash.value(eventNum)))
                {
                    m_sqliteManager->executeQuery(m_updateAlarmSqlInfo.arg(timeStr).arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()));
                    alarmDataOnTable();
                }

            }
        }
        else
        {
            QString curType = alarmTypeHash.value(eventNum);
            if(!DataStore::haveTypeItem(curType,extNum,QString::number(loopNum),QString::number(addrNum),networkNum))
            {



                m_sqliteManager->executeQuery(m_alarmSqlInfo.arg(extNum).arg(QString::number(loopNum)).arg(QString::number(addrNum)).arg(networkNum).arg(tr("未定义设备"))
                                              .arg(tr("未定义设备")).arg(curType).arg(currentState).arg(timeStr)
                                              .arg("").arg("").arg("")
                                              .arg("").arg("").arg("").arg("").arg(""));
                DataStore::insertTypeNoItem(curType,extNum,QString::number(loopNum),QString::number(addrNum),networkNum);


                alarmStatistics(curType);

            }
            else
            {

                m_sqliteManager->executeQuery(m_updateAlarmSqlInfo.arg(timeStr).arg(extNum).arg(QString::number(loopNum)).arg(QString::number(addrNum)).arg(networkNum));

            }

            alarmDataOnTable();
        }

        QString analogType;
        QVariant analogValue;

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
                    analogType = tr("电流(A)");
                }
                else if((type&0x03)==2)
                {
                    analogType = tr("温度(℃)");
                }

                analogValue = year*256+second;
                if(item!=nullptr)
                {
                    item->analogType() = analogType;
                    item->setAnlogValue(0,analogValue);
                }
                m_sqliteManager->executeQuery(QString("insert into AnalogInfo values('%1','%2','%3','%4','%5','%6','%7')").arg(extNum).arg(QString::number(loopNum)).arg(QString::number(addrNum)).arg(networkNum).arg(0).arg(analogType).arg(analogValue.toString()));
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
        QString eliminateAlarmType;
        quint8 alarmType =6;
        if(eventNum==0x02)
        {
            eliminateAlarmType = tr("反馈");
            alarmType =3;
        }
        else if(eventNum==0x04)
        {
            eliminateAlarmType=tr("故障");
            alarmType =4;
        }
        else if(eventNum==0x06)
        {
            eliminateAlarmType=tr("启动");
            alarmType =2;
        }
        else if(eventNum==0x0c)
        {
            eliminateAlarmType=tr("屏蔽");
            alarmType =5;
        }
        m_architePlanView->eliminateAlarm(extNum,QString::number(loopNum),QString::number(addrNum),networkNum,eliminateAlarmType);

        if(item!=nullptr)
        {
            sendAlarmInfo(sysNum,alarmType,1,0,timeStr);
        }
        else
        {
            sendAlarmInfo(0,alarmType,1,0,timeStr);
            m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复时间 ='%1',类型= '%2',状态 = '正常' where 状态 = '%3' and 分机号 = '%4' and 回路号 = '%5' and 地址号 = '%6' and 网络号 = '%7'").arg(timeStr).arg(eliminateAlarmType+tr("消除")).arg(eliminateAlarmType).arg(extNum).arg(QString::number(loopNum)).arg(QString::number(addrNum)).arg(networkNum));
            DataStore::deleteTypeItem(extNum,QString::number(loopNum),QString::number(addrNum),networkNum);
            alarmDataOnTable();
            if(eventNum==0x02)
            {
                alarmStatistics(tr("反馈"));
            }
            else if(eventNum==0x04)
            {
                alarmStatistics(tr("故障"));
            }
            else if(eventNum==0x06)
            {
                alarmStatistics(tr("启动"));
            }
            else if(eventNum==0x0c)
            {
                alarmStatistics(tr("屏蔽"));
            }


        }


        //alarmStatistics();
        if(eventNum==0x04)
        {
            QString currentState;
            quint8 curStateNum=0;
            switch ((type&0x03))
            {
            case 0:
            {
                currentState = tr("故障恢复");
                curStateNum =6;
            }
                break;
            case 1:
            {
                currentState = tr("常开门故障恢复");
                curStateNum =7;
            }
                break;

            case 2:
            {
                currentState = tr("常开门关闭");
                curStateNum =8;
            }

                break;
            default:
                break;
            }

            sendAlarmInfo(sysNum,4,curStateNum,0,timeStr);


        }
        else if(eventNum==0x02)
        {
            if((type&0x03)==0x01)
            {
                sendAlarmInfo(sysNum,3,9,0,timeStr);

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
            communicationStatus(commuStatusHash.value(eventNum),true);
            powerState = tr("正常");
            powerStateNum =0;
        }
        else if(loopNum==1)
        {
            communicationStatus(commuStatusHash.value(eventNum),false);
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
//    case 0x20:
//    {
//        if((loopNum|addrNum)==0x00)
//        {
//            Controller::instance()->getOperatorInfo()->insertEvent(tr("本机复位"));
//            m_architePlanView->clearAlarmFromExtNum(extNum,timeStr);
//        }
//        else if((loopNum&addrNum)==0x01)
//        {
//            Controller::instance()->getOperatorInfo()->insertEvent(tr("网络复位"));
//            m_architePlanView->clearExcptFireAlarm();

//        }
//    }
//        break;
        //    case 0x23:
        //    {
        //        Controller::instance()->getSpeechObj()->stopSpeech();
        //        Controller::instance()->getOperatorInfo()->insertEvent(tr("消音"));
        //    }
        //        break;
        //case 0x31:
    case 0x32:
    {
        QString handOrAutoState = tr("自动");
        quint8 handOrAutoStateNum =2;
        switch (type)
        {
        case 0:
        {
            handOrAutoState = tr("默认");
            Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash.value(eventNum),handOrAutoState);

            handOrAutoStateNum=0;
            QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"purple"));
            QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
        }
            break;
        case 1:
        {
            handOrAutoState = tr("手动");
            Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash.value(eventNum),handOrAutoState);

            handOrAutoStateNum=1;
            QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"blue"));
            QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
        }
            break;
        case 2:
        {
            handOrAutoState = tr("自动");
            Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash.value(eventNum),handOrAutoState);

            handOrAutoStateNum =2;
            QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
            QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
        }
            break;
        default:
        {
            switch (loopNum)
            {

            case 0:
            {
                handOrAutoState = tr("默认");
                Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash.value(eventNum),handOrAutoState);
                handOrAutoStateNum=0;
                QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"purple"));
                QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
            }
                break;
            case 1:
            {
                handOrAutoState = tr("手动");
                Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash.value(eventNum),handOrAutoState);
                handOrAutoStateNum=1;
                QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"blue"));
                QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
            }
                break;
            case 2:
            {
                handOrAutoState = tr("自动");
                Controller::instance()->getOperatorInfo()->insertEvent(commuStatusHash.value(eventNum),handOrAutoState);
                handOrAutoStateNum =2;
                QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoColor",Q_ARG(QVariant,true),Q_ARG(QVariant,"green"));
                QMetaObject::invokeMethod(m_alarmObj,"setHandOrAutoText",Q_ARG(QVariant,handOrAutoState));
            }
                break;
            default:
                break;
            }
        }
            break;

        }

        quint8 sysNum = m_sysNameHash.key(sysName);
        if(sysNum<0)
        {
            sysNum =0;
        }
        sendHandOrAutoState(sysNum,handOrAutoStateNum,0,timeStr);
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

                currentItem->deviceLocation() = QString::number(type*256+year)+tr("米");
                m_architePlanView->createAlarm(currentItem,tr("火警"),tr("光纤火警"));
                QString str= currentItem->sysOfDevice();
                quint8 alarmSysNum= m_sysNameHash.key(str);
                if(alarmSysNum<0)
                {
                    alarmSysNum =0;
                }
                sendAlarmInfo(alarmSysNum,0,5,0,timeStr);//光纤火警

            }
            else
            {
                if(!DataStore::haveTypeItem(tr("火警"),QString::number(date),QString::number(loopNum),QString::number(addrNum),QString::number(month)))
                {
                    m_sqliteManager->executeQuery(m_alarmSqlInfo.arg(QString::number(date)).arg(QString::number(loopNum)).arg(QString::number(addrNum)).arg(QString::number(month)).arg(tr("未定义设备"))
                                                  .arg(tr("未定义设备")).arg(tr("火警")).arg(tr("光纤火警")).arg(timeStr).arg("").arg("").arg("").arg("").arg("")
                                                  .arg("").arg("").arg(""));
                    DataStore::insertTypeNoItem(tr("火警"),QString::number(date),QString::number(loopNum),QString::number(addrNum),QString::number(month));

                    sendAlarmInfo(0,0,5,0,timeStr);//光纤火警
                    alarmDataOnTable();
                    alarmStatistics(tr("火警"));
                }
            }
        }
            break;
        case 0x02:
        {
            if(currentItem!=nullptr)
            {
                currentItem->deviceLocation() = QString::number(type*256+year)+tr("米");
                m_architePlanView->createAlarm(currentItem,tr("故障"),tr("光纤故障"));
                sendAlarmInfo(sysNum,4,10,0,timeStr);
            }
            else
            {
                if(!DataStore::haveTypeItem(tr("故障"),QString::number(date),QString::number(loopNum),QString::number(addrNum),QString::number(month)))
                {
                    m_sqliteManager->executeQuery(m_alarmSqlInfo.arg(QString::number(date)).arg(QString::number(loopNum)).arg(QString::number(addrNum)).arg(QString::number(month)).arg(tr("未定义设备"))
                                                  .arg(tr("未定义设备")).arg(tr("故障")).arg(tr("光纤故障")).arg(timeStr).arg("").arg("").arg("").arg("").arg("")
                                                  .arg("").arg("").arg(""));
                    DataStore::insertTypeNoItem(tr("故障"),QString::number(date),QString::number(loopNum),QString::number(addrNum),QString::number(month));

                    alarmDataOnTable();
                    sendAlarmInfo(0,4,10,0,timeStr);
                    alarmStatistics("故障");
                }
            }
        }
            break;
        case 0x03:
        {
            m_architePlanView->eliminateAlarm(QString::number(date),QString::number(loopNum),QString::number(addrNum),QString::number(month),tr("故障"));
            if(currentItem!=nullptr)
            {
                sendAlarmInfo(sysNum,4,6,0,timeStr);
            }
            else
            {
                m_sqliteManager->executeQuery(QString("update AlarmInfo set 报警恢复时间 ='%1',状态 = '正常',类型 ='故障消除' where 状态 != '正常' and 类型 ='故障' and 分机号 = '%2' and 回路号 = '%3' and 地址号 = '%4' and 网络号 = '%5'").arg(timeStr).arg(QString::number(date)).arg(QString::number(loopNum)).arg(QString::number(addrNum)).arg(QString::number(month)));
                DataStore::deleteTypeItem(QString::number(date),QString::number(loopNum),QString::number(addrNum),QString::number(month));
                alarmDataOnTable();
                alarmStatistics("故障");
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
        QVariant analogValue;
        QString analogType;
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
                analogType = tr("交流电流(mA)");
                analogValue = date*256+month;
            }
                break;
            case 1://直流电流
            {
                analogType = tr("直流电流(mA)");
                analogValue = (date*256+month)*10;
            }
                break;
            case 2://交流电压
            {
                analogType = tr("交流电压(V)");
                analogValue = date*256+month;
            }
                break;
            case 3://直流电压
            {
                analogType = tr("直流电压(V)");
                analogValue = (date*256+month)*0.1;
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
            {
                analogType = tr("剩余电流(mA)");
                analogValue = year*256+date;
            }
                break;
            case 2:
            {
                analogType = tr("温度(℃)");
                analogValue = year*256+date;
            }
                break;
            default:
                break;
            }

        }

        if(item!=nullptr)
        {
            item->analogType() = analogType;
            item->setAnlogValue(curChannel,analogValue);
        }
        m_sqliteManager->executeQuery(QString("insert into AnalogInfo values('%1','%2','%3','%4','%5','%6','%7','%8')").arg(extNum).arg(QString::number(loopNum)).arg(QString::number(addrNum)).arg(networkNum).arg(curChannel).arg(analogType).arg(analogValue.toString()).arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));
    }


        break;
    default:
        break;
    }
}

void CrtWidget::showOnlineView()
{
    if(m_extNumStateView!=nullptr)
    {
        m_extNumStateView->close();
        m_extNumStateView->show();
    }
}


