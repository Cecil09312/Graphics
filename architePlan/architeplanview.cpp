#include "architeplanview.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>

#include <QGLWidget>
#include <QOpenGLWidget>
//#include "control/controller.h"
#include "dataStore/datastore.h"
#include "jsonEdit/jsonedit.h"
#include "graphicsWidget/graphicsitem.h"
#include <QDebug>
#include "control/controller.h"
ArchitePlanView::ArchitePlanView(QWidget *parent)
    : QWidget(parent)
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
            generateAlarm(alarmTypeName,currentItem);
        }


    }



    //   GraphicsScene::getItemList();

}

void ArchitePlanView::generateAlarm(const QString &alarmTypeName, GraphicsItem *item)
{
    if(item!=nullptr)
    {
        item->getItemInfo().m_alarmType= alarmTypeName;
        Controller::instance()->getDataStore()->insertTypeItem(alarmTypeName,item);
        if(Controller::instance()->getDataStore()->getTypeItemList(alarmTypeName).at(0)==item)
        {
            item->startAnimations();
        }
        else
        {
            item->startColorAnimation();
        }
        QList<QGraphicsView*>viewList =   item->scene()->views();
        foreach (QGraphicsView*view, viewList)
        {
            if(view!=nullptr)
            {
                autoFitView(view);
            }
        }

        emit alarmHappend(alarmTypeName);
        // view->centerOn(currentItem);
    }
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
        if(m_stackedWidget->count()>1)
        {
            if(m_stackedWidget->currentIndex()>0 && m_stackedWidget->currentIndex()<m_stackedWidget->count()-1)
            {
                emit normalPage();
            }
            else if(m_stackedWidget->currentIndex()==0)
            {
                emit toFirstPage();
            }

        }
        else if(m_stackedWidget->count()==1)
        {
            emit noPage();
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

    connect(m_treeView,&TreeView::deleteIndex,this,[=](QStandardItem* item)
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
            }

        }

        int page =itemMap[item];
        GraphicsView*widget = m_widgetMap[page];
        m_stackedWidget->removeWidget(widget);
        m_widgetMap.remove(page);
        m_treeView->getTreeIndexMap().remove(item);
        if(m_stackedWidget->count()<=1)
        {
            emit noPage();
        }


    });

    connect(m_treeView,&TreeView::insertAnchPixmap,this,[=](QStandardItem*item,const QString &fileName)
    {
        QMap<QStandardItem*,int>itemMap;
        itemMap = m_treeView->getTreeIndexMap();
        int page =itemMap[item];
        GraphicsView*widget = m_widgetMap[page];
        if(widget!=nullptr)
            widget->loadPixmap(fileName);
    });

    connect(m_stackedWidget,&QStackedWidget::currentChanged,this,[=](int index)
    {
        int count = m_stackedWidget->count();
        if(index==0)
        {
            emit toFirstPage();

        }
        else if(index>0 && index <count-1)
        {
            emit normalPage();
        }
        else
        {
            emit toLastPage();
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
    QList< QList<QGraphicsItem *> >list= Controller::instance()->getDataStore()->getTypeItemHash().values();
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
    QList<QGraphicsItem*>itemList= Controller::instance()->getDataStore()->getTypeItemList(tr("火警"));
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

int ArchitePlanView::numOfTypeAlarm(const QString &type)
{
    int num= Controller::instance()->getDataStore()->numOfTypeItem(type);
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
    return m_stackedWidget->count();
}

int ArchitePlanView::currentPage()
{
    return m_stackedWidget->currentIndex()+1;
}

void ArchitePlanView::clearAlarm()
{
    QList< QList<QGraphicsItem *> >list= Controller::instance()->getDataStore()->getTypeItemHash().values();
    foreach (QList<QGraphicsItem *>itemList, list) {
        foreach (QGraphicsItem *currentItem, itemList) {
            GraphicsItem *item = dynamic_cast<GraphicsItem *>(currentItem);
            item->stopAnimations();
            item->stopColorAnimation();
            item->setGraphicsEffect(nullptr);
            item->restoreSize();
            Controller::instance()->getDataStore()->clearTypeItem();

        }
    }
}

void ArchitePlanView::toPreviousPage()
{
    int count =m_stackedWidget->count();
    if(count>0)
    {
        int currentIndex = m_stackedWidget->currentIndex();
        if(currentIndex<count && currentIndex>0)
        {
            m_stackedWidget->setCurrentIndex(currentIndex-1);
        }
        if(m_stackedWidget->currentIndex()==0)
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
    int count =m_stackedWidget->count();
    if(count>0)
    {
        int currentIndex = m_stackedWidget->currentIndex();
        if(currentIndex<count-1)
        {
            m_stackedWidget->setCurrentIndex(currentIndex+1);
        }

        if(m_stackedWidget->currentIndex()==(count-1))
        {
            emit toLastPage();
        }
        else
        {

            emit normalPage();
        }

    }
}
