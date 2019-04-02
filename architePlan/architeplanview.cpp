#include "architeplanview.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QGLWidget>
#include <QOpenGLWidget>
#include "dataStore/datastore.h"
#include "jsonEdit/jsonedit.h"
#include "graphicsWidget/graphicsitem.h"
#include <QDebug>
#include "control/controller.h"
#include <QModelIndex>

QMap<int,GraphicsView *>ArchitePlanView::m_widgetMap =QMap<int,GraphicsView *>();

ArchitePlanView::ArchitePlanView(QWidget *parent)
    : QWidget(parent),
      m_currentAlarmType("全部"),
      m_alarmPos(0)
{

    initWidget();
    setGlobalArchiteFromJson();
    initFromJsonFile();

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
        QModelIndex index= m_globalToArchitePlanHash[item]->index();
        m_treeView->deleteTreeItem(index);
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

    connect(m_autoSwitchTimer,&QTimer::timeout,this,&ArchitePlanView::viewsAutoSwitch);

    connect(Controller::instance()->getCommObj(),&AbstractLink::getData,this,[=](const QByteArray&array)
    {
        QList<QByteArray>dataArrayList=  m_serialDataProtocol->frameData(array);
        foreach (QByteArray array, dataArrayList)
        {
            quint8 eventNum =  m_serialDataProtocol->dataByte(array,0);//事件
            QString loopNum = QString("%1").arg(m_serialDataProtocol->dataByte(array,1));//回路
            QString addrNum = QString("%1").arg(m_serialDataProtocol->dataByte(array,2));//地址
            quint8 type = m_serialDataProtocol->dataByte(array,3);//属性，左6位主机地址
            QString extNum = QString("%1").arg(type>>2&0x3f);

            quint8 year= m_serialDataProtocol->dataByte(array,4);
            quint8 month= m_serialDataProtocol->dataByte(array,5);
            quint8 date = m_serialDataProtocol->dataByte(array,6);
            quint8 hour = m_serialDataProtocol->dataByte(array,7);
            quint8 minute= m_serialDataProtocol->dataByte(array,8)>>1&0x7f;
            quint8 second= m_serialDataProtocol->dataByte(array,9);

            QString timeStr = QString("%1/%2/%3 %4:%5:%6").arg((int)year+2000).arg((ushort)month,2,10,QChar('0'))
                    .arg((ushort)date,2,10,QChar('0')).arg((ushort)hour,2,10,QChar('0')).arg((ushort)minute,2,10,QChar('0'))
                    .arg((ushort)second,2,10,QChar('0'));

            QHash<quint8,QString>alarmTypeHash,eliminateAlarmHash,commuStatusHash;
            alarmTypeHash[0x01] = tr("火警");
            alarmTypeHash[0x03] = tr("故障");
            alarmTypeHash[0x05] = tr("联动");
            alarmTypeHash[0x0a] = tr("监管");
            alarmTypeHash[0x0b] = tr("屏蔽");
            alarmTypeHash[0x0d] = tr("反馈");
            commuStatusHash[0x12] = tr("主电");
            commuStatusHash[0x13] = tr("备电");
            if(commuStatusHash.keys().contains(eventNum))
            {
                if(loopNum.toInt()==0)
                {
                   emit communicationStatus(commuStatusHash[eventNum],true);
                }
                else if(loopNum.toInt()==1)
                {
                    emit communicationStatus(commuStatusHash[eventNum],false);
                }

            }

            if(alarmTypeHash.keys().contains(eventNum))
            {
                createAlarm(extNum,loopNum,addrNum,alarmTypeHash[eventNum],false,timeStr);
            }

            //            alarmTypeHash[0x12] = tr("主电");
            //            alarmTypeHash[0x13] = tr("备电");
            //            alarmTypeHash[0x20] = tr("复位");
            //            alarmTypeHash[0x23] = tr("消音");

            eliminateAlarmHash[0x02] = tr("反馈消除");
            eliminateAlarmHash[0x04] = tr("故障恢复");
            // eliminateAlarmHash[0x06] = tr("停止");
            eliminateAlarmHash[0x0c] = tr("屏蔽解除");

            // createAlarm();
        }
    });
}

ArchitePlanView::~ArchitePlanView()
{
    m_treeView->saveTreeItem();
    m_autoSwitchTimer->stop();
    saveArchiteInfo();
    saveOtherArchiteInfo();

    m_sqliteManager->close();
    m_sqliteManager->deleteLater();
    delete m_serialDataProtocol;
}


void ArchitePlanView::createAlarm(const QString&extNum,const QString&loopNum,const QString&addressNum,const QString &alarmTypeName,bool isAnalog,const QString &alarmTime)
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
                    if(currentItem->extNum()==extNum&&currentItem->loopNum()==loopNum&&currentItem->addrNum()==addressNum)
                    {
                        if(!DataStore::getTypeItemList(alarmTypeName).contains(currentItem))
                        {
                            generateAlarm(alarmTypeName,currentItem,view,isAnalog);
                            currentItem->getItemInfo().m_alarmTime = alarmTime;
                        }
                        break;
                    }
                }

            }
        }
    }
}


void ArchitePlanView::eliminateAlarm(GraphicsItem *item)
{
    if(item==nullptr)
    {
        return;
    }
    item->currentState() = tr("正常");
    item->stopAnimations();
    item->stopColorAnimation();
    item->setGraphicsEffect(nullptr);
    item->restoreSize();
    DataStore::deleteTypeItem(item);
    emit  eliminateAlarmFromTable(item);
}

void ArchitePlanView::generateAlarm(const QString &alarmTypeName, GraphicsItem *item,GraphicsView *view, bool isAnalog)
{
    if(item!=nullptr)
    {
        if(item->currentState()!=tr("正常"))
        {
            return;
        }
        item->currentState()= alarmTypeName;
        if(isAnalog)
        {
            item->alarmType() = tr("模拟")+alarmTypeName;
        }
        DataStore::insertTypeItem(alarmTypeName,item);
        QList<QGraphicsItem *> graphicsItemList=   DataStore::getTypeItemList(alarmTypeName);

        if(graphicsItemList.size()>0)
        {
            GraphicsItem*gItem =dynamic_cast<GraphicsItem*>(graphicsItemList.at(0));

            if(alarmTypeName==tr("屏蔽"))
            {
                item->setColorEndValue(QColor("pink"));
            }
            else if(alarmTypeName==tr("反馈"))
            {

                item->setColorEndValue(QColor("blue"));
            }
            else if(alarmTypeName==tr("故障"))
            {
                item->setColorEndValue(QColor("yellow"));
            }
            else if(alarmTypeName==tr("监管"))
            {
                item->setColorEndValue(QColor("orange"));
            }
            else
            {
                item->setColorEndValue(QColor("red"));

            }


            if(gItem==item)
            {
                if(alarmTypeName==tr("火警"))
                {
                    item->startAnimations();
                }
                else
                {
                    item->startColorAnimation();
                }
            }
            else
            {

                item->startColorAnimation();
            }

            if(view!=nullptr)
            {
                if(view->getItemList().contains(item))
                {
                    autoFitView(view);
                }

                insertAlarmWidget(alarmTypeName,view);
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
                autoFitView(view);

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
            emit alarmHappend(alarmTypeName);

            //            QString alarmHappendTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            //            item->getItemInfo().m_alarmTime = alarmHappendTime;
            emit alarmItem(item);
        }
    }
}

void ArchitePlanView::insertAlarmWidget(const QString &type, GraphicsView *view)
{
    if(!m_alarmWidgetHash[type].contains(view))
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
    m_autoSwitchTimer = new QTimer(this);

    m_serialDataProtocol = new SerialDataProtocol;
    m_treeView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_treeView->setMaximumWidth(180);
    m_stackedWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_stackedWidget->setStyleSheet("QStackedWidget{border:1px solid black}");
    m_treeView->setStyleSheet("QTreeView{border:1px solid black}");
    QSplitter *splitter = new QSplitter(this);
    splitter->setChildrenCollapsible(false);
    QHBoxLayout*globalHLayout = new QHBoxLayout;
    splitter->addWidget(m_stackedWidget);
    splitter->addWidget(m_treeView);

    m_tabWidget->addTab(m_globalGraphicsView ,tr("总平面布局图"));
    m_tabWidget->addTab(splitter,tr("建筑平面图"));
    m_tabWidget->addTab(m_sysArchitePlanView,tr("系统图"));
    // m_sysGraphicsView->loadPixmap("D:/program/GraphicsDisplay/images/dialog.png");
    //m_tabWidget->addTab(new QWidget(this),tr("平面图"));
    globalHLayout->addWidget(m_tabWidget);
    globalHLayout->setContentsMargins(QMargins(0,0,0,0));
    setLayout(globalHLayout);
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
        itemInfoTableList<<"extNum text" << "addrNum text" << "loopNum text" << "buildingName text" << "currentState text"
                        << "deviceLocation text" << "deviceNum text" << "equipmentModel text" << "floorOfDevice text"
                        << "iconName text"<<"manufacturers text"<<"periodOfValidity text"<<"pos text"<<"size double"
                        << "sysOfDevice text"<<"operator text";
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
            else
            {
                emit toLastPage();
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

    connect(m_tabWidget,&QTabWidget::currentChanged,this,&ArchitePlanView::tabIndex);


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

    QHash<QGraphicsView*,QList<QGraphicsItem *> >itemToViewHash;

    GraphicsView *currentView = dynamic_cast<GraphicsView *>(view);
    if(currentView !=nullptr)
    {
        foreach (QGraphicsItem *alarmItem, alarmItemList)
        {
            if(currentView->getItemList().contains(alarmItem))
            {
                itemToViewHash[currentView].push_back(alarmItem);
            }
        }

        qreal xMin=10000.0*1.0,xMax=0,yMin=10000.0*1.0,yMax=0,size =0;
        QList<QGraphicsItem *>currentItemList = itemToViewHash[view];
        foreach(QGraphicsItem *graphicsItem,currentItemList)
        {
            GraphicsItem *item = dynamic_cast<GraphicsItem *>(graphicsItem);
            size=item->scale()*item->radius()*2*1.2;
            if(item!=nullptr)
            {
                xMin= qMin(xMin,item->pos().x());
                yMin= qMin(yMin,item->pos().y());
                xMax= qMax(xMax,item->pos().x());
                yMax= qMax(yMax,item->pos().y());
            }
        }

        m_stackedWidget->setCurrentWidget(view);
        QRectF currentRectF = QRectF(qAbs(xMin),qAbs(yMin),qAbs(xMax-xMin),qAbs(yMax-yMin));
        view->fitInView(currentRectF.adjusted(-size,-size,size,size),Qt::KeepAspectRatio);
    }

}

void ArchitePlanView::toAlarmView()
{
    foreach (GraphicsView*view, m_alarmViewList)
    {
        if(view!=nullptr)
        {
            if(view->haveAlarmType(tr("火警"))||view->haveAlarmType(tr("联动")))
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
            QList<QVariant>valueList,extNumList,addrNumList,loopNumList,buildingNameList,
                    currentStateList,deviceLocationList,deviceNumList,equipmentModelList,floorOfDeviceList,
                    iconNameList,manufacturersList,periodOfValidityList,posList,sizeList,sysOfDeviceList,operatorList;
            QList<QGraphicsItem *> itemList= view->getItemList();
            foreach (QGraphicsItem *item, itemList)
            {
                GraphicsItem*currentItem = dynamic_cast<GraphicsItem*>(item);
                if(currentItem!=nullptr)
                {
                    extNumList.push_back( currentItem->extNum());
                    addrNumList.push_back(currentItem->addrNum());
                    loopNumList.push_back(currentItem->loopNum());
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
                }
            }

            valueList.push_back(extNumList);
            valueList.push_back(addrNumList);
            valueList.push_back(loopNumList);
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
    m_jsonValueList=JsonEdit::instance()->readFile(c_jsonFilePath).toList();
    for(int i=0;i<m_jsonValueList.size();i++)
    {
        QHash<QString,QVariant> parentHash=  m_jsonValueList.at(i).toHash();
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

            //  QHash<QString,QVariant> parentPixmapHash = parentHash["image"].toHash();
            // setViewFromJson(parentPixmapHash,parentItem);

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
    m_jsonValueList.clear();
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
                if(valueList.size()>j+15)
                {
                    GraphicsItem *item = new GraphicsItem(graphicsScene);
                    item->extNum() =valueList.at(j);
                    item->addrNum() = valueList.at(j+1);
                    item->loopNum() =valueList.at(j+2);
                    item->buildingName() = valueList.at(j+3);
                    item->currentState() =valueList.at(j+4);
                    item->deviceLocation() = valueList.at(j+5);
                    item->deviceNum() =valueList.at(j+6);
                    item->equipmentModel() = valueList.at(j+7);
                    item->floorOfDevice() =valueList.at(j+8);
                    item->setIconName( valueList.at(j+9));
                    item->manufacturers() =valueList.at(j+10);
                    item->periodOfValidity() = valueList.at(j+11);
                    QString posStr = valueList.at(j+12);
                    item->setPos(QPointF(posStr.section(",",0,0).toDouble(),posStr.section(",",1,1).toDouble()));
                    QString sizeStr = valueList.at(j+13);
                    item->setRadius(sizeStr.toDouble());
                    item->sysOfDevice() =valueList.at(j+14);
                    item->deviceOperator() = valueList.at(j+15);
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
        if(m_alarmWidgetHash["全部"].contains(currentView))
        {
            m_alarmWidgetHash["全部"].removeOne(currentView);
        }
    }
    if(m_alarmWidgetHash[m_currentAlarmType].contains(currentView))
    {
        m_alarmWidgetHash[m_currentAlarmType].removeOne(currentView);
    }
    GraphicsView*view =dynamic_cast<GraphicsView*>(m_stackedWidget->currentWidget());
    updateAlarmWidget(view);
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
    return m_alarmWidgetHash[m_currentAlarmType].size();
}

int ArchitePlanView::currentPage()
{
    GraphicsView*view = dynamic_cast<GraphicsView*>(m_stackedWidget->currentWidget());
    QList<GraphicsView*>viewList= m_alarmWidgetHash[m_currentAlarmType];
    return viewList.indexOf(view);
}

void ArchitePlanView::clearAlarm(bool alarmColorRedu)
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
                item->getItemInfo().m_currentState = tr("正常");
            }


            DataStore::clearTypeItem();
            clearAlarmWidget();
            m_alarmViewList.clear();
            emit noPage();
            emit reduInstruction(alarmColorRedu);
        }
    }

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
    emit clearAlarmFromTable();
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

void ArchitePlanView::toArchitePlan(const QString &extNum, const QString &loopNum, const QString &addressNum)
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
                    if(currentItem->extNum()==extNum && currentItem->loopNum()==loopNum && currentItem->addrNum()==addressNum)
                    {
                        if(m_tabWidget->count()>=2)
                        {
                            if(m_tabWidget->currentIndex()!=1)
                            {
                                m_tabWidget->setCurrentIndex(1);
                            }
                        }
                        m_stackedWidget->setCurrentWidget(view);
                        view->fitInView(currentItem,Qt::KeepAspectRatio);
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

    if(m_alarmViewList.size()>m_alarmPos)
    {
        autoFitView(m_alarmViewList.at(m_alarmPos));
        m_alarmPos++;
    }
    else
    {
        m_alarmPos =0;
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
