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

QMap<int,GraphicsView *>ArchitePlanView::m_widgetMap =QMap<int,GraphicsView *>();
ArchitePlanView::ArchitePlanView(QWidget *parent)
    : QWidget(parent),
      m_currentAlarmType("全部")
{
    initWidget();
    initFromJsonFile();
    setGlobalArchiteFromJson();
}

ArchitePlanView::~ArchitePlanView()
{
    m_treeView->saveTreeItem();
    saveArchiteInfo();
    saveOtherArchiteInfo();
}


void ArchitePlanView::createAlarm(const QString &alarmTypeName)
{
    int page =qAbs(qrand()% m_stackedWidget->count());

    if(m_widgetMap.size()>page && page>=0)
    {
        // QString alarmTypeName =tr("火警");
        GraphicsView *view = m_widgetMap[page];
        if(view==nullptr)
            return;
        QList<QGraphicsItem *>itemList =   view->getItemList();
        if(itemList.size()<=0)
            return;

        int pos = qAbs(qrand()%itemList.size()) ;
        if(itemList.size()>pos)
        {
            GraphicsItem *currentItem = dynamic_cast<GraphicsItem *>(itemList.at(pos));
            generateAlarm(alarmTypeName,currentItem,view);
        }
    }
}

void ArchitePlanView::eliminateAlarm(GraphicsItem *item)
{
    if(item==nullptr)
    {
        return;
    }
    item->alarmType() = "无";
    item->stopAnimations();
    item->stopColorAnimation();
    item->setGraphicsEffect(nullptr);
    item->restoreSize();
    item->getItemInfo().m_currentAlarmState = "报警消除";
    DataStore::deleteTypeItem(item);
    emit  eliminateAlarmFromTable(item);
}

void ArchitePlanView::generateAlarm(const QString &alarmTypeName, GraphicsItem *item,GraphicsView *view)
{
    if(item!=nullptr)
    {
        item->getItemInfo().m_alarmType= alarmTypeName;
        DataStore::insertTypeItem(alarmTypeName,item);
        if(DataStore::getTypeItemList(alarmTypeName).at(0)==item)
        {
            item->startAnimations();
        }
        else
        {
            item->startColorAnimation();
        }
        if(view->getItemList().contains(item))
        {
            autoFitView(view);
        }
        emit alarmHappend(alarmTypeName);
        insertAlarmWidget(alarmTypeName,view);
        insertAlarmWidget("全部",view);
        updateAlarmWidget(view);
        item->getItemInfo().m_currentAlarmState= "正在报警";
        QString alarmHappendTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        item->getItemInfo().m_alarmReceiveTime = alarmHappendTime;
        emit alarmItem(item);
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
    m_globalGraphicsView = new QWidget(this);
    m_sysArchitePlanView = new SysArchitePlanView(this);
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

    m_tabWidget->addTab(splitter,tr("建筑平面图"));
    m_tabWidget->addTab(m_globalGraphicsView ,tr("总平面布局图"));
    m_tabWidget->addTab(m_sysArchitePlanView,tr("系统图"));
    // m_sysGraphicsView->loadPixmap("D:/program/GraphicsDisplay/images/dialog.png");
    //m_tabWidget->addTab(new QWidget(this),tr("平面图"));
    globalHLayout->addWidget(m_tabWidget);
    globalHLayout->setContentsMargins(QMargins(0,0,0,0));
    setLayout(globalHLayout);
    Controller::instance()->setSysArchitePlanView(m_sysArchitePlanView);
    connect(m_treeView,&TreeView::treeIndex,this,[=](QStandardItem*item)
    {
        QMap<QStandardItem*,int>map= m_treeView->getTreeIndexMap();
        int page = map[item];
        if(m_widgetMap[page]==nullptr)
        {
            GraphicsView *widget = new GraphicsView(this);
            m_widgetMap[page]=widget;
            m_stackedWidget->addWidget(widget);
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
        m_stackedWidget->setCurrentWidget(m_widgetMap[page]);
        GraphicsView *currentView = dynamic_cast<GraphicsView *>(m_widgetMap[page]);
        updateAlarmWidget(currentView);

    });

    connect(m_treeView,&TreeView::clearIndex,this,[=]()
    {
        int count = m_widgetMap.size();
        for(int i=0;i<count;i++)
        {
            QWidget *widget = m_widgetMap.values().at(i);
            m_stackedWidget->removeWidget(widget);
        }
        m_widgetMap.clear();
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

}

void ArchitePlanView::autoFitView(QGraphicsView *view)
{
    QList< QList<QGraphicsItem *> >list= DataStore::getTypeItemHash().values();
    QList<QGraphicsItem *> alarmItemList;
    foreach (QList<QGraphicsItem *> itemList, list) {
        foreach (QGraphicsItem * item, itemList) {
            alarmItemList.push_back(item);
        }

    }

    QHash<QGraphicsView*,QList<QGraphicsItem *> >itemToViewHash;

    GraphicsView *currentView = dynamic_cast<GraphicsView *>(view);
    if(currentView !=nullptr)
    {
        foreach (QGraphicsItem *alarmItem, alarmItemList) {
            if(currentView->getItemList().contains(alarmItem)){
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
        QList<QGraphicsItem *>currentItemList =widget->getItemList();
        QList<QVariant>itemList;
        foreach (QGraphicsItem * graphicsItem, currentItemList)
        {
            GraphicsItem*currentItem=  dynamic_cast<GraphicsItem*> (graphicsItem);
            if(currentItem!=nullptr)
            {
                itemList.push_back(currentItem->itemInfo());
            }
        }

        imageHash["item"] = itemList;
    }
    return imageHash;
}

void ArchitePlanView::initFromJsonFile()
{
    QList<QVariant> valueList=JsonEdit::instance()->readFile(c_jsonFilePath).toList();
    for(int i=0;i<valueList.size();i++)
    {
        QHash<QString,QVariant> parentHash=  valueList.at(i).toHash();
        if(!parentHash.isEmpty())
        {
            QStandardItem *parentItem= m_treeView->addRootItem();
            parentItem->setText(parentHash["name"].toString());
            QList<QVariant> childList =  parentHash["child"].toList();
            QHash<QString,QVariant> parentPixmapHash = parentHash["image"].toHash();
            setViewFromJson(parentPixmapHash,parentItem);
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
                        setViewFromJson(childPixmapHash,childItem);
                    }
                }

            }

        }
    }
}

void ArchitePlanView::setViewFromJson(const QHash<QString,QVariant> &hash,QStandardItem *treeItem)
{

    if(!hash.isEmpty())
    {
        QMap<QStandardItem*,int>parentItemMap;
        parentItemMap = m_treeView->getTreeIndexMap();
        int page =parentItemMap[treeItem];
        GraphicsView*widget = m_widgetMap[page];
        if(widget!=nullptr)
        {
            QList<QVariant> itemList=  hash["item"].toList();
            foreach (QVariant itemValue, itemList) {
                GraphicsScene *scene = dynamic_cast<GraphicsScene *> (widget->scene()) ;
                if(scene!=nullptr)
                {
                    GraphicsItem *item = new GraphicsItem(scene);
                    scene->setItemInfo(item,itemValue.toHash());
                }
            }
            widget->loadPixmap(hash["path"].toString());

        }
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
            QList<QGraphicsItem*>viewItemList= currentView->getItemList();
            foreach (QGraphicsItem*currentItem, viewItemList)
            {
                if(listSize>pos && pos>=0)
                {
                    if(currentItem==itemList.at(pos))
                    {
                        m_tabWidget->setCurrentIndex(0);
                        m_stackedWidget->setCurrentWidget(currentView);
                        return;
                    }
                }
                else
                {
                    if(currentItem==itemList.at(listSize-1))
                    {
                        m_tabWidget->setCurrentIndex(0);
                        m_stackedWidget->setCurrentWidget(currentView);
                        return;
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
    architePlanHash["grobalArchitePlan"] = m_globalArchitePlanPixmapName;
    QmlForJson::writeFile(architePlanHash);
}

void ArchitePlanView::setGlobalArchiteFromJson()
{
    QHash<QString,QVariant> valueHash = QmlForJson::readFile().toHash();
    QString pixmapName= valueHash["grobalArchitePlan"].toString();
    setGlobalArchitePixmap(pixmapName);
}

void ArchitePlanView::updateAlarmWidget(GraphicsView *currentView)
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
    m_globalGraphicsView->setStyleSheet(QString("QWidget{margin:20;border-image:url(%1)}").arg(filePath));
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

void ArchitePlanView::clearAlarm()
{
    QList< QList<QGraphicsItem *> >list= DataStore::getTypeItemHash().values();
    foreach (QList<QGraphicsItem *>itemList, list)
    {
        foreach (QGraphicsItem *currentItem, itemList)
        {
            GraphicsItem *item = dynamic_cast<GraphicsItem *>(currentItem);
            item->stopAnimations();
            item->stopColorAnimation();
            item->setGraphicsEffect(nullptr);
            item->restoreSize();
            item->alarmType() = "无";
            item->getItemInfo().m_currentAlarmState = "报警消除";
            DataStore::clearTypeItem();
            clearAlarmWidget();
            emit noPage();
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
            emit toLastPage();
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

QString ArchitePlanView::currentAlarmType()
{
    return m_currentAlarmType;
}

void ArchitePlanView::deleteViewFromItem(QStandardItem* item)
{
    QMap<QStandardItem*,int>itemMap;
    itemMap = m_treeView->getTreeIndexMap();
    if(item->hasChildren())
    {
        for(int i=0;i<item->rowCount();i++)
        {
            QStandardItem*childItem =  item->child(i);
            int chileItemPage = itemMap[childItem];
            GraphicsView*childWidget = m_widgetMap[chileItemPage];
            m_stackedWidget->removeWidget(childWidget);
            m_widgetMap.remove(chileItemPage);
            m_treeView->getTreeIndexMap().remove(childItem);
            deleteAlarmWidget(childWidget);
        }
    }

    int page =itemMap[item];
    GraphicsView*widget = m_widgetMap[page];
    m_stackedWidget->removeWidget(widget);
    m_widgetMap.remove(page);
    m_treeView->getTreeIndexMap().remove(item);
    deleteAlarmWidget(widget);
}

GraphicsView *ArchitePlanView::viewToParentItem(QStandardItem *item)
{
    QMap<QStandardItem*,int>itemMap;
    itemMap = m_treeView->getTreeIndexMap();
    int page =itemMap[item];
    GraphicsView*widget = m_widgetMap[page];
    return widget;
}

QList<GraphicsView *> ArchitePlanView::viewsToChildItem(QStandardItem *item)
{
    QMap<QStandardItem*,int>itemMap;
    itemMap = m_treeView->getTreeIndexMap();
    QList<GraphicsView*>viewList;
    if(item->hasChildren())
    {
        for(int i=0;i<item->rowCount();i++)
        {
            QStandardItem*childItem =  item->child(i);
            int chileItemPage = itemMap[childItem];
            GraphicsView*childWidget = m_widgetMap[chileItemPage];
            viewList.push_back(childWidget);
        }
    }
    return viewList;
}
