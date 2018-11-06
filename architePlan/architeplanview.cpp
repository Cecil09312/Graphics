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
ArchitePlanView::ArchitePlanView(QWidget *parent)
    : QWidget(parent)
{
    initWidget();
    initFromJsonFile();
}

ArchitePlanView::~ArchitePlanView()
{
    m_treeView->saveTreeItem();
    saveArchiteInfo();
}


void ArchitePlanView::creatAlarm()
{
    //   int page =m_stackedWidget->count();

    //   if(m_widgetMap.size()>0)
    //   {
    //       m_widgetMap
    //   }

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



void ArchitePlanView::initWidget()
{
    m_treeView = new TreeView(this);
    m_stackedWidget = new QStackedWidget(this);
    m_tabWidget = new QTabWidget(this);
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
    //m_tabWidget->addTab(new QWidget(this),tr("平面图"));
    globalHLayout->addWidget(m_tabWidget);
    globalHLayout->setContentsMargins(QMargins(0,0,0,0));
    setLayout(globalHLayout);

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
        if(parentItem)
        {
            QMap<QString,QVariant> rootImageMap;
            QMap<QStandardItem*,int>itemMap;
            itemMap = m_treeView->getTreeIndexMap();
            int page =itemMap[parentItem];
            GraphicsView*widget = m_widgetMap[page];
            if(widget!=nullptr)
            {
                rootImageMap["path"]=widget->pixmapName();
              JsonEdit::instance()->setRootImage(model->indexFromItem(parentItem).row(),rootImageMap);
            }
            if(parentItem->hasChildren())
            {
                for(int j=0;j<parentItem->rowCount();j++)
                {
                    QStandardItem *childItem = parentItem->child(j);

                    if(childItem)
                    {
                        QMap<QString,QVariant> childImageMap;
                        QMap<QStandardItem*,int>childItemMap;
                        childItemMap = m_treeView->getTreeIndexMap();
                        int page =childItemMap[childItem];
                        GraphicsView*childWidget = m_widgetMap[page];
                        if(childWidget!=nullptr)
                        {
                            childImageMap["path"]=childWidget->pixmapName();
                            JsonEdit::instance()->setChildImage(model->indexFromItem(parentItem).row(),model->indexFromItem(childItem).row(),childImageMap);
                        }
                    }
                }
            }

        }

    }

    JsonEdit::instance()->writeFile(c_jsonFilePath);
}

void ArchitePlanView::initFromJsonFile()
{
    QList<QVariant> valueList=JsonEdit::instance()->readFile(c_jsonFilePath).toList();
    for(int i=0;i<valueList.size();i++)
    {
        QMap<QString,QVariant> parentMap=  valueList.at(i).toMap();
        if(!parentMap.isEmpty())
        {
            QStandardItem *parentItem= m_treeView->addRootItem();
            parentItem->setText(parentMap["name"].toString());
            QList<QVariant> childList =  parentMap["child"].toList();

            QMap<QString,QVariant> parentPixmapMap = parentMap["image"].toMap();
            if(!parentPixmapMap.isEmpty())
            {
                QMap<QStandardItem*,int>parentItemMap;
                parentItemMap = m_treeView->getTreeIndexMap();
                int page =parentItemMap[parentItem];
                GraphicsView*parentWidget = m_widgetMap[page];
                if(parentWidget)
                {
                    parentWidget->loadPixmap(parentPixmapMap["path"].toString());

                }
            }
            for(int j=0;j<childList.size();j++)
            {
                QMap<QString,QVariant> childMap=  childList.at(i).toMap();
                QModelIndex parentIndex= parentItem->index();

                if(parentIndex.isValid())
                {
                    QStandardItem *childItem= m_treeView->addChildItem(parentIndex);
                    if(childItem)
                    {
                        childItem->setText(childMap["name"].toString());

                        QMap<QString,QVariant> childPixmapMap = childMap["image"].toMap();
                        if(!childPixmapMap.isEmpty())
                        {
                            QMap<QStandardItem*,int>childItemMap;
                            childItemMap = m_treeView->getTreeIndexMap();
                            int page =childItemMap[childItem];
                            GraphicsView*childWidget = m_widgetMap[page];
                            if(childWidget)
                            {
                                childWidget->loadPixmap(childPixmapMap["path"].toString());

                            }

                        }

                    }
                }

            }

        }
    }
}

void ArchitePlanView::findFireAlarm(int pos)
{
    QList<QGraphicsItem*>itemList= Controller::instance()->getDataStore()->getTypeItemList(tr("火警"));
    int listSize = itemList.size();
    QList<GraphicsView *>viewList = m_widgetMap.values();
    if(viewList.size()>0)
    {
        if(listSize>0)
        {
            for(int i=0;i<viewList.size();i++)
            {
                GraphicsView * currentView = viewList.at(i);
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
}
