#include "architeplanview.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>

#include <QGLWidget>
#include <QOpenGLWidget>
#include "openglWidget/glwidget.h"
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
    // qDebug() << m_stackedWidget->count();
}

ArchitePlanView::~ArchitePlanView()
{
    m_treeView->saveTreeItem();
    saveArchiteInfo();
}


void ArchitePlanView::creatAlarm()
{
    int page =qrand()% m_stackedWidget->count();

    if(m_widgetMap.size()>page)
    {
        QString alarmTypeName =tr("火警");
        GraphicsView *view = m_widgetMap[page];
        QList<QGraphicsItem *>itemList =   view->getItemList();
        int pos = qrand()%itemList.size();
        if(itemList.size()>pos)
        {
            GraphicsItem *currentItem = dynamic_cast<GraphicsItem *>(itemList.at(pos));
            if(currentItem)
            {
                currentItem->setTypeName(alarmTypeName);
                currentItem->startAnimation();
                Controller::instance()->getDataStore()->insertTypeItem(alarmTypeName,currentItem);
                emit alarmHappend(alarmTypeName);
            }
        }
    }

    //   GraphicsScene::getItemList();

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
    m_textToSpeech = new QTextToSpeech(this);
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
                    GraphicsItem *item = new GraphicsItem(scene,this);
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

int ArchitePlanView::numOfTypeAlarm(const QString &type)
{
    int num= Controller::instance()->getDataStore()->numOfTypeItem(type);
    return num;
}

void ArchitePlanView::setGlobalArchitePixmap(const QString &pixmapName)
{
    QString filePath=  Controller::instance()->fileNameFromQml(pixmapName);
    m_globalGraphicsView->setStyleSheet(QString("QWidget{margin:20;border-image:url(%1)}").arg(filePath));
}
