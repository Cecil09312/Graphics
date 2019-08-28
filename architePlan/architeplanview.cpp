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

QMap<int,GraphicsView *>ArchitePlanView::m_widgetMap =QMap<int,GraphicsView *>();

ArchitePlanView::ArchitePlanView(QWidget *parent)
    : QWidget(parent),
      m_currentAlarmType("全部"),
      m_alarmPos(0),
      m_alarmNum(0)
{

    initWidget();
    setGlobalArchiteFromJson();
    initFromJsonFile();
    setContextMenuPolicy(Qt::CustomContextMenu);

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
                scene->removeGraphicsItem(scene->currentScenePos());
            }
        }
    });

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
                    QMetaObject::invokeMethod(m_itemSettingObj,"setEquipmentModel",Q_ARG(QVariant,currentItem->equipmentModel()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setSysOfDevice",Q_ARG(QVariant,currentItem->sysOfDevice()));

                    QMetaObject::invokeMethod(m_itemSettingObj,"setChannelNum",Q_ARG(QVariant,QString::number(currentItem->channelNum())));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setAnalogType",Q_ARG(QVariant,currentItem->analogType()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setDeviceLocation",Q_ARG(QVariant,currentItem->deviceLocation()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setPeriodOfValidity",Q_ARG(QVariant,currentItem->periodOfValidity()));
                    QMetaObject::invokeMethod(m_itemSettingObj,"setManufacturers",Q_ARG(QVariant,currentItem->manufacturers()));

                    QMetaObject::invokeMethod(m_itemSettingObj,"setOperator",Q_ARG(QVariant,currentItem->deviceOperator()));

                }
                else
                {
                    QMetaObject::invokeMethod(m_itemSettingObj,"clearItemInfo");
                }

                m_itemSettingView->close();
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
                    foreach (QGraphicsItem*item, scene->getItemList())
                    {
                        scene->removeItem(item);
                    }
                    scene->getItemList().clear();
                    DataStore::clearTypeItem();
                }
                else
                {
                    QMessageBox::warning(nullptr,tr("警告"),tr("存在报警信息，不能被清空"));
                }
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
                        if(graphicsItem->currentState()!=tr("正常"))
                        {
                            isHavingAlarm = true;
                            break;
                        }
                    }
                }

                if(!isHavingAlarm)
                {
                    foreach (QGraphicsItem*item,itemList)
                    {
                        GraphicsItem *graphicsItem = dynamic_cast<GraphicsItem*>(item);

                        if(graphicsItem!=nullptr)
                        {
                            scene->removeItem(graphicsItem);
                            scene->getItemList().removeOne(graphicsItem);
                            DataStore::deleteTypeItem(graphicsItem);
                        }
                    }
                }
                else
                {
                    QMessageBox::warning(nullptr,tr("警告"),tr("存在报警信息，不能被删除"));
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
                    QMetaObject::invokeMethod(maintObj,"setMaintTime",Q_ARG(QVariant,QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));
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
    Q_ASSERT(m_itemSettingObj);
    connect(m_itemSettingObj,SIGNAL(setSize(qreal)),this,SLOT(setItemSize(qreal)));
    connect(m_itemSettingObj,SIGNAL(setIcon(QString)),this,SLOT(setItemIcon(QString)));
    connect(m_itemSettingObj,SIGNAL(setItemInfo(QString,QString)),this,SLOT(setItemInfoFromType(const QString , const QString &)));

    connect(m_itemSettingObj,SIGNAL(setItemsManufacturers(int,QString)),this,SLOT(setItemsManufacturers(int,QString)));
    connect(m_itemSettingObj,SIGNAL(setItemsIcon(int,QString)),this,SLOT(setItemsIcon(int,QString)));
    connect(m_itemSettingObj,SIGNAL(setItemsPeriodOfValidity(int,QString)),this,SLOT(setItemsPeriodOfValidity(int,QString)));
    connect(m_itemSettingObj,SIGNAL(setItemsDeviceName(int,QString)),this,SLOT(setItemsEquipmentModel(int,QString)));

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::addGlobalItem,this,[=](GlobalGraphicsItem*item)
    {
        if(item!=nullptr)
        {
            QStandardItem *standardItem =  m_treeView->addRootItem(item->buildName());
            if(standardItem!=nullptr)
            {
                m_globalToArchitePlanHash[item] = standardItem;
            }
        }

    });

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::deleteGlobalItem,this,[=](GlobalGraphicsItem*item)
    {
        if(m_globalToArchitePlanHash.contains(item))
        {
            QModelIndex index= m_globalToArchitePlanHash[item]->index();
            m_treeView->deleteTreeItem(index);
        }
    });

    connect(m_globalGraphicsView->currentScene(),&GlobalGraphicsScene::clearItem,this,[=]()
    {
        m_treeView->clearItem();
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
        }

    });

    connect(m_autoSwitchTimer,&CustomTimer::timeout,this,&ArchitePlanView::viewsAutoSwitch);


}

ArchitePlanView::~ArchitePlanView()
{
    // m_dockWidget->close();
    m_firstFireWidget->close();
    m_autoSwitchTimer->stop();
    m_autoSwitchTimer->deleteLater();
    m_sqliteManager->close();
    m_sqliteManager->deleteLater();
    delete m_graphicsItemSettingMenu;
    m_itemSettingView->close();
    m_analogAlarmView->close();
    m_maintenanceView->close();
    m_itemSettingView->deleteLater();
    m_analogAlarmView->deleteLater();
    m_maintenanceView->deleteLater();
}


void ArchitePlanView::createAlarm(const QString&extNum,const QString&loopNum,const QString&addressNum,const QString&networkNum,const QString &alarmTypeName,bool isAnalog,const QString &alarmTime)
{

    foreach (GraphicsView *view, m_widgetMap.values())
    {
        if(view!=nullptr)
        {
            QList<QGraphicsItem *>itemList =   view->getItemList();
            foreach(QGraphicsItem *item,itemList)
            {
                GraphicsItem *currentItem = dynamic_cast<GraphicsItem *>(item);
                if(currentItem!=nullptr)
                {
                    if(currentItem->extNum()==extNum&&currentItem->loopNum()==loopNum&&currentItem->addrNum()==addressNum&&currentItem->networkNum()==networkNum)
                    {
                        if(!DataStore::getTypeItemList(alarmTypeName).contains(currentItem))
                        {
                            generateAlarm(alarmTypeName,alarmTime,currentItem,isAnalog);
                        }
                        break;
                    }
                }

            }
        }
    }
}


void ArchitePlanView::eliminateAlarm(GraphicsItem *item, const QString &alarmType, const QString &alarmReplyTime)
{
    if(item==nullptr)
    {
        return;
    }


    QString oldState = item->alarmState(alarmType);
    if(oldState!=tr("正常"))
    {
        oldState = alarmType;
    }

    item->removeAlarmRecord(alarmType,alarmReplyTime);
    item->stopAnimations();
    item->stopColorAnimation();
    item->setColorEffectValue(0.0);
    item->restoreSize();
    GraphicsView*curView = DataStore::itemDisplayView(item);
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
            deleteAlarmWidget(tr("全部"),curView);
        }
        updateAlarmWidget(curView);
    }
    filterAlarm(item,item->currentState());


    QList<QString> currentAlarmTextList=m_speechTextFromItemHash.value(item);
    foreach (QString alarmText, currentAlarmTextList)
    {
        if(oldState.endsWith("火警"))
        {
            if(alarmText.startsWith(tr("首火警")))
            {
                m_firstFireWidget->close();
                Controller::instance()->getSpeechObj()->removeAlarmText(alarmText);
            }


        }
        else
        {
            if(alarmText.startsWith(oldState))
            {
                Controller::instance()->getSpeechObj()->removeAlarmText(alarmText);
            }
        }

    }

    DataStore::deleteTypeItem(oldState,item);

    emit  eliminateAlarmFromTable(item,oldState);
    if(oldState!=tr("正常"))
    {
        emit alarmHappend(oldState);
    }

}

void ArchitePlanView::eliminateAlarm(const QString &extNum, const QString &loopNum, const QString &addrNum,const QString &networkNum,const QString &alarmType)
{
    foreach (GraphicsView *view, m_widgetMap.values())
    {
        if(view!=nullptr)
        {
            QList<QGraphicsItem *>itemList =   view->getItemList();
            foreach(QGraphicsItem *item,itemList)
            {
                GraphicsItem *currentItem = dynamic_cast<GraphicsItem *>(item);
                if(currentItem!=nullptr)
                {
                    if(currentItem->extNum()==extNum&&currentItem->loopNum()==loopNum&&currentItem->addrNum()==addrNum&&currentItem->networkNum()==networkNum)
                    {

                        QString replyTime=  QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
                        eliminateAlarm(currentItem,alarmType,replyTime);
//
                        return;
                    }
                }
            }
        }
    }
}

void ArchitePlanView::generateAlarm(const QString &alarmTypeName, const QString &alarmTime, GraphicsItem *item,  bool isAnalog)
{
    if(item!=nullptr)
    {
        QString state = item->alarmState(alarmTypeName);
        if(state!=tr("正常")&&!state.isEmpty())
        {
            return;
        }
        QString type = "";
        if(isAnalog)
        {
            type = tr("模拟")+alarmTypeName;
        }
        else
        {
            type = alarmTypeName;
        }
        item->setAlarmRecord(type,alarmTime,alarmTypeName);
        DataStore::insertTypeItem(alarmTypeName,item);
        filterAlarm(item,alarmTypeName);
        emit alarmItem(item,type);
        emit alarmHappend(alarmTypeName);
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
    m_firstFireWidget = new FirstFireAlarmInfoWidget();
    m_treeView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_treeView->setMaximumWidth(180);
    m_stackedWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_stackedWidget->setStyleSheet("QStackedWidget{border:1px solid black}");
    m_treeView->setStyleSheet("QTreeView{border:1px solid black}");
    QSplitter *splitter = new QSplitter(this);
    splitter->setChildrenCollapsible(false);
    QVBoxLayout*globalVLayout = new QVBoxLayout;
    splitter->addWidget(m_stackedWidget);
    splitter->addWidget(m_treeView);

    m_tabWidget->addTab(m_globalGraphicsView ,tr("总平面布局图"));
    m_tabWidget->addTab(splitter,tr("建筑平面图"));
    m_tabWidget->addTab(m_sysArchitePlanView,tr("系统图"));

    globalVLayout->addWidget(m_tabWidget);
    globalVLayout->setContentsMargins(QMargins(0,0,0,0));
    setLayout(globalVLayout);
    Controller::instance()->setSysArchitePlanView(m_sysArchitePlanView);
    QString dbName = QCoreApplication::applicationDirPath()+"/architeInfo.db";
    m_architeInfoDbName = dbName;
    m_sqliteManager = SqlManager::fromDriver("QSQLITE");

    m_sqliteManager->setDataBase("QSQLITE","info", "",
                                 "","",dbName,8888);
    m_sqliteManager->open();
    if(m_sqliteManager->isOpen())
    {
        QStringList tableNameList = m_sqliteManager->getTables();
        QStringList itemInfoTableList;
        itemInfoTableList<<"extNum text" << "addrNum text" << "loopNum text" << "networkNum text"<< "buildingName text" << "currentState text"
                        << "deviceLocation text" << "deviceNum text" << "equipmentModel text" << "floorOfDevice text"
                        << "iconName text"<<"manufacturers text"<<"periodOfValidity text"<<"pos text"<<"size double"
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
    QMenu *modeSelectMenu = new QMenu("模式选择",m_graphicsItemSettingMenu);
    m_deleteAction = new QAction(tr("删除"),m_graphicsItemSettingMenu);
    m_editAction = new QAction(tr("编辑"),m_graphicsItemSettingMenu);
    m_clearAction = new QAction(tr("清空"),m_graphicsItemSettingMenu);
    m_deleteSelectedAction = new QAction(tr("删除选中"),m_graphicsItemSettingMenu);
    m_closeAction= new QAction(tr("关闭"),m_graphicsItemSettingMenu);
    m_analogAlarmAction = new QAction(tr("报警模拟"),m_graphicsItemSettingMenu);
    m_maintenanceAction = new QAction(tr("设备维保"),m_graphicsItemSettingMenu);
    m_handDragAction = new QAction(tr("手动拖拽模式"),modeSelectMenu);
    m_rubberBandDragAction = new QAction(tr("橡皮筋模式"),modeSelectMenu);
    m_itemTextVisiableAction = new QAction(tr("文字可见"),m_graphicsItemSettingMenu);
    m_modeActionGroup = new QActionGroup(this);
    modeSelectMenu->addAction(m_handDragAction);
    modeSelectMenu->addAction(m_rubberBandDragAction);
    m_modeActionGroup->addAction(m_rubberBandDragAction);
    m_modeActionGroup->addAction(m_handDragAction);

    m_modeActionGroup->setExclusive(true);
    m_handDragAction->setCheckable(true);
    m_rubberBandDragAction->setCheckable(true);
    m_itemTextVisiableAction->setCheckable(true);
    m_handDragAction->setChecked(true);
    m_graphicsItemSettingMenu->addMenu(modeSelectMenu);
    m_itemSettingView = new QQuickView;
    m_itemSettingView->setSource(QUrl("qrc:/qml/itemSetting/GraphicsItemEditor.qml"));
    m_itemSettingView->setTitle(tr("设备信息设置界面"));

    m_itemSettingView->rootContext()->setContextProperty("ArchitePlanView",this);
    m_analogAlarmView = new QQuickView;
    m_analogAlarmView->setSource(QUrl("qrc:/qml/itemSetting/AnalogAlarmItem.qml"));
    m_analogAlarmView->setTitle(tr("报警模拟界面"));
    m_analogAlarmView->rootContext()->setContextProperty("ArchitePlanView",this);
    m_maintenanceView = new QQuickView;
    m_maintenanceView->setSource(QUrl("qrc:/qml/infoSetting/MaintenanceInfo.qml"));
    m_maintenanceView->setTitle(tr("设备维保"));
    m_itemSettingObj= m_itemSettingView->rootObject();
    m_graphicsItemSettingMenu->addAction(m_itemTextVisiableAction);
    m_graphicsItemSettingMenu->addAction(m_editAction);
    m_graphicsItemSettingMenu->addAction(m_analogAlarmAction);
    m_graphicsItemSettingMenu->addAction(m_maintenanceAction);
    m_graphicsItemSettingMenu->addAction(m_deleteAction);
    m_graphicsItemSettingMenu->addAction(m_deleteSelectedAction);
    m_graphicsItemSettingMenu->addAction(m_clearAction);
    m_graphicsItemSettingMenu->addAction(m_closeAction);



    connect(m_treeView,&TreeView::treeIndex,this,[=](QStandardItem*item)
    {
        QMap<QStandardItem*,int>map= m_treeView->getTreeIndexMap();
        if(item!=nullptr)
        {
            int page = map[item];
            if(m_widgetMap[page]==nullptr)
            {
                GraphicsView *widget = new GraphicsView(this);
                m_widgetMap[page]=widget;
                m_stackedWidget->addWidget(widget);
                m_itemToViewHash[item] = widget;
            }
        }

    });
    connect(m_treeView,&TreeView::clicked,this,[=](const QModelIndex&index)
    {
        int page=0;
        QStandardItemModel*model =dynamic_cast<QStandardItemModel*>(m_treeView->model());
        QStandardItem *item= model->itemFromIndex(index);
        QMap<QStandardItem*,int>itemMap;
        itemMap = m_treeView->getTreeIndexMap();
        page =itemMap[item];
        if(m_widgetMap[page]!=nullptr)
        {
            m_stackedWidget->setCurrentWidget(m_widgetMap[page]);
            GraphicsView *currentView = dynamic_cast<GraphicsView *>(m_widgetMap[page]);
            updateAlarmWidget(currentView);
        }

    });

    connect(m_treeView,&TreeView::clearIndex,this,[=]()
    {
        int count = m_widgetMap.size();
        for(int i=0;i<count;i++)
        {
            QWidget *widget = m_widgetMap.values().at(i);
            if(widget!=nullptr)
            {
                m_stackedWidget->removeWidget(widget);
                delete widget;
                widget = nullptr;
            }

        }
        m_widgetMap.clear();
        m_globalGraphicsView->currentScene()->clearGraphicsItem();
        emit noPage();
    });

    connect(m_treeView,&TreeView::deleteIndex,this,&ArchitePlanView::deleteViewFromItem);

    connect(m_treeView,&TreeView::insertAnchPixmap,this,[=](QStandardItem*item,const QString &fileName)
    {
        QMap<QStandardItem*,int>itemMap;
        itemMap = m_treeView->getTreeIndexMap();
        int page =itemMap[item];
        GraphicsView*widget = m_widgetMap[page];
        if(widget!=nullptr)
        {
            widget->loadPixmap(fileName);
        }
    });

    connect(m_stackedWidget,&QStackedWidget::currentChanged,this,[=](int index)
    {
        QList<GraphicsView*>viewList = m_alarmWidgetHash[m_currentAlarmType];
        int count = viewList.size();
        if(count>1)
        {
            if(index<m_widgetMap.size())
            {
                GraphicsView *view = m_widgetMap[index];
                int currentIndex= viewList.indexOf(view);
                if(currentIndex==0)
                {
                    emit toFirstPage();
                }
                else if(currentIndex>0 && currentIndex <count-1)
                {
                    emit normalPage();
                }
                else if(currentIndex==count-1)
                {
                    emit toLastPage();
                }
                else if(currentIndex<0)
                {
                    emit noPage();
                }
            }
        }
        else
        {
            emit noPage();
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

    connect(m_tabWidget,&QTabWidget::currentChanged,this,&ArchitePlanView::tabIndex);
    connect(m_firstFireWidget,&FirstFireAlarmInfoWidget::toFirstFire,this,&ArchitePlanView::firstFireAlarm);


}

void ArchitePlanView::showMenu(const QPoint &point)
{

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
            }
            else
            {
                m_itemTextVisiableAction->setEnabled(false);
                m_itemTextVisiableAction->setChecked(false);
            }

            if(userRight==UserManager::Super||userRight==UserManager::Engineer)
            {
                if(!itemList.isEmpty())
                {

                    if(itemList.contains(selectItem))
                    {
                        m_deleteAction->setEnabled(true);
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
                        m_deleteSelectedAction->setEnabled(true);
                    }

                    m_clearAction->setEnabled(true);
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
    QStandardItemModel *model=dynamic_cast<QStandardItemModel *>(m_treeView->model());
    for(int i=0;i<model->rowCount();i++)
    {
        QStandardItem *parentItem = model->item(i);
        if(parentItem!=nullptr)
        {
            QHash<QString,QVariant> rootImageHash =saveViewInfo(parentItem);
            JsonEdit::instance()->setRootImage(model->indexFromItem(parentItem).row(),rootImageHash);

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
    saveArchiteInfoToDb();
}

void ArchitePlanView::autoFitView(QGraphicsView *view)
{
    QList< QList<QGraphicsItem *> >list= DataStore::getTypeItemHash().values();
    QList<QGraphicsItem *> alarmItemList;
    foreach (QList<QGraphicsItem *> itemList, list)
    {
        foreach (QGraphicsItem * item, itemList)
        {
            alarmItemList.push_back(item);
        }
    }

    // QHash<QGraphicsView*,QList<QGraphicsItem *> >itemToViewHash;

    GraphicsView *currentView = dynamic_cast<GraphicsView *>(view);
    if(currentView !=nullptr)
    {
        //        foreach (QGraphicsItem *alarmItem, alarmItemList)
        //        {
        //            if(currentView->getItemList().contains(alarmItem))
        //            {
        //                itemToViewHash[currentView].push_back(alarmItem);
        //            }
        //        }

        //        qreal xMin=10000.0*1.0,xMax=0,yMin=10000.0*1.0,yMax=0,size =0;
        //        QList<QGraphicsItem *>currentItemList = itemToViewHash[view];
        //        foreach(QGraphicsItem *graphicsItem,currentItemList)
        //        {
        //            GraphicsItem *item = dynamic_cast<GraphicsItem *>(graphicsItem);
        //            //size=item->scale()*item->radius()*2*10;
        //            size = view->scene()->width();
        //            if(item!=nullptr)
        //            {
        //                xMin= qMin(xMin,item->pos().x());
        //                yMin= qMin(yMin,item->pos().y());
        //                xMax= qMax(xMax,item->pos().x());
        //                yMax= qMax(yMax,item->pos().y());
        //            }
        //        }

        m_stackedWidget->setCurrentWidget(currentView);
        QRectF currentRectF = currentView->scene()->sceneRect();
        currentView->fitInView(currentRectF.adjusted(0,0,currentView->pos().x(),view->pos().y()),Qt::KeepAspectRatio);

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
    QList<int> keyValueList= m_widgetMap.keys();
    foreach (int value, keyValueList)
    {
        GraphicsView *view=  m_widgetMap.value(value);
        if(view!=nullptr)
        {
            QList<QVariant>valueList,extNumList,addrNumList,loopNumList,networkNumList,buildingNameList,
                    currentStateList,deviceLocationList,deviceNumList,equipmentModelList,floorOfDeviceList,
                    iconNameList,manufacturersList,periodOfValidityList,posList,sizeList,sysOfDeviceList,operatorList,analogTypeList,channelNumList;
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
                    buildingNameList.push_back(currentItem->buildingName());
                    currentStateList.push_back(currentItem->currentState());
                    deviceLocationList.push_back(currentItem->deviceLocation());
                    deviceNumList.push_back(currentItem->deviceNum());
                    equipmentModelList.push_back(currentItem->equipmentModel());
                    floorOfDeviceList.push_back(currentItem->floorOfDevice());
                    iconNameList.push_back(currentItem->iconName());
                    manufacturersList.push_back(currentItem->manufacturers());
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
            valueList.push_back(buildingNameList);
            valueList.push_back(currentStateList);
            valueList.push_back(deviceLocationList);
            valueList.push_back(deviceNumList);
            valueList.push_back(equipmentModelList);
            valueList.push_back(floorOfDeviceList);
            valueList.push_back(iconNameList);
            valueList.push_back(manufacturersList);
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
    GraphicsView*widget = m_widgetMap[page];
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
    bool isHave = false;
    foreach (QGraphicsItem*item, globalScene->items()) {
        GlobalGraphicsItem*gItem = dynamic_cast<GlobalGraphicsItem*>(item);
        if(gItem!=nullptr)
        {
            isHave= true;
            break;
        }
    }

    if(isHave)
    {
        QList<QVariant>   jsonValueList=JsonEdit::instance()->readFile(c_jsonFilePath).toList();
        for(int i=0;i<jsonValueList.size();i++)
        {
            QHash<QString,QVariant> parentHash=  jsonValueList.at(i).toHash();
            if(!parentHash.isEmpty())
            {
                QStandardItem *parentItem= m_treeView->addRootItem(parentHash["name"].toString());
                QList<QGraphicsItem*>  globalItemList=     m_globalGraphicsView->currentScene()->items();
                foreach (QGraphicsItem*curItem, globalItemList)
                {
                    GlobalGraphicsItem *globalGraphicsItem = dynamic_cast<GlobalGraphicsItem *>(curItem);
                    if(globalGraphicsItem!=nullptr)
                    {
                        if(globalGraphicsItem->buildName() ==parentItem->text())
                        {
                            m_globalToArchitePlanHash[globalGraphicsItem] = parentItem;
                            break;
                        }
                    }
                }

                QList<QVariant> childList =  parentHash["child"].toList();

                for(int j=0;j<childList.size();j++)
                {
                    QHash<QString,QVariant> childHash=  childList.at(j).toHash();
                    QModelIndex parentIndex= parentItem->index();

                    if(parentIndex.isValid())
                    {
                        QStandardItem *childItem= m_treeView->addChildItem(parentIndex);
                        if(childItem)
                        {
                            childItem->setText(childHash["name"].toString());
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

void ArchitePlanView::initFromDataBase(GraphicsView *view,const QString &buildingName,const QString &floor)
{
    QStringList valueList=  m_sqliteManager->executeQuery(QString("select *from ItemInfo where buildingName ='%1' and floorOfDevice = '%2'").arg(buildingName).arg(floor));
    int valueSize = valueList.size();
    if(view!=nullptr)
    {
        if(valueSize>=m_itemInfoTableSize&&valueSize%m_itemInfoTableSize==0)
        {
            for(int j=0;j<valueSize;j=j+m_itemInfoTableSize)
            {
                QGraphicsScene *scene = view->scene();
                GraphicsScene *graphicsScene = dynamic_cast<GraphicsScene*>(scene);
                if(valueList.size()>j+18)
                {
                    GraphicsItem *item = new GraphicsItem(graphicsScene);
                    item->extNum() =valueList.at(j);
                    item->addrNum() = valueList.at(j+1);
                    item->loopNum() =valueList.at(j+2);
                    item->networkNum() =valueList.at(j+3);
                    item->buildingName() = valueList.at(j+4);
                    item->currentState() =valueList.at(j+5);
                    item->deviceLocation() = valueList.at(j+6);
                    item->deviceNum() =valueList.at(j+7);
                    item->equipmentModel() = valueList.at(j+8);
                    item->floorOfDevice() =valueList.at(j+9);
                    item->setIconName( valueList.at(j+10));
                    item->manufacturers() =valueList.at(j+11);
                    item->periodOfValidity() = valueList.at(j+12);
                    QString posStr = valueList.at(j+13);
                    item->setPos(QPointF(posStr.section(",",0,0).toDouble(),posStr.section(",",1,1).toDouble()));
                    QString sizeStr = valueList.at(j+14);
                    item->setRadius(sizeStr.toDouble());
                    item->sysOfDevice() =valueList.at(j+15);
                    item->deviceOperator() = valueList.at(j+16);
                    item->analogType() = valueList.at(j+17);
                    QString channelNumStr = valueList.at(j+18);
                    item->channelNum() = channelNumStr.toInt();
                    if(graphicsScene!=nullptr)
                    {
                        graphicsScene->addItem(item);
                        graphicsScene->getItemList().push_back(item);
                    }
                }
            }
        }
    }
}

GraphicsView* ArchitePlanView::setViewFromJson(const QHash<QString,QVariant> &hash,QStandardItem *treeItem)
{

    if(!hash.isEmpty())
    {
        QMap<QStandardItem*,int>parentItemMap;
        parentItemMap = m_treeView->getTreeIndexMap();
        int page =parentItemMap[treeItem];
        GraphicsView*widget = m_widgetMap[page];
        if(widget!=nullptr)
        {
            widget->loadPixmap(hash["path"].toString());
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
    QList<QGraphicsItem*>itemList= DataStore::getTypeItemList(tr("火警"));
    int listSize = itemList.size();

    QList<GraphicsView *>viewList = m_widgetMap.values();
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
                            m_stackedWidget->setCurrentWidget(currentView);
                            autoFitView(currentView);
                            return;
                        }
                    }
                    else
                    {
                        if(currentItem==itemList.at(listSize-1))
                        {
                            m_tabWidget->setCurrentIndex(1);
                            m_stackedWidget->setCurrentWidget(currentView);
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

    m_sqliteManager->executeQuery("delete from GlobalArchite");
    QList<QGraphicsItem*> itemList=  m_globalGraphicsView->currentScene()->items();
    QList<QVariant>buildingNameList,personOnDutyList,posList,sizeList,iconNameList,valueList;
    foreach (QGraphicsItem*graphicsItem, itemList)
    {
        GlobalGraphicsItem *globalItem = dynamic_cast<GlobalGraphicsItem *>(graphicsItem);
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
        QString state = item->alarmState(alarmType);
        if(state!=tr("正常")&&!state.isEmpty())
        {
            return;
        }
        item->setAlarmRecord(alarmType,alarmTime,alarmState);
        DataStore::insertTypeItem(alarmType,item);
        filterAlarm(item,alarmType);
        emit alarmItem(item,alarmType);
        emit alarmHappend(alarmType);
    }
}

GraphicsItem *ArchitePlanView::itemFormInfo(const QString &extNum, const QString &loopNum, const QString &addressNum, const QString &networkNum)
{
    GraphicsItem *graphicsItem = nullptr;
    bool isFind = false;
    QList<GraphicsView *>viewList = m_widgetMap.values();

    foreach (GraphicsView *currentView, viewList)
    {

        QList<QGraphicsItem*> itemList= currentView->getItemList();
        foreach (QGraphicsItem*item, itemList)
        {
            GraphicsItem *curItem = dynamic_cast<GraphicsItem *>(item);
            if(curItem->extNum()==extNum&&curItem->loopNum()==loopNum&&curItem->addrNum()==addressNum&&curItem->networkNum()==networkNum)
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

void ArchitePlanView::updateAlarmState(const QString &extNum, const QString &loopNum, const QString &addressNum, const QString &networkNum, const QString &curAlarmState)
{
    GraphicsItem *item = itemFormInfo(extNum,loopNum,addressNum,networkNum);
    if(item!=nullptr)
    {
        if(item->currentState() != curAlarmState)
        {
            item->currentState() = curAlarmState;
            emit alarmStateUpdate(extNum,loopNum,addressNum,networkNum,curAlarmState);
        }
    }
}

void ArchitePlanView::closeQuickView()
{
    m_itemSettingView->close();
    m_analogAlarmView->close();
    m_maintenanceView->close();
    m_globalGraphicsView->currentScene()->closeQuickView();
    m_treeView->closeQuickView();
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
        if(totalPage()>1)
        {
            if(currentIndex>0&&currentIndex<totalPage()-1)
            {
                emit normalPage();
            }
            else if(currentIndex==0)
            {
                emit toFirstPage();

            }
            else if(currentIndex==totalPage()-1)
            {
                emit toLastPage();
            }
        }
        else if(totalPage()<=1)
        {
            emit noPage();
        }
    }

}

void ArchitePlanView::deleteAlarmWidget(GraphicsView *currentView)
{
    if(m_currentAlarmType!="全部")
    {
        if(m_alarmWidgetHash.value("全部").contains(currentView))
        {
            m_alarmWidgetHash["全部"].removeOne(currentView);
        }
    }
    if(m_alarmWidgetHash.value(m_currentAlarmType).contains(currentView))
    {
        m_alarmWidgetHash[m_currentAlarmType].removeOne(currentView);
    }
    GraphicsView*view =dynamic_cast<GraphicsView*>(m_stackedWidget->currentWidget());
    updateAlarmWidget(view);
}

void ArchitePlanView::filterAlarm(GraphicsItem *item, const QString &alarmType)
{
    if(item!=nullptr)
    {

        item->stopAnimations();
        QString curState=  item->currentState();
        QString curType = item->alarmType();

        if(curState!=tr("正常")&&!curState.isEmpty())
        {
            QString speechText="";
            static GraphicsView * firstFireAlarmView = nullptr,*firstLinkAlarmView= nullptr;
            GraphicsView *view=DataStore::itemDisplayView(item);
            if(curState.endsWith(tr("故障"))||curState.endsWith(tr("屏蔽")))
            {
                item->setColorEndValue(QColor("yellow"));
            }
            else
            {
                item->setColorEndValue(QColor("red"));
            }
            QList<QGraphicsItem*> itemList= DataStore::getTypeItemList(curState);
            if(!itemList.isEmpty())
            {
                GraphicsItem *curItem = dynamic_cast<GraphicsItem*>(itemList.at(0));
                if(curItem==item)
                {
                    if(curState.endsWith(tr("火警")))
                    {
                        item->startScaleAnimation();
                        item->startColorAnimation();
                        firstFireAlarmView = view;
                        m_firstFireWidget->setFirstFireInfo(item);
                        m_firstFireWidget->show();
                        speechText = tr("首火警");
                    }
                    else
                    {
                        if(curState.endsWith(tr("启动")))
                        {
                            firstLinkAlarmView = view;
                        }
                        item->startAnimations();
                        //item->startRotationAnimation();
                        speechText = curType+tr("报警");
                    }
                }
                else
                {
                    item->startColorAnimation();
                    if(!curState.endsWith(tr("火警")))
                    {
                        speechText = curType+tr("报警");
                    }
                    else
                    {
                        speechText = curType;
                    }
                }
                speechText += ","+item->buildingName()+","+item->floorOfDevice()+","+item->deviceLocation();
                Controller::instance()->getSpeechObj()->insertAlarmText(speechText);
                m_speechTextFromItemHash[item].push_back(speechText);

                if(view!=nullptr)
                {
                    insertAlarmWidget(alarmType,view);
                    insertAlarmWidget("全部",view);
                    updateAlarmWidget(view);
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
                    if(firstFireAlarmView!=nullptr||firstLinkAlarmView!=nullptr)
                    {
                        if(firstFireAlarmView!=nullptr)
                        {
                            autoFitView(firstFireAlarmView);
                        }
                        else if(firstFireAlarmView==nullptr && firstLinkAlarmView!=nullptr)
                        {
                            autoFitView(firstLinkAlarmView);
                        }
                    }
                    else
                    {
                        autoFitView(view);
                    }

                }

                QStandardItem *parentItem =   getParnentItemFromView(view);
                if(parentItem!=nullptr)
                {
                    GlobalGraphicsItem*globalGraphicsItem=  m_globalToArchitePlanHash.key(parentItem);
                    if(globalGraphicsItem!=nullptr)
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
}



int ArchitePlanView::numOfTypeAlarm(const QString &type)
{
    int num= DataStore::numOfTypeItem(type);
    return num;
}

void ArchitePlanView::setGlobalArchitePixmap(const QString &pixmapName)
{
    QString filePath=  Controller::instance()->fileNameFromQml(pixmapName);
    m_globalArchitePlanPixmapName = filePath;
    m_globalGraphicsView->setPicture(filePath);
}

QMap<int, GraphicsView *> &ArchitePlanView::getWidgetMap()
{
    return m_widgetMap;
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

void ArchitePlanView::clearAlarm()
{
    QList< QList<QGraphicsItem *> >list= DataStore::getTypeItemHash().values();
    foreach (QList<QGraphicsItem *>itemList, list)
    {
        foreach (QGraphicsItem *currentItem, itemList)
        {
            GraphicsItem *item = dynamic_cast<GraphicsItem *>(currentItem);
            if(item!=nullptr)
            {
                item->stopAnimations();
                item->stopColorAnimation();
                item->setColorEffectValue(0.0);
                item->restoreSize();
                item->clearAllAlarm();
                emit findAlarmNum(0,0);
                //item->currentState() =tr("正常");
                //item->getItemInfo().m_currentState = tr("正常");
                QList<QString> currentSpeechTextList=m_speechTextFromItemHash.value(item);
                foreach (QString alarmText, currentSpeechTextList)
                {
                    if(!alarmText.isEmpty())
                    {
                        Controller::instance()->getSpeechObj()->removeAlarmText(alarmText);
                    }
                }


            }


        }
    }

    DataStore::clearTypeItem();
    clearAlarmWidget();
    m_alarmViewList.clear();
    emit noPage();
    emit reduInstruction();
    m_firstFireWidget->close();

    //m_dockWidget->close();
    QList<QGraphicsItem*> globalGraphicsItemList= m_globalGraphicsView->currentScene()->items();
    foreach (QGraphicsItem*item, globalGraphicsItemList)
    {
        GlobalGraphicsItem *globalGraphicsItem = dynamic_cast<GlobalGraphicsItem*>(item);
        if(globalGraphicsItem!=nullptr)
        {
            if(globalGraphicsItem->animalIsRunning())
            {
                globalGraphicsItem->startAnimal(false);
            }
        }
    }
    // Controller::instance()->getSpeechObj()->clearAlarmText();
    emit clearAlarmFromTable();
}

void ArchitePlanView::clearAlarmFromExtNum(const QString &extNum,const QString &rebackAlarmTime)
{
    QList< QList<QGraphicsItem *> >list= DataStore::getTypeItemHash().values();
    foreach (QList<QGraphicsItem *>itemList, list)
    {
        foreach (QGraphicsItem *currentItem, itemList)
        {
            GraphicsItem *item = dynamic_cast<GraphicsItem *>(currentItem);
            if(item!=nullptr)
            {
                if(item->extNum()==extNum)
                {
                    //item->getItemInfo().m_alarmReplyTime = rebackAlarmTime;
                    eliminateAlarm(item,item->alarmType(),rebackAlarmTime);
                }

            }
        }
    }

    QList<QString>typeNoItemList= DataStore::getTypeNoItemHash().keys();
    foreach (QString dataInfo, typeNoItemList)
    {
        QList<DataInfo *> infoList=  DataStore::getTypeNoItemHash().value(dataInfo);
        foreach (DataInfo *info, infoList)
        {
            if(info->m_extNum==extNum)
            {

                emit eliminateNoItemAlarm(info,rebackAlarmTime);
                DataStore::deleteDataInfo(info);
            }
        }
    }
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
        if(currentPage()==0)
        {
            emit toFirstPage();
        }
        else
        {
            emit normalPage();
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
        if(currentPage()==(count-1))
        {
            if(count==1)
            {
                emit noPage();
            }
            else
            {
                emit toLastPage();
            }

        }
        else
        {
            emit normalPage();
        }
    }
}


void ArchitePlanView::setCurrentAlarmType(const QString &type)
{
    m_currentAlarmType = type;
}

void ArchitePlanView::toArchitePlan(const QString &extNum, const QString &loopNum, const QString &addressNum,const QString &networkNum)
{
    foreach (GraphicsView *view, m_widgetMap.values())
    {
        if(view!=nullptr)
        {
            QList<QGraphicsItem *>itemList =   view->getItemList();
            foreach(QGraphicsItem *item,itemList)
            {
                GraphicsItem *currentItem = dynamic_cast<GraphicsItem *>(item);
                if(currentItem!=nullptr)
                {
                    if(currentItem->extNum()==extNum && currentItem->loopNum()==loopNum && currentItem->addrNum()==addressNum&&currentItem->networkNum()==networkNum)
                    {
                        if(m_tabWidget->count()>=2)
                        {
                            if(m_tabWidget->currentIndex()!=1)
                            {
                                m_tabWidget->setCurrentIndex(1);
                            }
                        }
                        m_stackedWidget->setCurrentWidget(view);
                        qreal size=currentItem->scale()*currentItem->radius()*8;
                        QRectF currentRectF = QRectF(currentItem->pos().x(),currentItem->pos().y(),size,size);
                        view->fitInView(currentRectF.adjusted(-size,-size,view->pos().x()/2,view->pos().y()/2),Qt::KeepAspectRatio);

                        if(m_currentAlarmType!=tr("全部"))
                        {
                            int totalNum = DataStore::numOfTypeItem(m_currentAlarmType);
                            int currentNum = DataStore::indexOfItem(extNum,loopNum,addressNum,networkNum,m_currentAlarmType);
                            emit findAlarmNum(totalNum,currentNum+1);
                        }
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

    if(item->parent()==nullptr)
    {
        GlobalGraphicsItem*globalGraphicsItem=  m_globalToArchitePlanHash.key(item);
        if(globalGraphicsItem!=nullptr)
        {
            GlobalGraphicsScene * scene = m_globalGraphicsView->currentScene();
            if(scene!=nullptr && scene->items().contains(globalGraphicsItem))
            {
                scene->removeItem(globalGraphicsItem);
                delete globalGraphicsItem;
                globalGraphicsItem= nullptr;
            }
        }

    }
    if(item->hasChildren())
    {
        for(int i=0;i<item->rowCount();i++)
        {
            QStandardItem*childItem =  item->child(i);
            int chileItemPage = itemMap[childItem];
            GraphicsView*childWidget = m_widgetMap[chileItemPage];
            if(childWidget!=nullptr)
            {
                m_stackedWidget->removeWidget(childWidget);
                deleteAlarmWidget(childWidget);
                delete childWidget;
                childWidget = nullptr;
            }

            m_widgetMap.remove(chileItemPage);
            m_treeView->getTreeIndexMap().remove(childItem);

        }
    }

    GlobalGraphicsItem*globalItem =  m_globalToArchitePlanHash.key(item);
    if(globalItem!=nullptr)
    {
        GlobalGraphicsScene *globalScene = m_globalGraphicsView->currentScene();
        if(globalScene!=nullptr)
        {
            if(globalScene->items().contains(globalItem))
            {
                globalScene->removeItem(globalItem);
                delete globalItem;
                globalItem = nullptr;
            }

        }

        // delete globalItem;
    }
    m_treeView->getTreeIndexMap().remove(item);

}

void ArchitePlanView::viewsAutoSwitch()
{
    if(m_tabWidget->count()>=2)
    {
        if(m_tabWidget->currentIndex()!=1)
        {
            m_tabWidget->setCurrentIndex(1);
        }

    }
    if(m_currentAlarmType==tr("全部"))
    {
       // emit findAlarmNum(0,0);
        QList<GraphicsView*> alarmViewList= haveAlarms(m_currentAlarmType);
        if(alarmViewList.size()>m_alarmPos)
        {
            autoFitView(alarmViewList.at(m_alarmPos));
            m_alarmPos++;
        }
        else
        {
            m_alarmPos =0;
        }
    }
    else
    {
        QList<QGraphicsItem*> gItemList= DataStore::getTypeItemList(m_currentAlarmType);
        if(m_alarmNum<gItemList.size())
        {
            QGraphicsItem*curItem = gItemList.at(m_alarmNum);
            GraphicsItem *item = dynamic_cast<GraphicsItem*>(curItem);
            if(item!=nullptr)
            {
                qreal size=item->scale()*item->radius()*8;
                QRectF currentRectF = QRectF(item->pos().x(),item->pos().y(),size,size);
                QGraphicsView *view = DataStore::itemDisplayView(item);
                if(view!=nullptr)
                {
                    m_stackedWidget->setCurrentWidget(view);
                    view->fitInView(currentRectF.adjusted(-size,-size,view->pos().x()/2,view->pos().y()/2),Qt::KeepAspectRatio);
                }
            }
            m_alarmNum++;
            emit findAlarmNum(gItemList.size(),m_alarmNum);
        }
        else
        {
            m_alarmNum=0;
        }
    }
}

void ArchitePlanView::startAutoSwitch(bool isAuto)
{
    if(isAuto)
    {
        if(!m_autoSwitchTimer->isActive())
        {
            m_autoSwitchTimer->start(1000);
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
            widget = m_widgetMap[page];
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
            GraphicsView*childWidget = m_widgetMap[chileItemPage];
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
        if(alarm==tr("全部"))
        {
            if(view!=nullptr)
            {
                if(view->haveAnyAlarm())
                {
                    if(!viewList.contains(view))
                    {
                        viewList.push_back(view);
                    }
                }
            }

        }
        else
        {
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
    foreach (GraphicsView *view, m_widgetMap.values())
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
