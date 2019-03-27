#include "treeview.h"
#include <QDebug>
#include <QList>
#include <QItemSelectionModel>
#include <QLineEdit>
#include <QHeaderView>
#include <algorithm>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlEngine>
#include "jsonEdit/jsonedit.h"

TreeView::TreeView(QWidget *parent):
    QTreeView(parent)
{
    initWidget();
    initMenu();
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_closeAction,&QAction::triggered,this,[=]()
    {
        m_treeSettingMenu->close();
    });
    connect(m_addChildAction,&QAction::triggered,this,[=]()
    {
        QModelIndex index = indexAt(m_rootPoint);
        addChildItem(index);
    });

    connect(m_editAction,&QAction::triggered,this,[=]()
    {
        QObject *obj= m_architeSettingView->rootObject();
        QStandardItem *item = m_stdModel->itemFromIndex(indexAt(m_rootPoint));
        if(item!=nullptr)
        {
            QMetaObject::invokeMethod(obj, "setArchiteName",Q_ARG(QVariant,item->text()));

            ArchitePlanView *architeView=  Controller::instance()->getArchitePlanView();
            GraphicsView *graphicsView =  architeView->viewFromChildItem(item);
            if(graphicsView!=nullptr)
            {
                QMetaObject::invokeMethod(obj, "setArchiteImage",Q_ARG(QVariant,graphicsView->pixmapName()));
            }
        }
        //QMetaObject::invokeMethod(obj,"clearTextField");
        m_architeSettingView->show();
    });

    connect(m_deleteAction,&QAction::triggered,this,[=]()
    {
        QModelIndex index = indexAt(m_rootPoint);
        deleteTreeItem(index);
    });
    connect(m_clearAction,&QAction::triggered,this,[=]()
    {
        QList<GraphicsView*>viewList= Controller::instance()->getArchitePlanView()->getWidgetMap().values();
        bool isCanClear = false;
        foreach (GraphicsView*view, viewList)
        {
            if(view!=nullptr)
            {
                if(view->haveAnyAlarm())
                {
                    isCanClear = true;
                    break;
                }
            }
        }

        if(isCanClear)
        {
            QMessageBox::critical(this,tr("警告"),tr("存在报警信息，消除报警才能清除"));
        }
        else
        {
            clearItem();
        }

    });
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&TreeView::customContextMenuRequested,this,[=](const QPoint&/*pos*/)
    {

        m_rootPoint=  QWidget::mapFromGlobal(QCursor::pos());
        if(Controller::instance()->getUserRight()!=UserManager::Super)
        {
            m_editAction->setEnabled(false);
            m_deleteAction->setEnabled(false);
            m_clearAction->setEnabled(false);
            //m_addAction->setEnabled(false);
            m_addChildAction->setEnabled(false);
        }
        else
        {
            QModelIndex index = indexAt(m_rootPoint);
            if(index.isValid())
            {
                QStandardItem *stdItem=  m_stdModel->itemFromIndex(index);
                if(stdItem!=nullptr && stdItem->parent()!=nullptr)
                {
                    m_editAction->setEnabled(true);
                    m_deleteAction->setEnabled(true);
                    m_addChildAction->setEnabled(false);
                }
                else if(stdItem==nullptr)
                {
                    m_editAction->setEnabled(false);
                    m_deleteAction->setEnabled(false);
                    m_addChildAction->setEnabled(false);
                }
                else if(stdItem!=nullptr && stdItem->parent()==nullptr)
                {
                    m_editAction->setEnabled(false);
                    m_deleteAction->setEnabled(true);
                    m_addChildAction->setEnabled(true);
                }
            }
            else
            {
                m_editAction->setEnabled(false);
                m_deleteAction->setEnabled(false);
                m_addChildAction->setEnabled(false);
            }

            if(m_stdModel->rowCount()>0)
            {
                m_clearAction->setEnabled(true);
            }
            else
            {
                m_clearAction->setEnabled(false);
            }
        }

        m_treeSettingMenu->exec(QCursor::pos());
    });

    connect(this,&TreeView::doubleClicked,this,[=](const QModelIndex &index)
    {
        QStandardItem*item=   m_stdModel->itemFromIndex(index);
        if(item!=nullptr)
        {
            if(item->parent()==nullptr)
            {
                emit toGlobalGraphicsView(item);
            }
        }

    });
}

TreeView::~TreeView()
{
    m_treeSettingMenu->close();
    delete m_treeSettingMenu;
    m_architeSettingView->deleteLater();
}



QMap<QStandardItem *, int> &TreeView::getTreeIndexMap()
{
    return m_treeIndexMap;
}

void TreeView::saveTreeItem()
{
    for(int i=0;i<m_stdModel->rowCount();i++)
    {
        QHash<QString,QVariant> rootHash;
        QStandardItem *parentItem = m_stdModel->item(i);
        if(parentItem)
        {
            rootHash["name"] =parentItem->text();
            JsonEdit::instance()->insertRoot(rootHash);
            if(parentItem->hasChildren())
            {
                for(int j=0;j<parentItem->rowCount();j++)
                {
                    QStandardItem *childItem = parentItem->child(j);

                    if(childItem)
                    {
                        QHash<QString,QVariant> childHash;
                        childHash["name"] = childItem->text();
                        JsonEdit::instance()->insertChild(i,childHash);
                    }
                }
            }

        }

    }
}

QStandardItem * TreeView::addRootItem(const QString &root)
{
    int rootCount= m_stdModel->rowCount();
    int totalRowCounts = getTotalCount();
    QStandardItem* stdRootItem = nullptr;
    QList<QStandardItem*>stdItemList=   m_stdModel->findItems(root);
    if(stdItemList.size()==0)
    {
        QStandardItem *rootItem =new QStandardItem(root);
        m_stdModel->insertRow(rootCount,rootItem);
        // m_treeIndexMap[rootItem]=totalRowCounts;
        //emit treeIndex(rootItem);
        m_parentIndexList.push_back(totalRowCounts);
        return rootItem;
    }
    else
    {
        foreach (QStandardItem*stdItem, stdItemList)
        {
            if(stdItem->text()==root)
            {
                stdRootItem = stdItem;
                break;
            }
        }
        return stdRootItem;
    }
}

QStandardItem* TreeView::addChildItem(QModelIndex index)
{

    if(!index.parent().isValid())
    {
        if(index.isValid())
        {
            QStandardItem *standardItem = m_stdModel->item(index.row());
            int totalRowCounts = getTotalCount();
            if(standardItem!=nullptr)
            {
                int rowCount =0;
                QList<int >childRowList = m_childIndexMap[index.row()];
                int size = childRowList.size();
                if(childRowList.size()>0)
                {
                    auto it= std::minmax_element(childRowList.begin(),childRowList.end()).second;
                    rowCount = *it+1;
                }
                QStandardItem*childItem = new QStandardItem(QString("%1层").arg(rowCount+1));
                standardItem->setChild(size,childItem);
                m_treeIndexMap[childItem] = totalRowCounts;
                emit treeIndex(childItem);
                m_childIndexMap[index.row()].push_back(rowCount);
                return childItem;
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
    else
    {
        return nullptr;
    }
}

void TreeView::setItemExpanded(const QStandardItem *item)
{
    for(int i=0;i<m_stdModel->rowCount();i++)
    {
        setExpanded(m_stdModel->item(i)->index(),false);
    }
    QModelIndex index = m_stdModel->indexFromItem(item);
    setExpanded(index,true);
}



void TreeView::deleteTreeItem(QModelIndex index)
{
    if(index.isValid())
    {
        QStandardItem *standardItem = m_stdModel->itemFromIndex(index);
        GraphicsView*view=  Controller::instance()->getArchitePlanView()->viewFromChildItem(standardItem);
        if(view!=nullptr)
        {
            if(view->haveAnyAlarm())
            {
                QMessageBox::critical(this,tr("警告"),tr("存在报警，消除报警后才能删除"));
                return;
            }
        }

        emit deleteIndex(standardItem);
        if(standardItem->parent()==nullptr)
        {
            if(m_parentIndexList.size()>index.row())
            {
                m_parentIndexList.removeAt(index.row());
            }
            m_stdModel->removeRow(index.row());
        }
        else
        {
            QStandardItem *parentItem =standardItem->parent();
            if(parentItem)
            {
                if(parentItem->rowCount()>index.row())
                {

                    if(m_childIndexMap[index.parent().row()].size()>index.row())
                    {
                        m_childIndexMap[index.parent().row()].removeAt(index.row());
                    }
                    parentItem->removeRow(index.row());
                    m_stdModel->setItem(index.parent().row(),parentItem);
                }
            }

        }
    }
}

void TreeView::clearItem()
{
    emit clearIndex();
    m_stdModel->clear();
    m_treeIndexMap.clear();
    m_childIndexMap.clear();
    m_parentIndexList.clear();
}

void TreeView::setItemName(const QString &name)
{
    QModelIndex index = indexAt(m_rootPoint);
    QStandardItem*item = m_stdModel->itemFromIndex(index);
    item->setText(name);
    if(item->parent()!=nullptr)
    {
        ArchitePlanView*architePlanView=   Controller::instance()->getArchitePlanView();
        GraphicsView *graphicsView= architePlanView->viewFromChildItem(item);
        QList<QGraphicsItem *>itemList=  graphicsView->getItemList();
        foreach (QGraphicsItem *currentItem, itemList)
        {
            GraphicsItem*graphicsItem = dynamic_cast<GraphicsItem*>(currentItem);
            if(graphicsItem!=nullptr)
            {
                graphicsItem->floorOfDevice() = name;
            }
        }

    }
}

void TreeView::insertPixmap(const QString &fileName)
{
    QModelIndex index = indexAt(m_rootPoint);
    QStandardItem*item = m_stdModel->itemFromIndex(index);
    emit insertAnchPixmap(item,fileName);
}

void TreeView::architeSettingViewClose()
{
    m_architeSettingView->close();
}

void TreeView::initWidget()
{
    m_stdModel = new QStandardItemModel(this);
    m_itemDelegate = new ItemDelegate(this);

    m_architeSettingView = new QQuickView();
    m_architeSettingView->setSource(QUrl("qrc:/qml/treeViewSetting/TreeViewSetting.qml"));
    m_architeSettingView->setGeometry(500,50,m_architeSettingView->width(),m_architeSettingView->height());
    m_architeSettingView->rootContext()->setContextProperty("TreeView",this);

    header()->hide();
    setModel(m_stdModel);
}

void TreeView::initMenu()
{
    m_treeSettingMenu = new QMenu;
    //m_addAction  = new QAction(tr("添加楼层"),m_treeSettingMenu);
    m_addChildAction = new QAction(tr("添加楼层"),m_treeSettingMenu);
    m_editAction = new QAction(tr("编辑"),m_treeSettingMenu);
    m_deleteAction= new QAction(tr("删除"),m_treeSettingMenu);
    m_clearAction= new QAction(tr("清空"),m_treeSettingMenu);
    m_closeAction = new QAction(tr("关闭"),m_treeSettingMenu);
    //m_treeSettingMenu->addAction(m_addAction);
    m_treeSettingMenu->addAction(m_addChildAction);
    m_treeSettingMenu->addAction(m_editAction);
    m_treeSettingMenu->addAction(m_deleteAction);
    m_treeSettingMenu->addAction(m_clearAction);
    m_treeSettingMenu->addAction(m_closeAction);

}

int TreeView::getTotalCount()
{
    int rootCount= m_stdModel->rowCount();
    int totalRowCounts = 0;
    for(int i=0;i<rootCount;i++)
    {
        if(m_stdModel->item(i)->hasChildren())
        {
            totalRowCounts+= m_stdModel->item(i)->rowCount();
        }
    }
    QList<int> treeIndexList=  m_treeIndexMap.values();
    if(treeIndexList.size()>0)
    {
        auto iter = std::minmax_element(treeIndexList.begin(),treeIndexList.end()).second;
        if(totalRowCounts<=*iter)
        {
            totalRowCounts = *iter+1;
        }
    }
    return totalRowCounts;
}



ItemDelegate::ItemDelegate(QObject *parent):
    QItemDelegate(parent)

{

}

ItemDelegate::~ItemDelegate()
{

}

void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(editor);
    lineEdit->setEnabled(true);
    lineEdit->setFocus();

    model->setData(index,lineEdit->text());
}



void ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(editor);
    lineEdit->setEnabled(true);
    lineEdit->setFocus();
    lineEdit->setText(index.model()->data(index,Qt::EditRole).toString());
}
