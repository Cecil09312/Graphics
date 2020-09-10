#include "architeplanview.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include "jsonEdit/jsonedit.h"
#include "graphicsWidget/graphicsitem.h"
#include <QDebug>
#include "control/controller.h"
#include <QModelIndex>
#include <QQuickItem>
#include <QQmlContext>
#include <QDesktopWidget>
//#ifdef Q_OS_LINUX
#include "xlsxdocument.h"

using namespace QXlsx;
//#endif

QHash<int,GraphicsView *>ArchitePlanView::m_widgetHash =QHash<int,GraphicsView *>();
bool ArchitePlanView::m_itemLimit = false;
ArchitePlanView::ArchitePlanView(QWidget *parent)
    : QWidget(parent),
      m_currentAlarmType(tr("火警")),
      m_alarmPos(0),
      m_alarmNum(0),
      m_firstSetSysInfo(false)
{

    initWidget();
    //    setGlobalArchiteFromJson();
    //    initFromJsonFile();
    setContextMenuPolicy(Qt::CustomContextMenu);

    m_typeList << tr("火警") << tr("启动") << tr("监管") << tr("故障")<< tr("反馈") << tr("屏蔽");
    connect(this,&ArchitePlanView::customContextMenuRequested,this,[=](const QPoint&/*pos*/)
    {
        if(m_tabWidget->currentIndex()==1)
        {
            showMenu(QCursor::pos());
        }

    });


    connect(m_deleteAction,&QAction::triggered,this,[=]()
    {

        GraphicsView *view= currentGraphicsView();
        if(view!=nullptr)
        {
            GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
            if(scene!=nullptr)
            {
                GraphicsItem*item= dynamic_cast<GraphicsItem*>(scene->itemAt(scene->currentScenePos(),QTransform())) ;
                if(item!=nullptr)
                {

                    if(item->currentState()=="OK"||item->currentState().isEmpty())
                    {
                        QMessageBox messageBox(QMessageBox::Question,tr("删除提示窗口"),tr("确认要删除吗?"));
                        messageBox.addButton(QMessageBox::Yes);
                        messageBox.addButton(QMessageBox::No);
                        messageBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
                        int btnValue=messageBox.exec();
                        if(btnValue==QMessageBox::Yes)
                        {
                            deleteItemInfoFromDb(item);
                            scene->removeGraphicsItem(scene->currentScenePos());
                            resetItemNumFromView(view);
                            if(item!=nullptr)
                            {
                                Controller::instance()->getOperatorInfo()->insertEvent(tr("删除设备"),QString(tr("分机号:%1,回路号:%2,地址号:%3,网络号:%4,设备删除成功")).arg(item->extNum())
                                                                                       .arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()));
                            }


                        }

                    }
                    else
                    {
                        QMessageBox messageBox(QMessageBox::Warning,tr("提示窗口"),tr("正在报警或者不是正常状态,不能删除!"));
                        messageBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
                        messageBox.exec();
                    }

                }

            }
        }

    });

    connect(m_treeView,&TreeView::updateTreeItemInfo,this,&ArchitePlanView::updateTreeItems);

    connect(m_editAction,&QAction::triggered,this,[=]()
    {
        GraphicsView *view= currentGraphicsView();
        if(view!=nullptr)
        {
            GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
            if(scene!=nullptr)
            {
                GraphicsItem *currentItem= dynamic_cast<GraphicsItem *> (scene->itemAt(scene->currentScenePos(),QTransform()));
                Q_ASSERT(m_itemSettingObj);
                QVariant currentIndex;
                QMetaObject::invokeMethod(m_itemSettingObj,"currentIconIndex",Q_RETURN_ARG(QVariant,currentIndex));
                if(currentItem!=nullptr)
                {
                    QMetaObject::invokeMethod(m_itemSettingObj,"setDeviceNum",Q_ARG(QVariant,currentItem->deviceNum()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setItemSize",Q_ARG(QVariant,currentItem->radius()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setExtNum",Q_ARG(QVariant,currentItem->extNum()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setLoopNum",Q_ARG(QVariant,currentItem->loopNum()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setAddrNum",Q_ARG(QVariant,currentItem->addrNum()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setNetworkNum",Q_ARG(QVariant,currentItem->networkNum()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setPowerAddr",Q_ARG(QVariant,currentItem->powerAddr()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setEquipmentModel",Q_ARG(QVariant,currentItem->equipmentModel()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setSysOfDevice",Q_ARG(QVariant,currentItem->sysOfDevice()));

                    QMetaObject::invokeMethod(m_itemSettingObj,"setChannelNum",Q_ARG(QVariant,QString::number(currentItem->channelNum())));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setAnalogType",Q_ARG(QVariant,currentItem->analogType()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setDeviceLocation",Q_ARG(QVariant,currentItem->deviceLocation()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setPeriodOfValidity",Q_ARG(QVariant,currentItem->periodOfValidity()));
                    // QMetaObject::invokeMethod(m_itemSettingObj,"setManufacturers",Q_ARG(QVariant,currentItem->manufacturers()));

                    QMetaObject::invokeMethod(m_itemSettingObj,"setOperator",Q_ARG(QVariant,currentItem->deviceOperator()));

                }
                else
                {
                    QMetaObject::invokeMethod(m_itemSettingObj,"clearItemInfo");
                }

                // m_itemSettingView->close();
                m_itemSettingView->show();

            }


        }
    });



    connect(m_itemTextVisiableAction,&QAction::triggered,this,[=]()
    {
        GraphicsView *view= currentGraphicsView();
        if(view!=nullptr)
        {
            GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
            if(scene!=nullptr)
            {
                GraphicsItem *currentItem= dynamic_cast<GraphicsItem *> (scene->itemAt(scene->currentScenePos(),QTransform()));
                if(currentItem!=nullptr)
                {
                    currentItem->setItemTextVisiable(m_itemTextVisiableAction->isChecked());
                }
            }
        }
    });
    connect(m_clearAction,&QAction::triggered,this,[=]()
    {
        GraphicsView *view= currentGraphicsView();
        if(view!=nullptr)
        {
            GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
            if(scene!=nullptr)
            {
                bool isHavingAlarm = scene->isHavingAlarms();
                if(!isHavingAlarm)
                {
                    QMessageBox messageBox(QMessageBox::Question,tr("清空提示窗口"),tr("确认要清空吗?"));
                    messageBox.addButton(QMessageBox::Yes);
                    messageBox.addButton(QMessageBox::No);
                    messageBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
                    int btnValue=messageBox.exec();


                    if(btnValue==QMessageBox::Yes)
                    {
                        foreach (QGraphicsItem*item, scene->getItemList())
                        {
                            GraphicsItem*gItem =dynamic_cast<GraphicsItem*>(item);

                            scene->removeItem(gItem);
                            if(gItem!=nullptr)
                            {
                                gItem->deleteLater();
                                gItem=nullptr;
                            }

                        }
                        scene->getItemList().clear();
                        Controller::instance()->getDataStore()->clearTypeItem();
                        QStandardItem *stdItem=   getItemFromView(view);
                        if(stdItem!=nullptr)
                        {
                            deleteInfoFromFloor(stdItem->text()) ;
                            Controller::instance()->getOperatorInfo()->insertEvent(tr("清空设备"),QString(tr("楼层:%1,清空设备成功")).arg(stdItem->text()));
                        }


                    }
                }

                else
                {
                    QMessageBox messageBox(QMessageBox::Warning,tr("提示窗口"),tr("有报警不能清空!"));
                    messageBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
                    messageBox.exec();
                }

            }

            if(view->getItemList().isEmpty())
            {
                Controller::instance()->getDataStore()->setItemNum(0);
            }

        }
    });
    connect(m_deleteSelectedAction,&QAction::triggered,this,[=]()
    {
        GraphicsView *view= currentGraphicsView();
        if(view!=nullptr)
        {
            GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
            if(scene!=nullptr)
            {
                QList<QGraphicsItem*>itemList = scene->selectedItems();
                bool isHavingAlarm = false;
                foreach (QGraphicsItem*item,itemList)
                {
                    GraphicsItem *graphicsItem = dynamic_cast<GraphicsItem*>(item);

                    if(graphicsItem!=nullptr)
                    {
                        if(graphicsItem->currentState()!="OK")
                        {
                            isHavingAlarm = true;
                            break;
                        }
                    }
                }

                if(!isHavingAlarm)
                {
                    QMessageBox messageBox(QMessageBox::Warning,tr("删除提示窗口"),tr("确认要删除吗?"));
                    messageBox.addButton(QMessageBox::Yes);
                    messageBox.addButton(QMessageBox::No);
                    messageBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
                    int btnValue=messageBox.exec();
                    if(btnValue==QMessageBox::Yes)
                    {
                        foreach (QGraphicsItem*item,itemList)
                        {
                            GraphicsItem *graphicsItem = dynamic_cast<GraphicsItem*>(item);

                            if(graphicsItem!=nullptr)
                            {

                                deleteItemInfoFromDb(graphicsItem);
                                scene->removeItem(graphicsItem);
                                scene->getItemList().removeOne(graphicsItem);
                                Controller::instance()->getDataStore()->deleteTypeItem(graphicsItem);
                            }
                            if(graphicsItem!=nullptr)
                            {
                                graphicsItem->deleteLater();
                                graphicsItem=nullptr;
                            }
                        }
                        resetItemNumFromView(view);
                        Controller::instance()->getOperatorInfo()->insertEvent(tr("删除选中设备"));
                    }

                }
                else
                {
                    QMessageBox messageBox(QMessageBox::Warning,tr("提示窗口"),tr("有报警存在,不能删除!"));
                    messageBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
                    messageBox.exec();
                }

            }
        }
    });

    connect(m_closeAction,&QAction::triggered,this,[=]()
    {
        m_graphicsItemSettingMenu->close();
    });



    connect(m_modeActionGroup,&QActionGroup::triggered,this,[=](QAction *action)
    {
        GraphicsView *view= currentGraphicsView();
        if(view!=nullptr)
        {
            if(action==m_handDragAction)
            {
                view->setDragMode(QGraphicsView::ScrollHandDrag);
            }
            else
            {
                view->setDragMode(QGraphicsView::RubberBandDrag);
            }
        }

    });


    connect(m_analogAlarmAction,&QAction::triggered,this,[=]()
    {
        m_analogAlarmView->close();
        m_analogAlarmView->show();
    });

    connect(m_maintenanceAction,&QAction::triggered,this,[=](){

        GraphicsView *view= currentGraphicsView();
        if(view!=nullptr)
        {
            GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
            if(scene!=nullptr)
            {
                GraphicsItem *item= dynamic_cast<GraphicsItem *> (scene->itemAt(scene->currentScenePos(),QTransform()));
                if(item!=nullptr)
                {
                    QObject *maintObj = m_maintenanceView->rootObject();
                    Q_ASSERT(maintObj);
                    QMetaObject::invokeMethod(maintObj,"setEquipmentCode",Q_ARG(QVariant,item->deviceNum()));
                    QMetaObject::invokeMethod(maintObj,"setBuildingName",Q_ARG(QVariant,item->buildingName()));
                    QMetaObject::invokeMethod(maintObj,"setFloor",Q_ARG(QVariant,item->floorOfDevice()));
                    QMetaObject::invokeMethod(maintObj,"setLocation",Q_ARG(QVariant,item->deviceLocation()));
                    QMetaObject::invokeMethod(maintObj,"setSysOfDevice",Q_ARG(QVariant,item->sysOfDevice()));
                    QMetaObject::invokeMethod(maintObj,"setOperator",Q_ARG(QVariant,item->deviceOperator()));

                }
                m_maintenanceView->close();
                m_maintenanceView->show();
            }
        }



    });

    connect(m_fitViewAction,&QAction::triggered,this,[=](){
        autoFitView(currentGraphicsView());
    });

    connect(Controller::instance()->getDataStore(),&DataStore::curLoopAddr,this,[=](const QString&loopNum,const QString&addrNum)
    {
        quint32 loopAndAddrValue = loopNum.toUInt()*1000+addrNum.toUInt();
        if(m_loopAddrExtHash.contains(loopAndAddrValue))
        {
            Controller::instance()->getDataStore()->extNum() = m_loopAddrExtHash.value(loopAndAddrValue);
        }

        if(m_loopAddrExtHash.contains(loopAndAddrValue))
        {
            QMetaObject::invokeMethod(m_itemSettingObj,"setEquipmentModel",Q_ARG(QVariant,m_loopAddrDeviceHash.value(loopAndAddrValue)));
        }

        if(m_loopAddrLocationHash.contains(loopAndAddrValue))
        {
            QMetaObject::invokeMethod(m_itemSettingObj,"setDeviceLocation",Q_ARG(QVariant,m_loopAddrLocationHash.value(loopAndAddrValue)));
        }

    });



    Q_ASSERT(m_itemSettingObj);
    connect(m_itemSettingObj,SIGNAL(setSize(qreal)),this,SLOT(setItemSize(qreal)));
    connect(m_itemSettingObj,SIGNAL(setIcon(QString)),this,SLOT(setItemIcon(QString)));
    connect(m_itemSettingObj,SIGNAL(setItemInfo(QString,QString)),this,SLOT(setItemInfoFromType(const QString , const QString &)));

    connect(m_itemSettingObj,SIGNAL(setItemsManufacturers(int,QString)),this,SLOT(setItemsManufacturers(int,QString)));
    connect(m_itemSettingObj,SIGNAL(setDeviceInstallTime(int,QString)),this,SLOT(setDeviceInstallTime(int,QString)));
    connect(m_itemSettingObj,SIGNAL(setItemsIcon(int,QString)),this,SLOT(setItemsIcon(int,QString)));
    connect(m_itemSettingObj,SIGNAL(setItemsPeriodOfValidity(int,QString)),this,SLOT(setItemsPeriodOfValidity(int,QString)));
    connect(m_itemSettingObj,SIGNAL(setItemsDeviceName(int,QString)),this,SLOT(setItemsEquipmentModel(int,QString)));


    connect(m_itemSettingObj,SIGNAL(startBatch()),this,SLOT(batchItems()));
    connect(m_itemSettingObj,SIGNAL(changeInfoFromFloor()),this,SLOT(changeItemsInfoFromFloor()));
    connect(m_itemSettingObj,SIGNAL(importExcelFile(QString)),this,SLOT(excelFileProcess(QString)));


    connect(m_itemSettingObj,SIGNAL(selectDevice(bool)),this,SLOT(setDeviceSelect(bool)));

    connect(m_itemSettingObj,SIGNAL(selectDevice(bool)),this,SLOT(setDeviceSelect(bool)));
    connect(m_itemSettingObj,SIGNAL(selectSys(bool)),this,SLOT(setSysSelect(bool)));
    connect(m_itemSettingObj,SIGNAL(selectChannel(bool)),this,SLOT(setChannelSelect(bool)));

    connect(m_itemSettingObj,SIGNAL(selectAnalog(bool)),this,SLOT(setAnalogSelect(bool)));
    connect(m_itemSettingObj,SIGNAL(selectSize(bool)),this,SLOT(setSizeSelect(bool)));


    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::addGlobalItem,this,[=](GlobalGraphicsItem*item)
    {
        if(item!=nullptr)
        {
            QStandardItem *standardItem =  m_treeView->addRootItem(item->buildName());
            if(standardItem!=nullptr)
            {
                m_globalToArchitePlanHash[item] = standardItem;
                Controller::instance()->getOperatorInfo()->insertEvent(tr("添加建筑物"),QString(tr("建筑物:%1,添加成功")).arg(item->buildName()));
            }



            m_sqliteManager->executeQuery(QString("insert into GlobalArchite values ('%1','%2','%3','%4','%5')").arg(item->buildName()).arg(item->personOnDuty()).arg(QString("%1,%2").arg(item->scenePos().x()).arg(item->scenePos().y())).arg(item->itemSize()).arg(item->iconName()));
        }

    });

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::deleteItems,this,[=]()
    {
        saveArchiteInfoToDb();
        saveGeneralLayoutInfo();
    });

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::deleteGlobalItem,this,[=](GlobalGraphicsItem*item)
    {
        if(item!=nullptr)
        {
            if(m_globalToArchitePlanHash.contains(item))
            {

                QModelIndex index= m_globalToArchitePlanHash.value(item)->index();
                m_treeView->deleteTreeItem(index);


            }
        }
    });

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::saveGeneralLayoutItems,this,&ArchitePlanView::saveGeneralLayoutInfo);

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::clearItem,this,[=]()
    {
        m_treeView->clearItem();
        Controller::instance()->getDataStore()->reInit();
    });

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::goToArchitePlan,this,[=](GlobalGraphicsItem*item)
    {
        if(item==nullptr)
        {
            return;
        }
        else
        {
            QStandardItem *stdItem = m_globalToArchitePlanHash[item];
            if(stdItem!=nullptr)
            {
                m_treeView->setItemExpanded(stdItem);
                m_tabWidget->setCurrentIndex(1);
                if(stdItem->hasChildren())
                {
                    m_treeView->clicked(stdItem->child(0)->index());
                }

            }
        }
    });

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::editItem,this,&ArchitePlanView::editGlobalItem);
    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::setBuildingName,this,[=](GlobalGraphicsItem *item,const QString &name)
    {
        QStandardItem *stdItem = m_globalToArchitePlanHash[item];
        if(stdItem!=nullptr)
        {
            stdItem->setText(name);
            QList<GraphicsView *>viewList=  viewsFromParentItem(stdItem);
            foreach (GraphicsView *view, viewList)
            {
                if(view!=nullptr)
                {
                    QList<QGraphicsItem*>graphicsItemList = view->getItemList();
                    foreach (QGraphicsItem*graphicsItem, graphicsItemList)
                    {
                        GraphicsItem*curItem = dynamic_cast<GraphicsItem*>(graphicsItem);

                        if(curItem!=nullptr)
                        {
                            curItem->buildingName() = name;
                        }

                    }
                }
            }

            JsonEdit::instance()->setRootName(stdItem->row(),name);
            JsonEdit::instance()->writeFile(c_jsonFilePath);

        }

        if(item!=nullptr)
        {
            QString curPos= QString("%1,%2").arg(item->scenePos().x()).arg(item->scenePos().y());
            m_sqliteManager->executeQuery(QString("update GlobalArchite set buildingName='%1' where pos='%2'").arg(name).arg(curPos));
        }

    });

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::setItemIcon,this,[=](GlobalGraphicsItem*item,const QString &icon)
    {
        if(item!=nullptr)
        {
            QString curPos= QString("%1,%2").arg(item->scenePos().x()).arg(item->scenePos().y());
            m_sqliteManager->executeQuery(QString("update GlobalArchite set iconName='%1' where pos='%2'").arg(icon).arg(curPos));
        }
    });

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::setItemSize,this,[=](GlobalGraphicsItem*item,qreal size)
    {
        if(item!=nullptr)
        {
            QString curPos= QString("%1,%2").arg(item->scenePos().x()).arg(item->scenePos().y());
            m_sqliteManager->executeQuery(QString("update GlobalArchite set size='%1' where pos='%2'").arg(size).arg(curPos));
        }
    });

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::setCurPersonOnDuty,this,[=](GlobalGraphicsItem*item,const QString &person)
    {
        if(item!=nullptr)
        {
            QString curPos= QString("%1,%2").arg(item->scenePos().x()).arg(item->scenePos().y());
            m_sqliteManager->executeQuery(QString("update GlobalArchite set personOnDuty='%1' where pos='%2'").arg(person).arg(curPos));
        }
    });

    connect(m_autoSwitchTimer,&CustomTimer::timeout,this,&ArchitePlanView::viewsAutoSwitch);


}

ArchitePlanView::~ArchitePlanView()
{

    m_autoSwitchTimer->stop();
    m_autoSwitchTimer->deleteLater();
    m_sqliteManager->close();
    m_sqliteManager->deleteLater();


    delete m_graphicsItemSettingMenu;
    delete m_analogValueQueryMenu;
    m_itemSettingView->close();
    m_analogAlarmView->close();
    m_maintenanceView->close();
    m_itemSettingView->deleteLater();
    m_analogAlarmView->deleteLater();
    m_maintenanceView->deleteLater();
}


void ArchitePlanView::createAlarm(const QString&extNum,const QString&loopNum,const QString&addressNum,const QString&networkNum,const QString &alarmTypeName,const QString& alarmState,const QString &alarmTime)
{

    GraphicsItem *item = itemFormInfo(extNum,loopNum,addressNum,networkNum);
    if(item!=nullptr)
    {
        createAlarm(item,alarmTypeName,alarmState,alarmTime);

    }

}


void ArchitePlanView::eliminateAlarm(GraphicsItem *item, const QString &alarmType, const QString &alarmReplyTime)
{
    if(item==nullptr)
    {
        return;
    }


    QString oldState = "";
    QString oldAlarmState =item->alarmState(alarmType);
    if(alarmType.startsWith(tr("模拟")))
    {
        oldState=  oldAlarmState;
    }
    else
    {
        if(oldAlarmState!="OK")
        {
            oldState = alarmType;
        }
    }

    emit eliminateAlarmFromTable(item,alarmType,alarmReplyTime);
    item->removeAlarmRecord(alarmType,alarmReplyTime);
    item->stopAnimations();

    disconnect(item,&GraphicsItem::moveToPos,nullptr,nullptr);
    disconnect(item,&GraphicsItem::sizeChanged,nullptr,nullptr);
    deleteAlarmText(item,oldState);
    Controller::instance()->getDataStore()->deleteTypeItem(oldState,item);

    GraphicsView*curView = Controller::instance()->getDataStore()->itemDisplayView(item);
    if(curView!=nullptr)
    {
        curView->removeGraphicsTextItem(oldState);
    }
    if(curView!=nullptr)
    {
        if(!curView->haveAlarmType(oldState))
        {
            deleteAlarmWidget(oldState,curView);
        }
        if(!curView->haveAnyAlarm())
        {
            if(m_alarmViewList.contains(curView))
            {
                m_alarmViewList.removeOne(curView);
            }

            QStandardItem *parentItem =   getParnentItemFromView(curView);
            if(parentItem!=nullptr)
            {
                GlobalGraphicsItem*globalGraphicsItem=  m_globalToArchitePlanHash.key(parentItem);
                if(globalGraphicsItem!=nullptr)
                {
                    globalGraphicsItem->setCurAnimationRunState(false);
                    if(globalGraphicsItem->animalIsRunning())
                    {
                        globalGraphicsItem->startAnimal(false);


                    }
                }
            }
        }
        updateAlarmWidget(currentGraphicsView());
    }


    filterAlarm(item);
    emit updateFirstAlarmView();


}

void ArchitePlanView::eliminateAlarm(const QString &extNum, const QString &loopNum, const QString &addrNum,const QString &networkNum, const QString &alarmType)
{
    GraphicsItem *currentItem = itemFormInfo(extNum, loopNum, addrNum,networkNum);
    if(currentItem!=nullptr)
    {
        QString replyTime=  QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        eliminateAlarm(currentItem,alarmType,replyTime);
    }

}

void ArchitePlanView::generateAlarm(const QString &alarmTypeName, const QString &alarmTime, GraphicsItem *item,  bool isAnalog)
{
    if(item!=nullptr)
    {
        QString state = item->alarmState(alarmTypeName);
        if(state!="OK"&&!state.isEmpty())
        {
            return;
        }
        QString curAlarmType = "";
        if(isAnalog)
        {
            curAlarmType = tr("模拟")+alarmTypeName;
        }
        else
        {
            curAlarmType = alarmTypeName;
        }
        item->setAlarmRecord(curAlarmType,alarmTime,alarmTypeName);
        Controller::instance()->getDataStore()->insertTypeItem(alarmTypeName,item);
        filterAlarm(item);
        emit alarmItem(item,curAlarmType);
        //emit alarmHappend(alarmTypeName);
    }
}

void ArchitePlanView::insertAlarmWidget(const QString &type, GraphicsView *view)
{
    if(!m_alarmWidgetHash.value(type).contains(view))
    {
        m_alarmWidgetHash[type].push_back(view);
    }
}

void ArchitePlanView::deleteAlarmWidget(const QString &type,GraphicsView *view)
{
    m_alarmWidgetHash[type].removeOne(view);
}

void ArchitePlanView::clearAlarmWidget()
{
    m_alarmWidgetHash.clear();
}

void ArchitePlanView::clearAlarmWidget(const QString &type)
{
    m_alarmWidgetHash[type].clear();
}

void ArchitePlanView::firstFireAlarm()
{
    findFireAlarm(0);
}

void ArchitePlanView::lastFireAlarm()
{
    findFireAlarm(-1);
}

void ArchitePlanView::currentGraphicsViewZoom(bool isZoomIn)
{
    GraphicsView*graphicsView=dynamic_cast<GraphicsView*> (m_stackedWidget->currentWidget()) ;
    if(graphicsView!=nullptr)
    {
        if(isZoomIn)
        {
            graphicsView->zoomIn();
        }
        else
        {
            graphicsView->zoomOut();
        }
    }

}

void ArchitePlanView::initWidget()
{

    m_treeView = new TreeView(this);
    m_stackedWidget = new QStackedWidget(this);

    m_tabWidget = new QTabWidget(this);
    m_globalGraphicsView = new GlobalGraphicsView(this);
    m_sysArchitePlanView = new SysArchitePlanView(this);
    m_autoSwitchTimer = new CustomTimer();

    //m_startArchiteViewTimer= new CustomTimer;

    //m_excelManager = new ExcelManager(this);
    m_treeView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    m_treeView->setMaximumWidth(800);
    m_treeView->setMinimumWidth(150);
    m_stackedWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_stackedWidget->setStyleSheet("QWidget{border:1px solid black}");
    m_treeView->setStyleSheet("QTreeView{border:1px solid black}");



    QSplitter *splitter = new QSplitter(this);
    splitter->setChildrenCollapsible(false);
    QVBoxLayout*globalVLayout = new QVBoxLayout;
    splitter->addWidget(m_stackedWidget);
    splitter->addWidget(m_treeView);

    m_stackedWidget->sizePolicy().setHorizontalStretch(90);
    m_treeView->sizePolicy().setHorizontalStretch(10);
    //splitter->setStretchFactor(0, 20);
    // splitter->setStretchFactor(1, 80);

    m_tabWidget->addTab(m_globalGraphicsView ,tr("总平面布局图"));
    m_tabWidget->addTab(splitter,tr("建筑平面图"));
    m_tabWidget->addTab(m_sysArchitePlanView,tr("系统图"));

    globalVLayout->addWidget(m_tabWidget);
    globalVLayout->setContentsMargins(QMargins(0,0,0,0));
    setLayout(globalVLayout);
    Controller::instance()->setSysArchitePlanView(m_sysArchitePlanView);
    QString dbName = QCoreApplication::applicationDirPath()+"/architeInfo.db";
    m_architeInfoDbName = dbName;
    m_sqliteManager = new SqlManager;

    m_sqliteManager->setDataBase("QSQLITE","info", "",
                                 "","",dbName,8888);
    m_sqliteManager->open();
    if(m_sqliteManager->isOpen())
    {
        QStringList tableNameList = m_sqliteManager->getTables();
        QStringList itemInfoTableList;
        itemInfoTableList<<"extNum text" << "addrNum text" << "loopNum text" << "networkNum text"<<"powerAddr text" <<"buildingName text" << "currentState text"
                        << "deviceLocation text" << "deviceNum text" << "equipmentModel text" << "floorOfDevice text"
                        << "iconName text"<<"manufacturers text"<<"deviceInstallTime text" <<"periodOfValidity text"<<"pos text"<<"size double"
                        << "sysOfDevice text"<<"operator text"<<"analogType text"<<"channelNum int";
        m_itemInfoTableSize = itemInfoTableList.size();
        QStringList globalArchiteList;
        globalArchiteList << "buildingName text"<<"personOnDuty text"<< "pos text"<<"size double"<<"iconName text";
        m_globalArchiteTableSize= globalArchiteList.size();

        if(!tableNameList.contains("ItemInfo"))
        {
            m_sqliteManager->executeQuery(QString("create table ItemInfo(%1)").arg(itemInfoTableList.join(",")));
        }

        if(!tableNameList.contains("GlobalArchite"))
        {
            m_sqliteManager->executeQuery(QString("create table GlobalArchite(%1)").arg(globalArchiteList.join(",")));
        }
    }

    m_graphicsItemSettingMenu = new QMenu;
    m_modeSelectMenu = new QMenu(tr("模式选择"),m_graphicsItemSettingMenu);
    m_deleteAction = new QAction(tr("删除"),m_graphicsItemSettingMenu);
    m_editAction = new QAction(tr("编辑"),m_graphicsItemSettingMenu);
    m_clearAction = new QAction(tr("清空"),m_graphicsItemSettingMenu);
    m_deleteSelectedAction = new QAction(tr("删除选中"),m_graphicsItemSettingMenu);
    m_closeAction= new QAction(tr("关闭"),m_graphicsItemSettingMenu);
    m_analogAlarmAction = new QAction(tr("报警模拟"),m_graphicsItemSettingMenu);
    m_maintenanceAction = new QAction(tr("设备维保"),m_graphicsItemSettingMenu);
    m_handDragAction = new QAction(tr("手动拖拽模式"),m_modeSelectMenu);
    m_rubberBandDragAction = new QAction(tr("橡皮筋模式"),m_modeSelectMenu);
    m_itemTextVisiableAction = new QAction(tr("文字可见"),m_graphicsItemSettingMenu);
    m_fitViewAction = new QAction(tr("最佳视图"),m_graphicsItemSettingMenu);
    // m_xAlignmentAction = new QAction(tr("行对齐"),m_graphicsItemSettingMenu);
    //m_yAlignmentAction = new QAction(tr("列对齐"),m_graphicsItemSettingMenu);
    m_analogValueQueryAction = new QAction(tr("模拟量查询"),m_graphicsItemSettingMenu);
    m_analogValueQueryMenu = new QMenu;
    m_modeActionGroup = new QActionGroup(this);
    m_modeSelectMenu->addAction(m_handDragAction);
    m_modeSelectMenu->addAction(m_rubberBandDragAction);
    m_modeActionGroup->addAction(m_rubberBandDragAction);
    m_modeActionGroup->addAction(m_handDragAction);
    m_analogValueQueryAction->setMenu(m_analogValueQueryMenu);
    m_modeActionGroup->setExclusive(true);
    m_handDragAction->setCheckable(true);
    m_rubberBandDragAction->setCheckable(true);
    m_itemTextVisiableAction->setCheckable(true);
    m_handDragAction->setChecked(true);
    m_graphicsItemSettingMenu->addMenu(m_modeSelectMenu);
    m_itemSettingView = new QQuickView;
    m_itemSettingView->setSource(QUrl("qrc:/qml/itemSetting/GraphicsItemEditor.qml"));
    m_itemSettingView->setTitle(tr("设备信息设置界面"));

    m_itemSettingView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);

    m_itemSettingView->setResizeMode(QQuickView::SizeRootObjectToView);

    m_itemSettingView->rootContext()->setContextProperty("ArchitePlanView",this);
    m_analogAlarmView = new QQuickView;
    m_analogAlarmView->setSource(QUrl("qrc:/qml/itemSetting/AnalogAlarmItem.qml"));
    m_analogAlarmView->setTitle(tr("报警模拟界面"));
    m_analogAlarmObj = m_analogAlarmView->rootObject();
    m_analogAlarmView->rootContext()->setContextProperty("ArchitePlanView",this);

    //    m_analogAlarmView->setMinimumSize(QSize(480,480));
    //    m_analogAlarmView->setMaximumSize(QSize(480,480));
    m_analogAlarmView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
    m_maintenanceView = new QQuickView;
    m_maintenanceView->setSource(QUrl("qrc:/qml/infoSetting/MaintenanceInfo.qml"));
    m_maintenanceView->setTitle(tr("设备维保"));
    m_maintenanceObj = m_maintenanceView->rootObject();
    //    m_maintenanceView->setMinimumSize(QSize(900,560));
    //    m_maintenanceView->setMaximumSize(QSize(900,560));
    m_maintenanceView->setFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
    m_itemSettingObj= m_itemSettingView->rootObject();
    m_graphicsItemSettingMenu->addAction(m_itemTextVisiableAction);
    m_graphicsItemSettingMenu->addAction(m_editAction);
    m_graphicsItemSettingMenu->addAction(m_fitViewAction);
    m_graphicsItemSettingMenu->addAction(m_analogAlarmAction);
    m_graphicsItemSettingMenu->addAction(m_maintenanceAction);
    m_graphicsItemSettingMenu->addAction(m_analogValueQueryAction);
    // m_graphicsItemSettingMenu->addAction(m_xAlignmentAction);
    //m_graphicsItemSettingMenu->addAction(m_yAlignmentAction);
    m_graphicsItemSettingMenu->addAction(m_deleteAction);
    m_graphicsItemSettingMenu->addAction(m_deleteSelectedAction);
    m_graphicsItemSettingMenu->addAction(m_clearAction);

    m_graphicsItemSettingMenu->addAction(m_closeAction);

    // connect(m_showExtOnlineAction,&QAction::triggered,this,&ArchitePlanView::showExtNumState);

    //m_startArchiteViewTimer->setSingleShot(true);


    connect(m_treeView,&TreeView::treeIndex,this,[=](QStandardItem*item)
    {
        QMap<QStandardItem*,int>map= m_treeView->getTreeIndexMap();
        if(item!=nullptr)
        {
            int page = map[item];
            if(m_widgetHash.value(page)==nullptr)
            {
                GraphicsView *widget = new GraphicsView(this);
                GraphicsScene*curScene=  dynamic_cast<GraphicsScene*>(widget->currentGraphicsScene());
                if(curScene!=nullptr)
                {
                    connect(curScene,&GraphicsScene::addOneItem,this,[=](GraphicsItem *item)
                    {
                        QList<GraphicsView*>viewList=  getWidgetHash().values();
                        bool isReplaced=false;
                        foreach(GraphicsView*view,viewList)
                        {
                            if(view!=nullptr)
                            {
                                QList<QGraphicsItem *> curItemList= view->getItemList();

                                foreach(QGraphicsItem *gItem,curItemList)
                                {
                                    GraphicsItem*curItem = dynamic_cast<GraphicsItem*>(gItem);
                                    if(curItem!=nullptr)
                                    {
                                        if(curItem!=item&&curItem->addrNum()==item->addrNum()&&
                                                curItem->loopNum()==item->loopNum()
                                                &&curItem->extNum()==item->extNum()&&
                                                curItem->networkNum()==item->networkNum()
                                                &&curItem->powerAddr()==item->powerAddr())
                                        {
                                            QMessageBox messageBox(QMessageBox::Question,tr("信息提示"),tr("已经存在，确认要替换吗"));
                                            messageBox.addButton(QMessageBox::Yes);
                                            messageBox.addButton(QMessageBox::No);
                                            messageBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
                                            int buttonValue=messageBox.exec();
                                            if(buttonValue==QMessageBox::Yes)
                                            {
                                                if(item!=nullptr)
                                                {
                                                    GraphicsScene*currentScene=dynamic_cast<GraphicsScene*>(view->currentGraphicsScene());
                                                    if(currentScene!=nullptr)
                                                    {
                                                        currentScene->removeGraphicsItem(curItem);
                                                    }

                                                    saveItemInfoToDb(item);
                                                    isReplaced = true;
                                                    Controller::instance()->getOperatorInfo()->insertEvent(tr("添加设备"),QString(tr("分机号:%1,回路号:%2,地址号:%3,网络号:%4,设备添加成功")).arg(item->extNum())
                                                                                                           .arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()));
                                                }

                                            }
                                            else
                                            {
                                                curScene->removeGraphicsItem(item);
                                            }
                                        }

                                    }
                                }
                            }
                        }

                        if(!isReplaced)
                        {
                            if(item!=nullptr)
                            {
                                Controller::instance()->getOperatorInfo()->insertEvent(tr("添加设备"),QString(tr("分机号:%1,回路号:%2,地址号:%3,网络号:%4,设备添加成功")).arg(item->extNum())
                                                                                       .arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()));
                            }

                        }
                    });


                    connect(curScene,&GraphicsScene::itemSetting,this,[=](GraphicsItem *item)
                    {
                        QList<GraphicsView*>viewList=  getWidgetHash().values();
                        foreach(GraphicsView*view,viewList)
                        {
                            if(view!=nullptr)
                            {
                                QList<QGraphicsItem *> curItemList= view->getItemList();
                                if(item!=nullptr)
                                {
                                    foreach(QGraphicsItem *gItem,curItemList)
                                    {
                                        GraphicsItem*curItem = dynamic_cast<GraphicsItem*>(gItem);
                                        if(curItem!=nullptr)
                                        {
                                            if(curItem!=item&&curItem->addrNum()==item->addrNum()&&
                                                    curItem->loopNum()==item->loopNum()
                                                    &&curItem->extNum()==item->extNum()&&
                                                    curItem->networkNum()==item->networkNum()
                                                    &&curItem->powerAddr()==item->powerAddr())
                                            {
                                                GraphicsScene*currentScene=dynamic_cast<GraphicsScene*>(view->currentGraphicsScene());
                                                QMessageBox messageBox;
                                                messageBox.setWindowFlags(Qt::WindowStaysOnTopHint);
                                                int buttonValue=messageBox.information(this,tr("信息提示"),tr("已经存在，确认要替换吗"),QMessageBox::Yes,QMessageBox::No);

                                                if(buttonValue==QMessageBox::Yes)
                                                {
                                                    if(currentScene!=nullptr)
                                                    {
                                                        currentScene->removeGraphicsItem(curItem);
                                                    }
                                                    saveItemInfoToDb(item);

                                                }
                                                else
                                                {



                                                    QString curValue=  currentScene->itemOldInfo(item);
                                                    QStringList curStrList=   curValue.split(",");
                                                    if(curStrList.size()>=5)
                                                    {
                                                        item->extNum()=curStrList.at(0);
                                                        item->loopNum()=curStrList.at(1);
                                                        item->addrNum()=curStrList.at(2);
                                                        item->networkNum()=curStrList.at(3);
                                                        item->powerAddr()=curStrList.at(4);
                                                        item->deviceNum() = QString("%1-%2").arg(item->loopNum()).arg(item->addrNum());
                                                        item->update();


                                                    }
                                                    curScene->clearItemOldInfo();
                                                    // curScene->removeGraphicsItem(item);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }




                    });
                }
                m_widgetHash[page]=widget;
                m_stackedWidget->addWidget(widget);
                m_itemToViewHash[item] = widget;
            }
        }

    });
    connect(m_treeView,&TreeView::doubleClicked,this,[=](const QModelIndex&index)
    {

        int page=0;
        QStandardItemModel*model =dynamic_cast<QStandardItemModel*>(m_treeView->model());
        if(model==nullptr)
        {
            return;
        }
        QStandardItem *item= model->itemFromIndex(index);

        if(item==nullptr)
        {
            return ;
        }
        if(item->parent()==nullptr)
        {
            return;
        }
        QMap<QStandardItem*,int>itemMap;
        itemMap = m_treeView->getTreeIndexMap();
        page =itemMap.value(item);
        if(m_widgetHash.value(page)!=nullptr)
        {
            GraphicsView *currentView = dynamic_cast<GraphicsView *>(m_widgetHash.value(page));
            if(currentView!=nullptr)
            {
                m_stackedWidget->setCurrentWidget(currentView);
            }


        }
    });

    connect(m_treeView,&TreeView::clearIndex,this,[=]()
    {
        int count = m_widgetHash.size();
        for(int i=0;i<count;i++)
        {
            QWidget *widget = m_widgetHash.values().at(i);
            if(widget!=nullptr)
            {
                m_stackedWidget->removeWidget(widget);
            }

        }
        m_widgetHash.clear();
        m_globalGraphicsView->currentScene()->clearGraphicsItem();
        emit noPage();
        m_sqliteManager->executeQuery("delete from ItemInfo");
        m_sqliteManager->executeQuery("delete from GlobalArchite");
    });

    connect(m_treeView,&TreeView::deleteParentIndex,this,&ArchitePlanView::deleteViewFromItem);
    connect(m_treeView,&TreeView::deleteChildIndex,this,[=](QStandardItem*item){
        QMap<QStandardItem*,int>itemMap;
        itemMap = m_treeView->getTreeIndexMap();
        int chileItemPage = itemMap.value(item);
        GraphicsView*childWidget = m_widgetHash.value(chileItemPage);
        if(childWidget!=nullptr)
        {
            m_stackedWidget->removeWidget(childWidget);
            deleteAlarmWidget(childWidget);
        }
    });
    connect(m_treeView,&TreeView::insertAnchPixmap,this,[=](QStandardItem*item,const QString &fileName)
    {
        QMap<QStandardItem*,int>itemMap;
        itemMap = m_treeView->getTreeIndexMap();
        int page =itemMap[item];
        GraphicsView*widget = m_widgetHash.value(page);

        if(widget!=nullptr)
        {
            widget->loadPixmap(fileName);
            widget->updateSvg();
        }

    });

    connect(m_stackedWidget,&QStackedWidget::currentChanged,this,[=](int index)
    {

        QList<GraphicsView*>viewList = m_alarmWidgetHash[m_currentAlarmType];
        int count = viewList.size();

        GraphicsView *view = dynamic_cast<GraphicsView *>(m_stackedWidget->widget(index));
        int currentIndex= viewList.indexOf(view);
        if(view!=nullptr)
        {
            if(view->haveAnyAlarm())
            {
                QList<QGraphicsItem *>alarmItemList=  view->getItemList();
                foreach(QGraphicsItem *curItem,alarmItemList)
                {
                    GraphicsItem*curGraphicsItem = dynamic_cast<GraphicsItem*>(curItem);
                    if(curGraphicsItem!=nullptr)
                    {
                        if(curGraphicsItem->currentState()!="OK"&&m_typeList.contains(curGraphicsItem->alarmType()))
                        {
                            startAlarmAnimation(curGraphicsItem);
                        }
                    }
                }

            }
        }
        if(m_treeView!=nullptr)
        {
            QStandardItem *stdItem = getItemFromView(view);
            if(stdItem!=nullptr)
            {

                if(stdItem->index()!=m_treeView->currentIndex())
                {
                    m_treeView->setCurrentIndex(stdItem->index());
                }
            }
        }

        if(count>1)
        {
            if(count>2)
            {

                if(currentIndex>=1&&currentIndex<count-1)
                {
                    emit normalPage();
                }
                else if(currentIndex<1)
                {
                    emit toFirstPage();
                }
                else if(currentIndex==count-1)
                {
                    emit toLastPage();
                }
            }
            else
            {
                if(currentIndex<1)
                {
                    emit toFirstPage();
                }
                else
                {
                    emit toLastPage();
                }
            }

        }
        else
        {
            emit noPage();
        }

    });

    connect(m_itemSettingView,&QQuickView::visibleChanged,this,[=](bool arg)
    {
        if(!arg)
        {
            m_itemSettingView->close();
            QList<GraphicsView*>viewList=  m_widgetHash.values();
            foreach(GraphicsView*curView,viewList)
            {
                if(curView!=nullptr)
                {
                    GraphicsScene*curScene=dynamic_cast<GraphicsScene*>(curView->currentGraphicsScene());
                    if(curScene!=nullptr)
                    {
                        curScene->checkSettingItem();
                    }
                }


            }



        }

    });

    connect(m_treeView,&TreeView::toGlobalGraphicsView,this,[=](QStandardItem*item)
    {
        GlobalGraphicsItem*globalGraphicsItem=  m_globalToArchitePlanHash.key(item);
        if(globalGraphicsItem!=nullptr)
        {
            m_tabWidget->setCurrentIndex(0);
        }

    });

    //    connect(m_xAlignmentAction,&QAction::triggered,this,[=]()
    //    {
    //        qreal yMin = 10000.000;
    //        GraphicsView *graphicsView=currentGraphicsView();
    //        if(graphicsView!=nullptr)
    //        {
    //           if(graphicsView->scene()!=nullptr)
    //           {
    //               QList<QGraphicsItem*>itemList = graphicsView->scene()->selectedItems();
    //               foreach (QGraphicsItem*curItem, itemList)
    //               {
    //                   GraphicsItem*item = dynamic_cast<GraphicsItem*>(curItem);
    //                   if(item!=nullptr)
    //                   {
    //                     yMin= qMin(yMin,item->pos().y());
    //                   }
    //               }
    //           }

    //           QList<QGraphicsItem*>itemList = graphicsView->scene()->selectedItems();
    //           foreach (QGraphicsItem*curItem, itemList)
    //           {
    //               GraphicsItem*item = dynamic_cast<GraphicsItem*>(curItem);
    //               if(item!=nullptr)
    //               {
    //                 item->setY(yMin);
    //               }
    //           }
    //        }
    //    });

    //    connect(m_yAlignmentAction,&QAction::triggered,this,[=]()
    //    {
    //        qreal xMin = 10000.000;
    //        GraphicsView *graphicsView=currentGraphicsView();
    //        if(graphicsView!=nullptr)
    //        {
    //           if(graphicsView->scene()!=nullptr)
    //           {
    //               QList<QGraphicsItem*>itemList = graphicsView->scene()->selectedItems();
    //               foreach (QGraphicsItem*curItem, itemList)
    //               {
    //                   GraphicsItem*item = dynamic_cast<GraphicsItem*>(curItem);
    //                   if(item!=nullptr)
    //                   {
    //                     xMin= qMin(xMin,item->pos().x());
    //                   }
    //               }
    //           }

    //           QList<QGraphicsItem*>itemList = graphicsView->scene()->selectedItems();
    //           foreach (QGraphicsItem*curItem, itemList)
    //           {
    //               GraphicsItem*item = dynamic_cast<GraphicsItem*>(curItem);
    //               if(item!=nullptr)
    //               {
    //                 item->setX(xMin);
    //               }
    //           }
    //        }
    //    });

    connect(m_tabWidget,&QTabWidget::currentChanged,this,[=](int index)
    {
        if(index==2)
        {

            if(!m_firstSetSysInfo)
            {
                m_sysArchitePlanView->setSysArchitePlanInfo();
                m_firstSetSysInfo = true;
            }

        }
        else if(index==0)
        {
            if(m_globalGraphicsView!=nullptr)
            {
                GlobalGraphicsScene *scene=  m_globalGraphicsView->currentScene();
                if(scene!=nullptr)
                {
                    QList<GlobalGraphicsItem*> itemList=   scene->currentItemList();
                    foreach(GlobalGraphicsItem*curItem,itemList)
                    {
                        if(curItem!=nullptr)
                        {
                            if(curItem->curAnimationRunState())
                            {
                                curItem->startAnimal(true);
                            }
                        }
                    }
                }
            }

        }
        emit tabIndex(index);
    });

    connect(m_analogAlarmObj,SIGNAL(creatAnalogAlarm(QString)),this,SIGNAL(getAnalogAlarm(QString)));

    connect(m_analogAlarmObj,SIGNAL(eliminateCurrentAlarm(QString)),this,SIGNAL(eliminateAnalogAlarm(QString)));
}

void ArchitePlanView::showMenu(const QPoint &point)
{

    m_analogValueQueryMenu->clear();
    GraphicsView *view = currentGraphicsView();
    if(view!=nullptr)
    {
        GraphicsScene *scene=dynamic_cast<GraphicsScene *> (view->currentGraphicsScene());
        if(scene!=nullptr)
        {
            UserManager::UserRight userRight= Controller::instance()->getUserRight();
            QList<QGraphicsItem*>itemList = scene->getItemList();
            QGraphicsItem*selectItem=  scene->itemAt(scene->currentScenePos(),QTransform());
            GraphicsItem *selectGrraphicsItem=dynamic_cast<GraphicsItem *>(selectItem);
            if(selectGrraphicsItem!=nullptr)
            {
                m_itemTextVisiableAction->setEnabled(true);
                m_itemTextVisiableAction->setChecked(selectGrraphicsItem->itemTextIsVisiable());
                if(selectGrraphicsItem->analogType()==tr("无"))
                {
                    m_analogValueQueryAction->setEnabled(false);

                }
                else
                {
                    m_analogValueQueryAction->setEnabled(true);
                    if(m_analogValueQueryMenu->isEmpty())
                    {
                        QAction *curAction = new QAction(QString(tr("当前设备")),m_analogValueQueryMenu);
                        m_analogValueQueryMenu->addAction(curAction);
                        connect(curAction,&QAction::triggered,this,[=]()
                        {
                            QMessageBox messageBox(QMessageBox::Question,tr("查询模拟量带子网号确认"),tr("是否带子网号查询模拟量"));
                            messageBox.addButton(QMessageBox::Yes);
                            messageBox.addButton(QMessageBox::No);
                            messageBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
                            int curInfo=messageBox.exec();
                            if(curInfo==QMessageBox::Yes)
                            {
                                emit sendAnalogValue(selectGrraphicsItem,0);

                            }
                            else
                            {
                                emit sendAnalogNoNetwork(selectGrraphicsItem,0);
                            }

                        });

                        int curChannelNum = selectGrraphicsItem->channelNum();

                        if(curChannelNum>0)
                        {
                            for(int i=0;i<curChannelNum;i++)
                            {

                                QAction *action = new QAction(QString(tr("%1通道")).arg(i+1),m_analogValueQueryMenu);
                                m_analogValueQueryMenu->addAction(action);
                                connect(action,&QAction::triggered,this,[=]()
                                {
                                    QMessageBox messageBox(QMessageBox::Question,tr("查询模拟量带子网号确认"),tr("是否带子网号查询模拟量"));
                                    messageBox.addButton(QMessageBox::Yes);
                                    messageBox.addButton(QMessageBox::No);
                                    messageBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);
                                    int curInfo=messageBox.exec();

                                    if(curInfo==QMessageBox::Yes)
                                    {

                                        emit sendAnalogValue(selectGrraphicsItem,quint8(i+1));
                                    }
                                    else
                                    {
                                        emit sendAnalogNoNetwork(selectGrraphicsItem,quint8(i+1));
                                    }


                                });
                            }
                        }
                    }

                }

            }
            else
            {
                m_itemTextVisiableAction->setEnabled(false);
                m_itemTextVisiableAction->setChecked(false);
                m_analogValueQueryAction->setEnabled(false);
            }

            if(userRight==UserManager::Super)
            {

                if(!itemList.isEmpty())
                {
                    if(itemList.contains(selectItem))
                    {
                        if(ArchitePlanView::itemLimit())
                        {
                            m_deleteAction->setEnabled(true);
                        }
                        else
                        {
                            m_deleteAction->setEnabled(false);
                        }

                        m_maintenanceAction->setEnabled(true);

                    }
                    else
                    {
                        m_deleteAction->setEnabled(false);
                        m_maintenanceAction->setEnabled(false);
                    }
                    if(scene->selectedItems().isEmpty())
                    {
                        m_deleteSelectedAction->setEnabled(false);
                    }
                    else
                    {
                        if(ArchitePlanView::itemLimit())
                        {
                            m_deleteSelectedAction->setEnabled(true);
                        }
                        else
                        {
                            m_deleteSelectedAction->setEnabled(false);
                        }

                    }

                    if(ArchitePlanView::itemLimit())
                    {
                        m_clearAction->setEnabled(true);
                    }
                    else
                    {
                        m_clearAction->setEnabled(false);
                    }

                    m_analogAlarmAction->setEnabled(true);



                }
                else
                {
                    m_deleteAction->setEnabled(false);
                    m_clearAction->setEnabled(false);
                    m_deleteSelectedAction->setEnabled(false);
                    m_analogAlarmAction->setEnabled(false);
                    m_maintenanceAction->setEnabled(false);
                }
                m_editAction->setEnabled(true);

            }
            else
            {
                m_deleteAction->setEnabled(false);
                m_editAction->setEnabled(false);
                m_clearAction->setEnabled(false);
                m_deleteSelectedAction->setEnabled(false);
                m_analogAlarmAction->setEnabled(false);
                m_maintenanceAction->setEnabled(false);

            }
            m_graphicsItemSettingMenu->exec(point);
        }

    }


}

void ArchitePlanView::saveArchiteInfo()
{
    updateTreeItems();
    saveArchiteInfoToDb();
}

void ArchitePlanView::autoFitView(QGraphicsView *view)
{

    GraphicsView *currentView = dynamic_cast<GraphicsView *>(view);
    if(currentView !=nullptr)
    {
        m_stackedWidget->setCurrentWidget(currentView);

        QRectF currentRectF = currentView->scene()->sceneRect();
        currentView->fitInView(currentRectF.adjusted(0,0,currentView->pos().x(),view->pos().y()),Qt::KeepAspectRatio);
        currentView->scaleValueChanged(currentView->transform().m11());

    }

}

void ArchitePlanView::toAlarmView()
{
    foreach (GraphicsView*view, m_alarmViewList)
    {
        if(view!=nullptr)
        {
            if(view->haveAlarmType(tr("火警"))||view->haveAlarmType(tr("启动")))
            {
                if(m_tabWidget->count()>=2)
                {
                    if(m_tabWidget->currentIndex()!=1)
                    {
                        m_tabWidget->setCurrentIndex(1);
                    }
                }
                autoFitView(view);
                break;
            }
        }
    }
}

QStandardItem *ArchitePlanView::getParnentItemFromView(GraphicsView *view)
{
    if(view!=nullptr)
    {
        QStandardItem *stdItem=  m_itemToViewHash.key(view);
        if(stdItem!=nullptr)
        {
            QStandardItem*parentItem = stdItem->parent();
            return parentItem;

        }
        else
        {
            return nullptr;
        }

    }
    else
    {
        return nullptr;
    }
}

void ArchitePlanView::saveArchiteInfoToDb()
{
    m_sqliteManager->executeQuery("delete from ItemInfo");
    QList<int> keyValueList= m_widgetHash.keys();
    foreach (int value, keyValueList)
    {
        GraphicsView *view=  m_widgetHash.value(value);
        if(view!=nullptr)
        {
            QList<QVariant>valueList,extNumList,addrNumList,loopNumList,networkNumList,powerAddrList,buildingNameList,
                    currentStateList,deviceLocationList,deviceNumList,equipmentModelList,floorOfDeviceList,
                    iconNameList,manufacturersList,deviceInstallTimerList,periodOfValidityList,posList,sizeList,sysOfDeviceList,operatorList,analogTypeList,channelNumList;
            QList<QGraphicsItem *> itemList= view->getItemList();

            foreach (QGraphicsItem *item, itemList)
            {
                GraphicsItem*currentItem = dynamic_cast<GraphicsItem*>(item);
                if(currentItem!=nullptr)
                {
                    extNumList.push_back( currentItem->extNum());
                    addrNumList.push_back(currentItem->addrNum());
                    loopNumList.push_back(currentItem->loopNum());
                    networkNumList.push_back(currentItem->networkNum());
                    powerAddrList.push_back(currentItem->powerAddr());
                    buildingNameList.push_back(currentItem->buildingName());
                    currentStateList.push_back(currentItem->currentState());
                    deviceLocationList.push_back(currentItem->deviceLocation());
                    deviceNumList.push_back(currentItem->deviceNum());
                    equipmentModelList.push_back(currentItem->equipmentModel());
                    floorOfDeviceList.push_back(currentItem->floorOfDevice());
                    iconNameList.push_back(currentItem->iconName());
                    manufacturersList.push_back(currentItem->manufacturers());
                    deviceInstallTimerList.push_back(currentItem->deviceInstallTime());
                    periodOfValidityList.push_back(currentItem->periodOfValidity());
                    posList.push_back(QString("%1,%2").arg(currentItem->scenePos().x()).arg(currentItem->scenePos().y()));
                    sizeList.push_back(currentItem->radius());
                    sysOfDeviceList.push_back(currentItem->sysOfDevice());
                    operatorList.push_back(currentItem->deviceOperator());
                    analogTypeList.push_back(currentItem->analogType());
                    channelNumList.push_back(currentItem->channelNum());
                }
            }

            valueList.push_back(extNumList);
            valueList.push_back(addrNumList);
            valueList.push_back(loopNumList);
            valueList.push_back(networkNumList);
            valueList.push_back(powerAddrList);
            valueList.push_back(buildingNameList);
            valueList.push_back(currentStateList);
            valueList.push_back(deviceLocationList);
            valueList.push_back(deviceNumList);
            valueList.push_back(equipmentModelList);
            valueList.push_back(floorOfDeviceList);
            valueList.push_back(iconNameList);
            valueList.push_back(manufacturersList);
            valueList.push_back(deviceInstallTimerList);
            valueList.push_back(periodOfValidityList);
            valueList.push_back(posList);
            valueList.push_back(sizeList);
            valueList.push_back(sysOfDeviceList);
            valueList.push_back(operatorList);
            valueList.push_back(analogTypeList);
            valueList.push_back(channelNumList);
            m_sqliteManager->insertBatch("ItemInfo",valueList);

        }
    }
}

QStandardItem *ArchitePlanView::getItemFromView(GraphicsView *view)
{
    if(view!=nullptr)
    {
        QStandardItem *stdItem=  m_itemToViewHash.key(view);
        return stdItem;

    }
    else
    {
        return nullptr;
    }
}

GraphicsView *ArchitePlanView::currentGraphicsView()
{
    GraphicsView *graphicsView = dynamic_cast<GraphicsView *> (m_stackedWidget->currentWidget());
    return graphicsView;
}

bool ArchitePlanView::havingAlarms()
{
    return !m_alarmViewList.isEmpty();
}

QString ArchitePlanView::architeInfoDbName()
{
    return m_architeInfoDbName;
}




QHash<QString,QVariant> ArchitePlanView::saveViewInfo(QStandardItem *item)
{
    QHash<QString,QVariant> imageHash;
    QMap<QStandardItem*,int>itemMap;
    itemMap = m_treeView->getTreeIndexMap();
    int page =itemMap[item];
    GraphicsView*widget = m_widgetHash.value(page);
    if(widget!=nullptr)
    {
        imageHash["path"]=widget->pixmapName();

    }

    return imageHash;
}

void ArchitePlanView::initFromJsonFile()
{
    if(m_globalGraphicsView==nullptr)
    {
        return;
    }

    GlobalGraphicsScene *globalScene=  m_globalGraphicsView->currentScene();
    if(globalScene==nullptr)
    {
        return;
    }
    QList<QString>buildingNameList;
    foreach (GlobalGraphicsItem*gItem, globalScene->currentItemList())
    {
        if(gItem!=nullptr)
        {
            buildingNameList.push_back(gItem->buildName());
        }
    }

    QList<QVariant>   jsonValueList=JsonEdit::instance()->readFile(c_jsonFilePath).toList();
    foreach (QString name, buildingNameList)
    {
        QStandardItem *parentItem= m_treeView->addRootItem(name);
        GlobalGraphicsItem*globalGraphicsItem=globalScene->itemFromBuildingName(name);
        if(globalGraphicsItem!=nullptr)
        {
            m_globalToArchitePlanHash[globalGraphicsItem] = parentItem;
        }
        for(QVariant curValue:jsonValueList)
        {
            QHash<QString,QVariant> parentHash=  curValue.toHash();
            if(!parentHash.isEmpty())
            {
                QString rootItemName=parentHash.value("name").toString();
                if(rootItemName==name)
                {
                    QList<QVariant> childList =  parentHash.value("child").toList();

                    for(QVariant childValue:childList)
                    {
                        QHash<QString,QVariant> childHash=  childValue.toHash();
                        QModelIndex parentIndex= parentItem->index();

                        if(parentIndex.isValid())
                        {
                            QStandardItem *childItem= m_treeView->addChildItem(parentIndex);
                            if(childItem!=nullptr)
                            {
                                childItem->setText(childHash.value("name").toString());
                                QHash<QString,QVariant> childPixmapHash = childHash["image"].toHash();
                                GraphicsView*view=  setViewFromJson(childPixmapHash,childItem);
                                initFromDataBase(view,parentItem->text(),childItem->text());


                            }
                        }

                    }
                }
            }
        }

    }
    GraphicsView *curView=   currentGraphicsView();
    if(curView!=nullptr)
    {
        QStandardItem *curStdItem=   getItemFromView(curView);
        if(curStdItem!=nullptr)
        {
            if(m_treeView!=nullptr)
            {
                m_treeView->setCurrentIndex(curStdItem->index());
            }
        }
    }

}





void ArchitePlanView::initFromDataBase(GraphicsView *view,const QString &buildingName,const QString &floor)
{

    QStringList dbValueList=  m_sqliteManager->executeQuery(QString("select *from ItemInfo where buildingName ='%1' and floorOfDevice = '%2'").arg(buildingName).arg(floor));
    int valueSize = dbValueList.size();

    if(view!=nullptr)
    {
        QGraphicsScene *scene = view->scene();
        GraphicsScene *graphicsScene = dynamic_cast<GraphicsScene*>(scene);
        if(valueSize>=m_itemInfoTableSize&&valueSize%m_itemInfoTableSize==0)
        {
            for(int j=0;j<valueSize;j=j+m_itemInfoTableSize)
            {

                if(valueSize>j+m_itemInfoTableSize-1)
                {


                    GraphicsItem *item = new GraphicsItem(graphicsScene);

                    item->extNum() =dbValueList.at(j);
                    Controller::instance()->getDataStore()->extNum()=item->extNum();
                    item->addrNum() = dbValueList.at(j+1);
                    item->loopNum() =dbValueList.at(j+2);
                    Controller::instance()->getDataStore()->loopNum() = item->loopNum();
                    QString curNetworkNum = dbValueList.at(j+3);
                    if(curNetworkNum.isEmpty())
                    {
                        curNetworkNum="0";
                    }

                    item->networkNum() =curNetworkNum;
                    Controller::instance()->getDataStore()->networkNum() = curNetworkNum;
                    QString curPowerAddr= dbValueList.at(j+4);
                    if(curPowerAddr.isEmpty())
                    {
                        curPowerAddr= "0";
                    }

                    item->powerAddr() = curPowerAddr;
                    Controller::instance()->getDataStore()->powerAddr() = curPowerAddr;
                    item->buildingName() = dbValueList.at(j+5);
                    item->currentState() =dbValueList.at(j+6);
                    item->deviceLocation() = dbValueList.at(j+7);
                    item->deviceNum() =dbValueList.at(j+8);
                    QString pixPath =dbValueList.at(j+11);
                    item->setIconName(pixPath);

                    item->equipmentModel() = dbValueList.at(j+9);
                    item->floorOfDevice() =dbValueList.at(j+10);

                    item->manufacturers() =dbValueList.at(j+12);
                    item->deviceInstallTime()=dbValueList.at(j+13);
                    item->periodOfValidity() = dbValueList.at(j+14);
                    QString posStr = dbValueList.at(j+15);
                    item->setPos(QPointF(posStr.section(",",0,0).toDouble(),posStr.section(",",1,1).toDouble()));
                    QString sizeStr = dbValueList.at(j+16);
                    item->setRadius(sizeStr.toDouble());
                    item->sysOfDevice() =dbValueList.at(j+17);
                    item->deviceOperator() = dbValueList.at(j+18);
                    item->analogType() = dbValueList.at(j+19);
                    QString channelNumStr = dbValueList.at(j+20);
                    item->channelNum() = channelNumStr.toInt();
                    if(graphicsScene!=nullptr)
                    {
                        graphicsScene->addItem(item);
                        graphicsScene->getItemList().push_back(item);
                    }

                    item->setIconIndex(ItemIconInfoToJson::iconIndex(item->iconName()));

                }
            }
        }
    }

    // m_dbValueList.clear();
}

GraphicsView* ArchitePlanView::setViewFromJson(const QHash<QString,QVariant> &hash,QStandardItem *treeItem)
{

    if(!hash.isEmpty())
    {
        QMap<QStandardItem*,int>parentItemMap;
        parentItemMap = m_treeView->getTreeIndexMap();
        int page =parentItemMap[treeItem];
        GraphicsView*widget = m_widgetHash.value(page);
        if(widget!=nullptr)
        {
            QString pixPath= hash.value("path").toString();
            widget->loadPixmap(pixPath);
        }
        return widget;
    }
    else
    {
        return nullptr;
    }
}

void ArchitePlanView::findFireAlarm(int pos)
{
    QList<QGraphicsItem*>itemList= Controller::instance()->getDataStore()->getTypeItemList(tr("火警"));
    int listSize = itemList.size();

    QList<GraphicsView *>viewList = m_widgetHash.values();
    if(listSize>0)
    {
        foreach (GraphicsView *currentView, viewList)
        {
            if(currentView!=nullptr)
            {
                QList<QGraphicsItem*>viewItemList= currentView->getItemList();
                foreach (QGraphicsItem*currentItem, viewItemList)
                {
                    if(listSize>pos && pos>=0)
                    {
                        if(currentItem==itemList.at(pos))
                        {
                            m_tabWidget->setCurrentIndex(1);
                            QStandardItem *stdItem=  getItemFromView(currentView);
                            if(stdItem!=nullptr)
                            {
                                m_treeView->setCurrentIndex(stdItem->index());
                            }

                            autoFitView(currentView);
                            return;
                        }
                    }
                    else
                    {
                        if(currentItem==itemList.at(listSize-1))
                        {
                            m_tabWidget->setCurrentIndex(1);
                            autoFitView(currentView);
                            return;
                        }
                    }
                }
            }

        }
    }
}

void ArchitePlanView::saveOtherArchiteInfo()
{
    QHash<QString,QVariant> architePlanHash;

    architePlanHash["sysArchitePlan"] = m_sysArchitePlanView->infoToJson();
    saveGeneralLayoutInfo();
    architePlanHash["grobalPlanPicture"] =m_globalArchitePlanPixmapName;
    QmlForJson qmlForJson;
    qmlForJson.writeFile(architePlanHash);
}

void ArchitePlanView::saveInfo()
{
    m_treeView->saveTreeItem();
    saveArchiteInfo();
    saveOtherArchiteInfo();
}

void ArchitePlanView::createAlarm(GraphicsItem *item,const QString &alarmType,const QString & alarmState,const QString &alarmTime)
{

    if(item!=nullptr)
    {
        QString  state=item->alarmState(alarmType);
        //        if(!state.isEmpty()&&item->alarmState(alarmType)!="OK")
        //        {
        //            return;
        //        }
        item->setAlarmRecord(alarmType,alarmTime,alarmState);
        QString curAlarmType=alarmType;

        if(alarmType.startsWith(tr("模拟")))
        {
            curAlarmType.remove(tr("模拟"));
        }

        item->stopAnimations();

        Controller::instance()->getDataStore()->insertTypeItem(curAlarmType,item);
        updateAlarmText(item,curAlarmType);
        filterAlarm(item);
        filterAlarmView(item,curAlarmType);
        emit alarmItem(item,alarmType);
        //emit alarmHappend(curAlarmType);

    }
}

GraphicsItem *ArchitePlanView::itemFormInfo(const QString &extNum, const QString &loopNum, const QString &addressNum, const QString &networkNum,const QString &powerAddr)
{
    GraphicsItem *graphicsItem = nullptr;
    bool isFind = false;
    QList<GraphicsView *>viewList = m_widgetHash.values();

    foreach (GraphicsView *currentView, viewList)
    {

        QList<QGraphicsItem*> itemList= currentView->getItemList();
        foreach (QGraphicsItem*item, itemList)
        {
            GraphicsItem *curItem = dynamic_cast<GraphicsItem *>(item);
            if(curItem->extNum()==extNum&&curItem->loopNum()==loopNum&&
                    curItem->addrNum()==addressNum&&curItem->networkNum()==networkNum &&curItem->powerAddr()==powerAddr)
            {
                graphicsItem = curItem;
                isFind = true;
                break;
            }
        }
        if(isFind)
        {
            break;
        }

    }
    return graphicsItem;
}



void ArchitePlanView::closeQuickView()
{
    m_itemSettingView->close();
    m_analogAlarmView->close();
    m_maintenanceView->close();
    m_globalGraphicsView->currentScene()->closeQuickView();
    m_treeView->closeQuickView();
}

void ArchitePlanView::retranslate()
{
    m_treeView->retranslate();
    m_sysArchitePlanView->retranslate();
    m_globalGraphicsView->retranslate();

    if(m_tabWidget->count()>2)
    {
        m_tabWidget->setTabText(0,tr("总平面布局图"));
        m_tabWidget->setTabText(1,tr("建筑平面图"));
        m_tabWidget->setTabText(2,tr("系统图"));
    }
    m_modeSelectMenu->setTitle(tr("模式选择"));
    m_deleteAction->setText(tr("删除"));
    m_editAction->setText(tr("编辑"));
    m_clearAction->setText(tr("清空"));
    m_deleteSelectedAction->setText(tr("删除选中"));
    m_closeAction->setText(tr("关闭"));
    m_analogAlarmAction->setText(tr("报警模拟"));
    m_maintenanceAction->setText(tr("设备维保"));
    m_handDragAction->setText(tr("手动拖拽模式"));
    m_rubberBandDragAction->setText(tr("橡皮筋模式"));
    m_itemTextVisiableAction->setText(tr("文字可见"));
    m_fitViewAction->setText(tr("最佳视图"));
    m_analogValueQueryAction->setText(tr("模拟量查询"));
    m_itemSettingView->setTitle(tr("设备信息设置界面"));
    m_analogAlarmView->setTitle(tr("报警模拟界面"));
    m_maintenanceView->setTitle(tr("设备维保"));

    if(m_itemSettingObj!=nullptr)
    {
        QMetaObject::invokeMethod(m_itemSettingObj,"retranslate");
    }

    if(m_maintenanceObj!=nullptr)
    {
        QMetaObject::invokeMethod(m_maintenanceObj,"retranslate");
    }

    if(m_analogAlarmObj!=nullptr)
    {
        QMetaObject::invokeMethod(m_analogAlarmObj,"retranslate");
    }

    m_typeList.clear();

    m_typeList << tr("火警") << tr("启动") << tr("监管") << tr("故障")<< tr("反馈") << tr("屏蔽");



}



void ArchitePlanView::setGlobalArchiteFromJson()
{
    QmlForJson qmlForJson;
    QHash<QString,QVariant> valueHash = qmlForJson.readFile().toHash();
    QString pixmapName= valueHash["grobalPlanPicture"].toString();
    setGlobalArchitePixmap(pixmapName);
    QStringList itemValueList=   m_sqliteManager->executeQuery("select *from GlobalArchite");
    int listSize = itemValueList.size();
    for(int i=0;i<listSize;i=i+m_globalArchiteTableSize)
    {
        QString posStr = itemValueList.at(i+2);
        qreal x = posStr.section(",",0,0).toDouble();
        qreal y = posStr.section(",",1,1).toDouble();
        QPointF point(x,y);
        GlobalGraphicsItem *currentItem=   m_globalGraphicsView->currentScene()->addGlobalGraphicsItem(point);
        if(currentItem!=nullptr)
        {
            if(itemValueList.size()>i+4)
            {
                currentItem->setBuildName(itemValueList.at(i));
                currentItem->setPersonOnDuty(itemValueList.at(i+1));
                currentItem->setIconName(itemValueList.at(i+4));
                QString sizeStr= itemValueList.at(i+3);
                currentItem->setItemSize(sizeStr.toDouble());
            }
        }
    }

}

void ArchitePlanView::updateAlarmWidget(GraphicsView *currentView)
{
    if(currentView!=nullptr)
    {

        int currentIndex = m_alarmWidgetHash[m_currentAlarmType].indexOf(currentView);
        int total = totalPage();
        if(total>1)
        {
            if(total>2)
            {
                if(currentIndex>=1&&currentIndex<total-1)
                {
                    emit normalPage();
                }
                else if(currentIndex<1)
                {
                    emit toFirstPage();
                }
                else if(currentIndex==total-1)
                {
                    emit toLastPage();
                }
            }
            else
            {
                if(currentIndex<1)
                {
                    emit toFirstPage();
                }
                else
                {
                    emit toLastPage();
                }
            }
        }
        else if(total<=1)
        {
            emit noPage();
        }
    }

}

void ArchitePlanView::deleteAlarmWidget(GraphicsView *currentView)
{

    if(m_alarmWidgetHash.value(m_currentAlarmType).contains(currentView))
    {
        m_alarmWidgetHash[m_currentAlarmType].removeOne(currentView);
    }
    updateAlarmWidget(currentGraphicsView());
}

void ArchitePlanView::filterAlarm(GraphicsItem *item)
{
    if(item!=nullptr)
    {
        QString curType = item->alarmType();
        QString curState= item->currentState();
        if(curState!="OK"&&!curState.isEmpty())
        {
            if(!curType.startsWith(tr("模拟")))
            {
                curState = curType;
            }
        }
        else
        {
            item->stopAnimations();
            return;
        }
        startAlarmAnimation(item);
        viewSwitch(item);
    }
}

void ArchitePlanView::filterAlarmView(GraphicsItem *item, const QString &alarmType)
{
    GraphicsView *view = Controller::instance()->getDataStore()->itemDisplayView(item);
    if(view!=nullptr)
    {
        insertAlarmWidget(alarmType,view);
        updateAlarmWidget(currentGraphicsView());
        if(!m_alarmViewList.contains(view))
        {
            m_alarmViewList.push_back(view);
        }
        if(m_tabWidget->count()>=2)
        {
            if(m_tabWidget->currentIndex()!=1)
            {
                m_tabWidget->setCurrentIndex(1);
            }
        }

        QStandardItem *parentItem =   getParnentItemFromView(view);
        if(parentItem!=nullptr)
        {
            GlobalGraphicsItem*globalGraphicsItem=  m_globalToArchitePlanHash.key(parentItem);
            if(globalGraphicsItem!=nullptr)
            {
                globalGraphicsItem->setCurAnimationRunState(true);
                if(m_tabWidget->currentIndex()==0)
                {
                    if(!globalGraphicsItem->animalIsRunning())
                    {
                        globalGraphicsItem->startAnimal(true);

                    }
                }

            }
        }

    }


}

void ArchitePlanView::viewSwitch(GraphicsItem *item)
{
    if(m_tabWidget->count()>=2)
    {
        if(m_tabWidget->currentIndex()!=1)
        {
            m_tabWidget->setCurrentIndex(1);
        }
    }

    QList<QGraphicsItem *> itemList=  Controller::instance()->getDataStore()->getTypeItemList(tr("火警"));
    if(item!=nullptr)
    {
        GraphicsView *view=   Controller::instance()->getDataStore()->itemDisplayView(item);
        if(!itemList.isEmpty())
        {

            if(item->alarmType()==tr("火警")&&itemList.at(0)==item)
            {
                autoFitView(view);
            }
        }
    }

}

void ArchitePlanView::startAlarmAnimation(const QString &alarmType)
{
    QList<QGraphicsItem*>itemList=Controller::instance()->getDataStore()->getTypeItemList(alarmType);
    foreach (QGraphicsItem*curItem, itemList)
    {
        GraphicsItem *item = dynamic_cast<GraphicsItem *>(curItem);
        filterAlarm(item);
    }

}

void ArchitePlanView::startAlarmAnimation(GraphicsItem *item)
{
    if(item!=nullptr)
    {

        QString curType = item->alarmType();
        QString curState= item->currentState();
        GraphicsView *view = Controller::instance()->getDataStore()->itemDisplayView(item);
        if(curState!="OK"&&!curState.isEmpty())
        {
            if(!curType.startsWith(tr("模拟")))
            {
                curState = curType;
            }
        }
        else
        {
            return;
        }

        QList<QGraphicsItem*> itemList= Controller::instance()->getDataStore()->getTypeItemList(curState);
        if(!itemList.isEmpty())
        {
            GraphicsItem *curItem = dynamic_cast<GraphicsItem*>(itemList.at(0));

            if(curItem==item)
            {

                emit setFirstAlarmInfo(item);

                if(curState.endsWith(tr("火警")))
                {
                    item->startScaleAnimation();
                }
                else
                {
                    if(view!=currentGraphicsView())
                    {
                        return;
                    }
                    item->startColorAnimation();
                    if(curState.endsWith(tr("故障")))
                    {
                        item->setColorEndValue(QColor("yellow"));
                    }
                    else if(curState.endsWith(tr("屏蔽")))
                    {
                        item->setColorEndValue(QColor("orange"));
                    }
                    else
                    {
                        item->setColorEndValue(QColor("red"));
                    }

                }


                if(view!=nullptr)
                {

                    qreal scaleValue = 0.6;
                    if(item!=nullptr)
                    {
                        view->addGraphicsTextItem(QPointF(item->pos().x()-item->radius()*0.8/view->transform().m11(),item->pos().y()+item->radius()*scaleValue/view->transform().m22()),curState);
                    }

                    QGraphicsTextItem *textItem = view->textItem(curState);
                    if(textItem!=nullptr)
                    {
                        connect(item,&GraphicsItem::moveToPos,this,[=](const QPointF &pos)
                        {

                            if(textItem!=nullptr&&item!=nullptr)
                            {
                                qreal m11 = view->transform().m11();
                                qreal m22 = view->transform().m22();
                                textItem->setPos(QPointF(pos.x()-item->radius()*1/m11*0.8,pos.y()+item->radius()*1/m22*scaleValue));
                            }

                        });
                        connect(item,&GraphicsItem::sizeChanged,this,[=](qreal size)
                        {
                            if(textItem!=nullptr&&item!=nullptr)
                            {
                                qreal m11 = view->transform().m11();
                                qreal m22 = view->transform().m22();
                                textItem->setPos(QPointF(item->pos().x()-size*0.8/m11,item->pos().y()+size*scaleValue/m22));
                            }

                        });

                        connect(view,&GraphicsView::currentScaleValue,this,[=](qreal currentValue){
                            if(textItem!=nullptr&&item!=nullptr)
                            {
                                textItem->setPos(QPointF(item->pos().x()-item->radius()/currentValue*0.8,item->pos().y()+item->radius()*scaleValue/currentValue));
                            }

                        });

                    }

                }
            }
            else
            {
                if(view!=currentGraphicsView())
                {
                    return;
                }
                if(curState.endsWith(tr("故障")))
                {
                    item->setColor(QColor("yellow"));
                }
                else if(curState.endsWith(tr("屏蔽")))
                {
                    item->setColor(QColor("orange"));
                }
                else
                {
                    item->setColor(QColor("red"));
                }

            }

        }
    }
}


void ArchitePlanView::deleteAlarmText(GraphicsItem *item, const QString &alarmType)
{
    Controller::instance()->getSpeechObj()->removeAlarmText(speechInfo(item,alarmType));
}

void ArchitePlanView::updateAlarmText(GraphicsItem *item, const QString &alarmType)
{

    Controller::instance()->getSpeechObj()->insertAlarmText(speechInfo(item,alarmType));
}

QString ArchitePlanView::speechInfo(GraphicsItem *item, const QString &alarmType)
{
    QString speechText="";
    if(item!=nullptr)
    {
        QList<QGraphicsItem *>itemList=  Controller::instance()->getDataStore()->getTypeItemList(alarmType);
        if(itemList.size()>0)
        {
            if(item==itemList.at(0))
            {
                if(alarmType==tr("火警"))
                {
                    speechText = tr("首火警");
                }
                else
                {

                    speechText = alarmType+tr("报警");

                }
            }
            else
            {
                if(alarmType!=tr("火警"))
                {
                    speechText = alarmType+tr("报警");
                }
                else
                {
                    speechText = alarmType;
                }
            }

            speechText += ","+item->buildingName()+","+item->floorOfDevice()+","+item->deviceLocation()+
                    ";"+QString("%1,%2,%3,%4").arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum());


        }

    }
    return speechText;
}

void ArchitePlanView::saveItemInfoToDb(GraphicsItem *item)
{
    QStringList valueList;
    for(int i=0;i<m_itemInfoTableSize;i++)
    {
        valueList.push_back(QString("'%%1'").arg(i+1));
    }

    QString insertStr = QString("insert into ItemInfo values (")+valueList.join(",")+")";
    if(item!=nullptr)
    {

        m_sqliteManager->executeQuery(insertStr.arg(item->extNum()).arg(item->addrNum()).arg(item->loopNum()).arg(item->networkNum()).arg(item->powerAddr())
                                      .arg(item->buildingName()).arg(item->currentState()).arg(item->deviceLocation()).arg(item->deviceNum()).arg(item->equipmentModel())
                                      .arg(item->floorOfDevice()).arg(item->iconName()).arg(item->manufacturers()).arg(item->deviceInstallTime()).arg(item->periodOfValidity())
                                      .arg(QString("%1,%2").arg(item->scenePos().x()).arg(item->scenePos().y())).arg(item->radius()).arg(item->sysOfDevice()).arg(item->deviceOperator())
                                      .arg(item->analogType()).arg(item->channelNum()));
    }
}

void ArchitePlanView::deleteItemInfoFromDb(GraphicsItem *item)
{
    if(item!=nullptr)
    {
        QString deleteStr = QString("delete from ItemInfo where extNum ='%1' and loopNum='%2' and addrNum ='%3' and networkNum ='%4' and powerAddr='%5'").arg(item->extNum()).arg(item->loopNum()).arg(item->addrNum()).arg(item->networkNum()).arg(item->powerAddr());
        m_sqliteManager->executeQuery(deleteStr);
    }


}

void ArchitePlanView::deleteInfoFromFloor(const QString &floorName)
{
    m_sqliteManager->executeQuery(QString("delete from ItemInfo where floorOfDevice ='%1'").arg(floorName));
}

void ArchitePlanView::deleteInfoFromBuilding(const QString &buildingName)
{
    m_sqliteManager->executeQuery(QString("delete from ItemInfo where buildingName ='%1'").arg(buildingName));
}

void ArchitePlanView::clearDbInfo()
{

}

void ArchitePlanView::resetItemNumFromView(GraphicsView *view)
{
    if(view!=nullptr)
    {
        if(view->getItemList().size()>0)
        {
            int value =0;
            foreach(QGraphicsItem*curItem,view->getItemList())
            {
                GraphicsItem *curGraphicsItem = dynamic_cast<GraphicsItem *>(curItem);
                if(curGraphicsItem!=nullptr)
                {
                    value=qMax(value,curGraphicsItem->addrNum().toInt());
                }
            }
            Controller::instance()->getDataStore()->setItemNum(value);
        }
        else
        {
            Controller::instance()->getDataStore()->setItemNum(0);
        }
    }

}

void ArchitePlanView::clearGlobalViewAlarm()
{
    QList<QGraphicsItem*> globalGraphicsItemList= m_globalGraphicsView->currentScene()->items();
    foreach (QGraphicsItem*item, globalGraphicsItemList)
    {
        GlobalGraphicsItem *globalGraphicsItem = dynamic_cast<GlobalGraphicsItem*>(item);
        if(globalGraphicsItem!=nullptr)
        {
            globalGraphicsItem->setCurAnimationRunState(false);
            if(globalGraphicsItem->animalIsRunning())
            {
                globalGraphicsItem->startAnimal(false);

            }
        }
    }
}

void ArchitePlanView::clearItemsAlarmText(GraphicsItem*item)
{
    if(item!=nullptr)
    {
        foreach(const QString &type,item->alarmTypeList())
        {
            deleteAlarmText(item,type);
        }
    }
}



int ArchitePlanView::numOfTypeAlarm(const QString &type)
{
    int num= Controller::instance()->getDataStore()->numOfTypeItem(type);
    return num;
}

void ArchitePlanView::setGlobalArchitePixmap(const QString &pixmapName)
{
    QString filePath=  Controller::instance()->fileNameFromQml(pixmapName);

#ifdef Q_OS_LINUX
    if(!filePath.startsWith("/home"))
    {
        QFileInfo fileInfo(filePath);
        filePath =QApplication::applicationDirPath()+"/楼层图/" +fileInfo.fileName();
    }
#endif
    m_globalArchitePlanPixmapName = filePath;
    m_globalGraphicsView->setPicture(filePath);
}

QHash<int, GraphicsView *> &ArchitePlanView::getWidgetHash()
{
    return m_widgetHash;
}

int ArchitePlanView::totalPage()
{
    return m_alarmWidgetHash.value(m_currentAlarmType).size();
}

int ArchitePlanView::currentPage()
{
    GraphicsView*view = dynamic_cast<GraphicsView*>(m_stackedWidget->currentWidget());
    QList<GraphicsView*>viewList= m_alarmWidgetHash.value(m_currentAlarmType);
    return viewList.indexOf(view);
}

void ArchitePlanView::clearAlarm(bool clearFireAlarm)
{
    emit hideFirstFireAlarmInfo();
    emit reduInstruction(clearFireAlarm);
    Controller::instance()->getDataStore()->clearStoreAlarm();
    emit findAlarmNum(0,0);
    clearAllGraphicsTextItem();
    Controller::instance()->getSpeechObj()->clearAlarmText();
    clearAlarmWidget();
    m_alarmViewList.clear();
    emit noPage();
    clearGlobalViewAlarm();
    emit clearAlarmFromTable();
}

void ArchitePlanView::clearAlarmFromExtNum(const QString &extNum,const QString &networkNum)
{

    QList< QList<QGraphicsItem *> >list= Controller::instance()->getDataStore()->getTypeItemHash().values();
    //QStringList alarmStringList;
    //alarmStringList<< tr("火警")<<tr("启动") << tr("监管") << tr("故障")<<tr("反馈") <<tr("屏蔽");
    foreach (QList<QGraphicsItem *>itemList, list)
    {
        foreach (QGraphicsItem *currentItem, itemList)
        {
            GraphicsItem *item = dynamic_cast<GraphicsItem *>(currentItem);
            if(item!=nullptr)
            {
                GraphicsView*curView = Controller::instance()->getDataStore()->itemDisplayView(item);
                if(item->extNum()==extNum)
                {
                    QString curAlarmStr=   item->alarmType();
                    int itemIndex=Controller::instance()->getDataStore()->getTypeItemList(curAlarmStr).indexOf(item);
                    if(itemIndex==0)
                    {
                        if(curAlarmStr==tr("火警"))
                        {
                            emit hideFirstFireAlarmInfo();
                        }

                        if(curView!=nullptr)
                        {
                            curView->removeGraphicsTextItem(curAlarmStr);
                        }

                        disconnect(item,&GraphicsItem::moveToPos,nullptr,nullptr);
                        disconnect(item,&GraphicsItem::sizeChanged,nullptr,nullptr);
                    }

                    clearItemsAlarmText(item);
                    item->stopAnimations();
                    item->clearAllAlarm();
                    Controller::instance()->getDataStore()->deleteTypeItem(item);
                    if(curView!=nullptr)
                    {
                        foreach (QString curAlarm, item->alarmTypeList())
                        {
                            if(!curView->haveAlarmType(curAlarm))
                            {
                                deleteAlarmWidget(curAlarm,curView);
                            }

                        }

                        if(!curView->haveAnyAlarm())
                        {
                            if(m_alarmViewList.contains(curView))
                            {
                                m_alarmViewList.removeOne(curView);
                            }

                            QStandardItem *parentItem =   getParnentItemFromView(curView);
                            if(parentItem!=nullptr)
                            {
                                GlobalGraphicsItem*globalGraphicsItem=  m_globalToArchitePlanHash.key(parentItem);
                                if(globalGraphicsItem!=nullptr)
                                {
                                    globalGraphicsItem->setCurAnimationRunState(false);
                                    if(globalGraphicsItem->animalIsRunning())
                                    {
                                        globalGraphicsItem->startAnimal(false);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }

    Controller::instance()->getDataStore()->deleteTypeNoItem(extNum);


    int totalNum = Controller::instance()->getDataStore()->numOfTypeItem(m_currentAlarmType);
    emit findAlarmNum(totalNum,0);
    //    foreach (QString curAlarm, alarmStringList)
    //    {
    //        alarmHappend(curAlarm);
    //        // updateAlarmText(curAlarm);
    //    }
    emit alarmStateUpdate(extNum,networkNum);



}

void ArchitePlanView::clearExcptFireAlarm()
{
    clearAlarm(false);
    emit clearAllAlarmExceptFire();

}

void ArchitePlanView::toPreviousPage()
{
    int count =totalPage();
    if(count>0)
    {
        QList<GraphicsView*>viewList=  m_alarmWidgetHash[m_currentAlarmType];
        int currentIndex = currentPage();
        if(currentIndex<count && currentIndex>0)
        {
            if(viewList.size()>currentIndex-1)
            {
                m_stackedWidget->setCurrentWidget(viewList.at(currentIndex-1));
            }
        }
    }

}

void ArchitePlanView::toNextPage()
{
    int count =totalPage();
    if(count>0)
    {
        int currentIndex = currentPage();
        QList<GraphicsView*>viewList=  m_alarmWidgetHash[m_currentAlarmType];
        if(currentIndex<count-1)
        {
            if(viewList.size()>currentIndex+1)
            {
                m_stackedWidget->setCurrentWidget(viewList.at(currentIndex+1));
            }
        }

    }
}


void ArchitePlanView::setCurrentAlarmType(const QString &type)
{
    m_currentAlarmType = type;
}

void ArchitePlanView::toArchitePlan(const QString &extNum, const QString &loopNum, const QString &addressNum,const QString &networkNum,const QString&powerAddr)
{
    foreach (GraphicsView *view, m_widgetHash.values())
    {
        if(view!=nullptr)
        {
            QList<QGraphicsItem *>itemList =   view->getItemList();
            foreach(QGraphicsItem *item,itemList)
            {
                GraphicsItem *currentItem = dynamic_cast<GraphicsItem *>(item);
                if(currentItem!=nullptr)
                {
                    if(currentItem->extNum()==extNum && currentItem->loopNum()==loopNum
                            && currentItem->addrNum()==addressNum
                            && currentItem->networkNum()==networkNum && currentItem->powerAddr()==powerAddr)
                    {
                        if(m_tabWidget->count()>=2)
                        {
                            if(m_tabWidget->currentIndex()!=1)
                            {
                                m_tabWidget->setCurrentIndex(1);
                            }
                        }

                        QString curAlarmType= currentItem->alarmType();
                        int curIndex=Controller::instance()->getDataStore()->indexOfItem(extNum,loopNum,addressNum,networkNum,curAlarmType);
                        if(curIndex>0)
                        {
                            currentItem->startScaleAnimation(1);
                        }
                        else if(curIndex<0)
                        {
                            view->fitInView(currentItem,Qt::KeepAspectRatio);
                        }


                        m_stackedWidget->setCurrentWidget(view);
                        view->scaleValueChanged(view->transform().m11());
                        return;
                    }
                }
            }
        }
    }
}

QString ArchitePlanView::currentAlarmType()
{
    return m_currentAlarmType;
}

void ArchitePlanView::deleteViewFromItem(QStandardItem* item)
{
    QMap<QStandardItem*,int>itemMap;
    itemMap = m_treeView->getTreeIndexMap();
    if(item!=nullptr)
    {
        GlobalGraphicsItem*globalGraphicsItem=  m_globalToArchitePlanHash.key(item);
        m_globalGraphicsView->deleteItemFromScene(globalGraphicsItem);
        deleteInfoFromBuilding(item->text());

        if(item->hasChildren())
        {
            for(int i=0;i<item->rowCount();i++)
            {
                QStandardItem*childItem =  item->child(i);
                int chileItemPage = itemMap.value(childItem);
                GraphicsView*childWidget = m_widgetHash.value(chileItemPage);
                if(childWidget!=nullptr)
                {
                    m_stackedWidget->removeWidget(childWidget);                    deleteAlarmWidget(childWidget);

                }

                m_widgetHash.remove(chileItemPage);
                m_treeView->getTreeIndexMap().remove(childItem);
            }
        }
    }
    m_treeView->getTreeIndexMap().remove(item);
    saveGeneralLayoutInfo();
}

void ArchitePlanView::deleteViewFromIndex(QStandardItem *item)
{
    QMap<QStandardItem*,int>itemMap;
    itemMap = m_treeView->getTreeIndexMap();
    if(item!=nullptr)
    {
        deleteInfoFromBuilding(item->text());
        if(item->hasChildren())
        {
            for(int i=0;i<item->rowCount();i++)
            {
                QStandardItem*childItem =  item->child(i);
                int chileItemPage = itemMap.value(childItem);
                GraphicsView*childWidget = m_widgetHash.value(chileItemPage);
                if(childWidget!=nullptr)
                {
                    m_stackedWidget->removeWidget(childWidget);                    deleteAlarmWidget(childWidget);

                }

                m_widgetHash.remove(chileItemPage);
                m_treeView->getTreeIndexMap().remove(childItem);
            }
        }
    }
    m_treeView->getTreeIndexMap().remove(item);
    saveGeneralLayoutInfo();
}

void ArchitePlanView::viewsAutoSwitch()
{

    //QMutexLocker lock(&m_mutex);
#ifdef Q_OS_LINUX
    sigsetjmp(sigEnv,1);
#endif
    if(m_tabWidget->count()>=2)
    {
        if(m_tabWidget->currentIndex()!=1)
        {
            m_tabWidget->setCurrentIndex(1);
        }
    }
    QList<QGraphicsItem*> gItemList= Controller::instance()->getDataStore()->getTypeItemList(tr("火警")/*m_currentAlarmType*/);
    if(m_alarmNum<gItemList.size())
    {
        QGraphicsItem*curItem = gItemList.at(m_alarmNum);
        GraphicsItem *item = dynamic_cast<GraphicsItem*>(curItem);
        if(item!=nullptr)
        {
            // qreal size=item->scale()*item->radius()*8;
            //QRectF currentRectF = QRectF(item->pos().x(),item->pos().y(),size,size);
            GraphicsView *view = dynamic_cast<GraphicsView *>(Controller::instance()->getDataStore()->itemDisplayView(item));
            if(view!=nullptr)
            {
                //                QStandardItem *stdItem = getItemFromView(view);
                //                if(stdItem!=nullptr)
                //                {
                //                    m_treeView->setCurrentIndex(stdItem->index());
                //                }

                QString curAlarmType= item->alarmType();
                int curIndex=Controller::instance()->getDataStore()->indexOfTypeFromItem(curAlarmType,item);
                if(curIndex>0)
                {
                    item->startScaleAnimation(1);
                }

                m_stackedWidget->setCurrentWidget(view);
                //view->fitInView(item,Qt::KeepAspectRatio);
                //view->fitInView(currentRectF.adjusted(-size,-size,view->pos().x()/2,view->pos().y()/2),Qt::KeepAspectRatio);
                view->scaleValueChanged(view->transform().m11());
            }
        }
        m_alarmNum++;
        int totalNum = Controller::instance()->getDataStore()->numOfTypeItem(tr("火警")/*m_currentAlarmType*/);
        emit findAlarmNum(totalNum,m_alarmNum);
    }
    else
    {
        m_alarmNum=0;
    }
#ifdef Q_OS_LINUX
    signal(SIGSEGV, segFaultReceive);
#endif
}

void ArchitePlanView::startAutoSwitch(bool isAuto)
{
    if(isAuto)
    {
        if(!m_autoSwitchTimer->isActive())
        {
            m_autoSwitchTimer->start(1500);
        }
    }
    else
    {
        m_autoSwitchTimer->stop();
        m_alarmPos =0;
    }
}

void ArchitePlanView::setItemSize(qreal size)
{
    GraphicsView *view= currentGraphicsView();
    if(view!=nullptr)
    {
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
        if(scene!=nullptr)
        {
            scene->setItemSize(size);
        }
    }
}

void ArchitePlanView::setItemIcon(QString iconName)
{
    GraphicsView *view= currentGraphicsView();
    if(view!=nullptr)
    {
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
        if(scene!=nullptr)
        {
            scene->setItemIcon(iconName);
        }
    }
}

void ArchitePlanView::setItemInfoFromType(const QString &type, const QString &info)
{

    GraphicsView *view= currentGraphicsView();
    if(view!=nullptr)
    {
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
        if(scene!=nullptr)
        {
            if(type=="loopNum"|| type=="addrNum")
            {
                QVariant loopNumValue,addrNumValue;
                QMetaObject::invokeMethod(m_itemSettingObj,"getLoopNum",Q_RETURN_ARG(QVariant,loopNumValue));
                QMetaObject::invokeMethod(m_itemSettingObj,"getAddrNum",Q_RETURN_ARG(QVariant,addrNumValue));
                if(!loopNumValue.toString().isEmpty() && !addrNumValue.toString().isEmpty())
                {


                    quint32 loopAddrValue= loopNumValue.toUInt()*1000+addrNumValue.toUInt();
                    if(m_loopAddrDeviceHash.contains(loopAddrValue))
                    {
                        QMetaObject::invokeMethod(m_itemSettingObj,"setExtNum",Q_ARG(QVariant,m_loopAddrExtHash.value(loopAddrValue)));
                        QMetaObject::invokeMethod(m_itemSettingObj,"setEquipmentModel",Q_ARG(QVariant,m_loopAddrDeviceHash.value(loopAddrValue)));

                        QMetaObject::invokeMethod(m_itemSettingObj,"setDeviceLocation",Q_ARG(QVariant,m_loopAddrLocationHash.value(loopAddrValue)));


                        scene->setItemInfoFromType("extNum",m_loopAddrExtHash.value(loopAddrValue));
                        scene->setItemInfoFromType("equipmentModel",m_loopAddrDeviceHash.value(loopAddrValue));
                        scene->setItemInfoFromType("deviceLocation",m_loopAddrLocationHash.value(loopAddrValue));


                    }
                }
            }
            scene->setItemInfoFromType(type,info);
        }
    }
}



void ArchitePlanView::setItemsIcon(int index, QString iconName)
{
    GraphicsView *view= currentGraphicsView();
    if(view!=nullptr)
    {
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
        if(scene!=nullptr)
        {
            scene->setItemsIcon(index,iconName);
        }
    }
}

void ArchitePlanView::setItemsEquipmentModel(int index, QString device)
{
    GraphicsView *view= currentGraphicsView();
    if(view!=nullptr)
    {
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
        if(scene!=nullptr)
        {
            scene->setItemsEquipmentModel(index,device);
        }
    }
}

void ArchitePlanView::setItemsManufacturers(int index, QString manufacturers)
{
    GraphicsView *view= currentGraphicsView();
    if(view!=nullptr)
    {
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
        if(scene!=nullptr)
        {
            scene->setItemsManufacturers(index,manufacturers);
        }
    }
}

void ArchitePlanView::setItemsPeriodOfValidity(int index, QString periodOfValidity)
{
    GraphicsView *view= currentGraphicsView();
    if(view!=nullptr)
    {
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
        if(scene!=nullptr)
        {
            scene->setItemsPeriodOfValidity(index,periodOfValidity);
        }
    }
}

void ArchitePlanView::setDeviceInstallTime(int index, QString deviceInstallTime)
{
    GraphicsView *view= currentGraphicsView();
    if(view!=nullptr)
    {
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(view->currentGraphicsScene());
        if(scene!=nullptr)
        {
            scene->setDeviceInstallTime(index,deviceInstallTime);
        }
    }
}

void ArchitePlanView::saveGeneralLayoutInfo()
{
    QList<QString> nameList;
    for(int i=0;i<m_treeView->model()->rowCount();i++)
    {
        QStandardItemModel* curModel= dynamic_cast<QStandardItemModel*>(m_treeView->model()) ;
        if(curModel!=nullptr)
        {
            nameList.push_back( curModel->item(i)->text());
        }
    }
    m_globalGraphicsView->currentScene()->sortItemList(nameList);

    m_sqliteManager->executeQuery("delete from GlobalArchite");
    QList<GlobalGraphicsItem *> itemList=  m_globalGraphicsView->currentScene()->currentItemList();
    QList<QVariant>buildingNameList,personOnDutyList,posList,sizeList,iconNameList,valueList;
    if(itemList.size()>0)
    {
        foreach (GlobalGraphicsItem*globalItem, itemList)
        {
            if(globalItem!=nullptr)
            {
                buildingNameList.push_back(globalItem->buildName());
                personOnDutyList.push_back(globalItem->personOnDuty());
                posList.push_back(QString("%1,%2").arg( globalItem->scenePos().x()).arg(globalItem->scenePos().y()));
                sizeList.push_back(globalItem->itemSize());
                iconNameList.push_back(globalItem->iconName());
            }
        }
        valueList.push_back(buildingNameList);
        valueList.push_back(personOnDutyList);
        valueList.push_back(posList);
        valueList.push_back(sizeList);
        valueList.push_back(iconNameList);
        m_sqliteManager->insertBatch("GlobalArchite",valueList);
    }
}

void ArchitePlanView::updateTreeItems()
{
    QStandardItemModel *model=dynamic_cast<QStandardItemModel *>(m_treeView->model());
    for(int i=0;i<model->rowCount();i++)
    {
        QStandardItem *parentItem = model->item(i);
        if(parentItem!=nullptr)
        {

            if(parentItem->hasChildren())
            {
                for(int j=0;j<parentItem->rowCount();j++)
                {
                    QStandardItem *childItem = parentItem->child(j);
                    if(childItem!=nullptr)
                    {
                        QHash<QString,QVariant> childImageHash= saveViewInfo(childItem);
                        JsonEdit::instance()->setChildImage(model->indexFromItem(parentItem).row(),model->indexFromItem(childItem).row(),childImageHash);
                    }
                }
            }

        }
    }

    JsonEdit::instance()->writeFile(c_jsonFilePath);
}

void ArchitePlanView::batchItems()
{
    QVariant sysValue,operatorValue,iconSizeValue,minNetworkNumValue,
            maxNetworkNumValue,minExtNumValue,maxExtNumValue,minLoopNumValue,maxLoopNumValue,
            minAddrNumValue,maxAddrNumValue,iconName,channelNum,analogType;
    int minNetworkNum,maxNetworkNum,minExtNum,maxExtNum,minLoopNum,maxLoopNum,minAddrNum,maxAddrNum;
    QMetaObject::invokeMethod(m_itemSettingObj,"getSysName",Q_RETURN_ARG(QVariant,sysValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"getOperator",Q_RETURN_ARG(QVariant,operatorValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"getIconSize",Q_RETURN_ARG(QVariant,iconSizeValue));


    QMetaObject::invokeMethod(m_itemSettingObj,"getMinNetworkNum",Q_RETURN_ARG(QVariant,minNetworkNumValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"getMaxNetworkNum",Q_RETURN_ARG(QVariant,maxNetworkNumValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"getMinExtNum",Q_RETURN_ARG(QVariant,minExtNumValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"getMaxExtNum",Q_RETURN_ARG(QVariant,maxExtNumValue));

    QMetaObject::invokeMethod(m_itemSettingObj,"getMinLoopNum",Q_RETURN_ARG(QVariant,minLoopNumValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"getMaxLoopNum",Q_RETURN_ARG(QVariant,maxLoopNumValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"getMinAddrNum",Q_RETURN_ARG(QVariant,minAddrNumValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"getMaxAddrNum",Q_RETURN_ARG(QVariant,maxAddrNumValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"curIconName",Q_RETURN_ARG(QVariant,iconName));

    QMetaObject::invokeMethod(m_itemSettingObj,"getChannelNum",Q_RETURN_ARG(QVariant,channelNum));
    QMetaObject::invokeMethod(m_itemSettingObj,"getAnalogType",Q_RETURN_ARG(QVariant,analogType));
    minNetworkNum = minNetworkNumValue.toInt();
    maxNetworkNum=maxNetworkNumValue.toInt();
    minExtNum=minExtNumValue.toInt();
    maxExtNum=maxExtNumValue.toInt();
    minLoopNum= minLoopNumValue.toInt();
    maxLoopNum=maxLoopNumValue.toInt();
    minAddrNum= minAddrNumValue.toInt();
    maxAddrNum= maxAddrNumValue.toInt();
    QString curEquipmentModel= ItemIconInfoToJson::getValue(QString::number(ItemIconInfoToJson::iconIndex(iconName.toString())),"deviceName");
    quint8 curValue=0;
    if(!minNetworkNumValue.toString().isEmpty()&&!maxNetworkNumValue.toString().isEmpty())
    {
        curValue= curValue|0x01;
    }
    else
    {
        curValue= curValue&0xfe;
    }

    if(!minAddrNumValue.toString().isEmpty()&&!maxAddrNumValue.toString().isEmpty())
    {
        curValue= curValue|0x02;
    }
    else
    {
        curValue= curValue&0xfd;
    }

    if(!minLoopNumValue.toString().isEmpty()&&!maxLoopNumValue.toString().isEmpty())
    {
        curValue= curValue|0x04;
    }
    else
    {
        curValue= curValue&0xfb;
    }

    if(!minExtNumValue.toString().isEmpty()&&!maxExtNumValue.toString().isEmpty())
    {
        curValue= curValue|0x08;
    }
    else
    {
        curValue= curValue&0xf7;
    }
    if(curValue==0)
    {
        return;
    }

    QList<GraphicsView *>viewList = m_widgetHash.values();

    foreach (GraphicsView *currentView, viewList)
    {

        if(currentView!=nullptr)
        {


            QList<QGraphicsItem*> itemList= currentView->getItemList();
            foreach (QGraphicsItem*item, itemList)
            {
                GraphicsItem *curItem = dynamic_cast<GraphicsItem *>(item);
                if(curItem!=nullptr)
                {
                    int curNetworkNum = curItem->networkNum().toInt();
                    int curExtNum = curItem->extNum().toInt();
                    int curLoopNum = curItem->loopNum().toInt();
                    int curAddrNum = curItem->addrNum().toInt();
                    switch (curValue&0x0f) {
                    case 0x01:
                        if(curNetworkNum>=minNetworkNum&&curNetworkNum<=maxNetworkNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }

                        }
                        break;
                    case 0x02:
                        if(curAddrNum>=minAddrNum&&curAddrNum<=maxAddrNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;

                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    case 0x03:
                        if(curAddrNum>=minAddrNum&&curAddrNum<=maxAddrNum&&curNetworkNum>=minNetworkNum&&curNetworkNum<=maxNetworkNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;

                    case 0x04:
                        if(curLoopNum>=minLoopNum&&curLoopNum<=maxLoopNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    case 0x05:
                        if(curLoopNum>=minLoopNum&&curLoopNum<=maxLoopNum&&curNetworkNum>=minNetworkNum&&curNetworkNum<=maxNetworkNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    case 0x06:
                        if(curLoopNum>=minLoopNum&&curLoopNum<=maxLoopNum&&curAddrNum>=minAddrNum&&curAddrNum<=maxAddrNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    case 0x07:
                        if(curLoopNum>=minLoopNum&&curLoopNum<=maxLoopNum&&curAddrNum>=minAddrNum&&curAddrNum<=maxAddrNum
                                &&curNetworkNum>=minNetworkNum&&curNetworkNum<=maxNetworkNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    case 0x08:
                        if(curExtNum>=minExtNum&&curExtNum<=maxExtNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    case 0x09:
                        if(curExtNum>=minExtNum&&curExtNum<=maxExtNum&&curNetworkNum>=minNetworkNum&&curNetworkNum<=maxNetworkNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    case 0x0a:
                        if(curExtNum>=minExtNum&&curExtNum<=maxExtNum&&curAddrNum>=minAddrNum&&curAddrNum<=maxAddrNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    case 0x0b:
                        if(curExtNum>=minExtNum&&curExtNum<=maxExtNum&&curAddrNum>=minAddrNum
                                &&curAddrNum<=maxAddrNum&&curNetworkNum>=minNetworkNum&&curNetworkNum<=maxNetworkNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;

                    case 0x0c:
                        if(curExtNum>=minExtNum&&curExtNum<=maxExtNum&&curLoopNum>=minLoopNum&&curLoopNum<=maxLoopNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    case 0x0d:
                        if(curExtNum>=minExtNum&&curExtNum<=maxExtNum&&curLoopNum>=minLoopNum&&curLoopNum<=maxLoopNum&&curNetworkNum>=minNetworkNum&&curNetworkNum<=maxNetworkNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    case 0x0e:
                        if(curExtNum>=minExtNum&&curExtNum<=maxExtNum&&curLoopNum>=minLoopNum&&curLoopNum<=maxLoopNum&&curAddrNum>=minAddrNum&&curAddrNum<=maxAddrNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))
                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    case 0x0f:
                        if(curExtNum>=minExtNum&&curExtNum<=maxExtNum&&curLoopNum>=minLoopNum&&curLoopNum<=maxLoopNum&&curAddrNum>=minAddrNum&&curAddrNum<=maxAddrNum&&curNetworkNum>=minNetworkNum&&curNetworkNum<=maxNetworkNum)
                        {
                            if(m_itemInfoSelectHash.value("device"))

                            {
                                curItem->setIconName(iconName.toString());
                                curItem->equipmentModel() =curEquipmentModel;
                            }
                            if(m_itemInfoSelectHash.value("size"))
                            {
                                curItem->setRadius(iconSizeValue.toReal());
                            }
                            if(m_itemInfoSelectHash.value("sys"))
                            {
                                curItem->sysOfDevice() = sysValue.toString();
                            }


                            if(m_itemInfoSelectHash.value("channel"))
                            {
                                if(!channelNum.toString().isEmpty())
                                {
                                    curItem->setChannelNum(channelNum.toInt());
                                }
                            }

                            if(m_itemInfoSelectHash.value("analog"))
                            {
                                curItem->analogType() = analogType.toString();
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            //  currentView->scene()->update();
        }
    }

}

void ArchitePlanView::changeItemsInfoFromFloor()
{
    QVariant sysValue,operatorValue,iconSizeValue,iconName,channelNum,analogType;

    QMetaObject::invokeMethod(m_itemSettingObj,"getSysName",Q_RETURN_ARG(QVariant,sysValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"getOperator",Q_RETURN_ARG(QVariant,operatorValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"getIconSize",Q_RETURN_ARG(QVariant,iconSizeValue));
    QMetaObject::invokeMethod(m_itemSettingObj,"curIconName",Q_RETURN_ARG(QVariant,iconName));
    QMetaObject::invokeMethod(m_itemSettingObj,"getChannelNum",Q_RETURN_ARG(QVariant,channelNum));
    QMetaObject::invokeMethod(m_itemSettingObj,"getAnalogType",Q_RETURN_ARG(QVariant,analogType));
    GraphicsView *curView =currentGraphicsView();
    QString curEquipmentModel= ItemIconInfoToJson::getValue(QString::number(ItemIconInfoToJson::iconIndex(iconName.toString())),"deviceName");
    if(curView!=nullptr)
    {
        QList<QGraphicsItem*> itemList=  curView->getItemList();
        foreach (QGraphicsItem*item, itemList)
        {
            GraphicsItem *curItem = dynamic_cast<GraphicsItem *>(item);
            if(curItem!=nullptr)
            {
                if(m_itemInfoSelectHash.value("device"))
                {
                    curItem->setIconName(iconName.toString());
                    curItem->equipmentModel() = curEquipmentModel;
                }
                if(m_itemInfoSelectHash.value("size"))
                {
                    curItem->setRadius(iconSizeValue.toReal());
                }
                if(m_itemInfoSelectHash.value("sys"))
                {
                    curItem->sysOfDevice() = sysValue.toString();
                }

                if(m_itemInfoSelectHash.value("channel"))
                {
                    if(!channelNum.toString().isEmpty())
                    {
                        curItem->setChannelNum(channelNum.toInt());
                    }
                }

                if(m_itemInfoSelectHash.value("analog"))
                {
                    curItem->analogType() = analogType.toString();
                }

            }
        }

        // curView->scene()->update();
    }
}

void ArchitePlanView::excelFileProcess(QString filePath)
{
    QString curFileName = Controller::instance()->fileNameFromQml(filePath);
    //#ifdef Q_OS_WIN


    //    bool excelIsOpen=  m_excelManager->openExcel(curFileName);
    //    if(excelIsOpen)
    //    {
    //        m_loopAddrExtHash.clear();
    //        m_loopAddrDeviceHash.clear();
    //        m_loopAddrLocationHash.clear();

    //        QVariant bus=  m_excelManager->readExcel("总线");//
    //        QVariant dataMapping=  m_excelManager->readExcel("数据映射表");//
    //        QList<QVariant> dataMappingList= dataMapping.toList();
    //        // qDebug() << dataMappingList.size();
    //        QHash<quint8,QPair<quint8,quint8> >dataMappingHash;
    //        for(int i=1;i<dataMappingList.size();i++)
    //        {
    //            QList<QVariant>curList= dataMappingList.at(i).toList();
    //            if(curList.size()>4)
    //            {
    //                QString type=  curList.at(1).toString();
    //                if(!type.isEmpty())
    //                {
    //                    QPair<quint8,quint8>pair;
    //                    pair.first = curList.at(2).toUInt();
    //                    pair.second = curList.at(3).toUInt();
    //                    dataMappingHash[curList.at(0).toUInt()]=pair;
    //                }
    //            }
    //        }

    //        QList<QVariant>busList=  bus.toList();
    //        for (int j=1;j<busList.size();j++) {
    //            QList<QVariant>curBusList=  busList.at(j).toList();
    //            if(curBusList.size()>=6)
    //            {
    //                QString loopAddr = curBusList.at(0).toString();
    //                if(!loopAddr.isEmpty())
    //                {
    //                    quint32 loopAddrValue = loopAddr.toUInt();
    //                    m_loopAddrDeviceHash[loopAddrValue]=curBusList.at(1).toString();
    //                    m_loopAddrLocationHash[loopAddrValue]=curBusList.at(3).toString();
    //                    quint8 loopNum = loopAddrValue/1000;
    //                    QList<QPair<quint8,quint8> >pairList=dataMappingHash.values();
    //                    for (int m=0;m< pairList.size();m++)
    //                    {
    //                        QPair<quint8,quint8>curPair = pairList.at(m);
    //                        if(loopNum>=curPair.first&&loopNum<=curPair.second)
    //                        {
    //                            m_loopAddrExtHash[loopAddrValue] =QString::number(dataMappingHash.key(curPair));

    //                        }
    //                    }

    //                }

    //            }
    //        }
    //        QMetaObject::invokeMethod(m_itemSettingObj,"setImportExcelState",Q_ARG(QVariant,true));
    //        m_excelManager->closeExcel();
    //        m_excelManager->quitExcel();

    //    }
    //    else
    //    {
    //        QMetaObject::invokeMethod(m_itemSettingObj,"setImportExcelState",Q_ARG(QVariant,false));
    //    }
    //#elif defined Q_OS_LINUX
    Document xlsx(curFileName);
    QList<QString> sheetNameList=xlsx.sheetNames();
    if(!sheetNameList.isEmpty())
    {
        m_loopAddrExtHash.clear();
        m_loopAddrDeviceHash.clear();
        m_loopAddrLocationHash.clear();
    }
    else
    {
        QMetaObject::invokeMethod(m_itemSettingObj,"setImportExcelState",Q_ARG(QVariant,false));
        return;
    }
    QHash<quint32,QPair<quint32,quint32> >dataMappingHash;
    foreach(const QString name,sheetNameList)
    {

        if(name.contains("数据映射表"))
        {
            xlsx.selectSheet(name);
            CellRange range;

            range = xlsx.dimension();

            int curHostNum=1,startLoopNum=3,endLoopNum=4;
            for(int j=1;j<range.columnCount();j++)
            {
                if(xlsx.read(1,j).toString().contains("联网主机号"))
                {
                    curHostNum=j;
                }
                else if(xlsx.read(1,j).toString().contains("起始回路"))
                {
                    startLoopNum =j;
                }
                else if(xlsx.read(1,j).toString().contains("终止回路"))
                {
                    endLoopNum =j;
                }
            }

            for(int i=2;i<range.rowCount();i++)
            {
                if(xlsx.read(i,2).isValid())
                {
                    QPair<quint32,quint32> pair;
                    quint32 ext=  xlsx.read(i,curHostNum).toUInt();
                    pair.first=  xlsx.read(i,startLoopNum).toUInt();
                    pair.second= xlsx.read(i,endLoopNum).toUInt();
                    dataMappingHash[ext]=pair;
                }
            }

        }
        else if(name.contains("总线"))
        {
            xlsx.selectSheet(name);
            CellRange range;

            int addrCodeIndex=1,deviceTypeIndex=2,descriptionIndex=3;
            range = xlsx.dimension();
            for(int j=1;j<range.columnCount();j++)
            {
                if(xlsx.read(1,j).toString()=="地址码")
                {
                    addrCodeIndex =j;
                }

                else if(xlsx.read(1,j).toString()=="设备类型")
                {
                    deviceTypeIndex =j;
                }
                else if(xlsx.read(1,j).toString()=="描述")
                {
                    descriptionIndex =j;
                }
            }

            for(int i=2;i<range.rowCount();i++)
            {
                quint32 curValue= xlsx.read(i,addrCodeIndex).toUInt();
                m_loopAddrDeviceHash[curValue] = xlsx.read(i,deviceTypeIndex).toString();
                m_loopAddrLocationHash[curValue]=xlsx.read(i,descriptionIndex).toString();
                foreach(quint32 extNum,dataMappingHash.keys())
                {

                    quint32 currentValue= curValue/1000;
                    QPair<quint32,quint32>curPair = dataMappingHash.value(extNum);

                    if(currentValue>=curPair.first && currentValue <=curPair.second)
                    {
                        m_loopAddrExtHash[curValue] = QString::number(extNum);

                    }
                }

            }

        }
    }

    QMetaObject::invokeMethod(m_itemSettingObj,"setImportExcelState",Q_ARG(QVariant,true));
    //#endif
}

void ArchitePlanView::setDeviceSelect(bool selected)
{
    m_itemInfoSelectHash["device"] = selected;
}

void ArchitePlanView::setSysSelect(bool selected)
{
    m_itemInfoSelectHash["sys"] = selected;
}

void ArchitePlanView::setChannelSelect(bool selected)
{
    m_itemInfoSelectHash["channel"] = selected;
}

void ArchitePlanView::setAnalogSelect(bool selected)
{
    m_itemInfoSelectHash["analog"] = selected;
}

void ArchitePlanView::setSizeSelect(bool selected)
{
    m_itemInfoSelectHash["size"] = selected;
}







GraphicsView *ArchitePlanView::viewFromChildItem(QStandardItem *childItem)
{
    QMap<QStandardItem*,int>itemMap;
    itemMap = m_treeView->getTreeIndexMap();
    GraphicsView*widget = nullptr;
    if(childItem!=nullptr)
    {
        if(childItem->parent()!=nullptr)
        {
            int page =itemMap[childItem];
            widget = m_widgetHash.value(page);
        }
    }
    return widget;

}

QList<GraphicsView *> ArchitePlanView::viewsFromParentItem(QStandardItem *parentItem)
{
    QMap<QStandardItem*,int>itemMap;
    itemMap = m_treeView->getTreeIndexMap();
    QList<GraphicsView*>viewList;
    if(parentItem->hasChildren())
    {
        for(int i=0;i<parentItem->rowCount();i++)
        {
            QStandardItem*childItem =  parentItem->child(i);
            int chileItemPage = itemMap[childItem];
            GraphicsView*childWidget = m_widgetHash.value(chileItemPage);
            viewList.push_back(childWidget);
        }
    }
    return viewList;
}


QList<GraphicsView *> ArchitePlanView::haveAlarms(const QString &alarm)
{
    QList<GraphicsView *>viewList;
    for(int i=0;i<m_stackedWidget->count();i++)
    {
        GraphicsView*view =dynamic_cast<GraphicsView*>(m_stackedWidget->widget(i));

        if(view!=nullptr)
        {
            if(view->haveAlarmType(alarm))
            {
                if(!viewList.contains(view))
                {
                    viewList.push_back(view);
                }
            }
        }


    }
    return viewList;
}

void ArchitePlanView::saveMySqlInfo(const QString &hostName, const QString &userName, const QString &password, const QString &databaseName, int port)
{
    QHash<QString,QVariant> mySqlInfoHash;
    mySqlInfoHash["hostName"] = hostName;
    mySqlInfoHash["userName"] = userName;
    mySqlInfoHash["password"] = password;
    mySqlInfoHash["databaseName"] = databaseName;
    mySqlInfoHash["port"] = port;
    QString mySqlInfoPath=  QCoreApplication::applicationDirPath()+"/mySqlInfo.json";
    QmlForJson qmlForJson;
    qmlForJson.writeFile(mySqlInfoHash,mySqlInfoPath);
}

QString ArchitePlanView::deviceSysName(const QString &extNum)
{
    QString sysName = "";
    sysName.clear();
    foreach (GraphicsView *view, m_widgetHash.values())
    {
        if(view!=nullptr)
        {
            QList<QGraphicsItem *>itemList =   view->getItemList();
            foreach(QGraphicsItem *item,itemList)
            {
                GraphicsItem *currentItem = dynamic_cast<GraphicsItem *>(item);
                if(currentItem!=nullptr)
                {
                    if(currentItem->extNum()==extNum)
                    {
                        sysName = currentItem->sysOfDevice();
                        break;
                    }
                }
            }
        }
        if(!sysName.isEmpty())
        {
            break;
        }
    }
    return sysName;
}

void ArchitePlanView::clearAllGraphicsTextItem()
{
    foreach (GraphicsView*view, m_alarmViewList)
    {
        if(view!=nullptr)
        {
            //disconnect(view,&GraphicsView::currentScaleValue,0,0);
            view->clearGraphicsTextItem();
        }
    }
}

void ArchitePlanView::itemIconSetting()
{
    QMetaObject::invokeMethod(m_itemSettingObj,"initIconSetting");
}



bool &ArchitePlanView::itemLimit()
{
    return m_itemLimit;
}

void ArchitePlanView::setItemLimit(bool isOk)
{
    m_itemLimit = isOk;
}
