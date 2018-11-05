#include "treeview.h"
#include <QDebug>
#include <QList>
#include <QItemSelectionModel>
#include <QLineEdit>
#include <QHeaderView>
#include <algorithm>
#include <QQmlContext>
//#include "jsonEdit/jsonedit.h"
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlEngine>
//#include "control/controller.h"

TreeView::TreeView(QWidget *parent):
    QTreeView(parent)
{
    initWidget();
    initMenu();
    connect(m_addAction,&QAction::triggered,this,[=]()
    {
        addRootItem();
    });
    connect(m_closeAction,&QAction::triggered,this,[=]()
    {
        m_menu->close();
    });
    connect(m_addChildAction,&QAction::triggered,this,[=]()
    {
        QModelIndex index = indexAt(m_rootPoint);
        addChildItem(index);
    });

    connect(m_editAction,&QAction::triggered,this,[=]()
    {
        QObject *obj= m_architeSettingView->rootObject();
        QMetaObject::invokeMethod(obj,"clearTextField");
        m_architeSettingView->show();
    });

    connect(m_deleteAction,&QAction::triggered,this,[=]()
    {
        QModelIndex index = indexAt(m_rootPoint);
        deleteTreeItem(index);
    });
    connect(m_clearAction,&QAction::triggered,this,[=]()
    {
        clearItem();
    });
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&TreeView::customContextMenuRequested,this,[=](const QPoint&/*pos*/)
    {
        m_rootPoint=  QWidget::mapFromGlobal(QCursor::pos());
        m_menu->exec(QCursor::pos());
    });

}

TreeView::~TreeView()
{
    m_menu->close();
    delete m_menu;
    delete m_architeSettingView;
}



QMap<QStandardItem *, int> &TreeView::getTreeIndexMap()
{
    return m_treeIndexMap;
}

void TreeView::saveTreeItem()
{
    for(int i=0;i<m_stdModel->rowCount();i++)
    {
        QMap<QString,QVariant> rootMap;
        QStandardItem *parentItem = m_stdModel->item(i);
        if(parentItem)
        {
            rootMap["name"] =parentItem->text();
            Controller<JsonEdit>::instance()->insertRoot(rootMap);
            if(parentItem->hasChildren())
            {
                for(int j=0;j<parentItem->rowCount();j++)
                {
                    QStandardItem *childItem = parentItem->child(j);

                    if(childItem)
                    {
                        QMap<QString,QVariant> childMap;
                        childMap["name"] = childItem->text();
                        Controller<JsonEdit>::instance()->insertChild(i,childMap);
                    }
                }
            }

        }

    }
}

QStandardItem * TreeView::addRootItem()
{
    int rootCount= m_stdModel->rowCount();
    int totalRowCounts = getTotalCount();
    QStandardItem *rootItem =new QStandardItem(QString("第%1层").arg(rootCount+1));
    m_stdModel->insertRow(rootCount,rootItem);
    m_treeIndexMap[rootItem]=totalRowCounts;
    emit treeIndex(rootItem);
    m_parentIndexList.push_back(totalRowCounts);
    return rootItem;
}

QStandardItem* TreeView::addChildItem(QModelIndex index)
{

    if(!index.parent().isValid())
    {
        if(index.isValid())
        {
            QStandardItem *standardItem = m_stdModel->item(index.row());
            int totalRowCounts = getTotalCount();
            if(standardItem)
            {
                int rowCount =0;
                QList<int >childRowList = m_childIndexMap[index.row()];
                int size = childRowList.size();
                if(childRowList.size()>0)
                {
                    auto it= std::minmax_element(childRowList.begin(),childRowList.end()).second;
                    rowCount = *it+1;
                }
                QStandardItem*childItem = new QStandardItem(QString("%1").arg(rowCount+1));
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

void TreeView::deleteTreeItem(QModelIndex index)
{
    if(index.isValid())
    {
        QStandardItem *standardItem = m_stdModel->itemFromIndex(index);
        emit deleteIndex(standardItem);
        if(standardItem->parent()==nullptr)
        {
            m_stdModel->removeRow(index.row());
            if(m_parentIndexList.size()>index.row())
            {
                m_parentIndexList.removeAt(index.row());
            }

        }
        else
        {
            QStandardItem *parentItem =standardItem->parent();
            if(parentItem)
            {
                if(parentItem->rowCount()>index.row())
                {
                    parentItem->removeRow(index.row());
                    m_stdModel->setItem(index.parent().row(),parentItem);
                    if(m_childIndexMap[index.parent().row()].size()>index.row())
                        m_childIndexMap[index.parent().row()].removeAt(index.row());
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
    m_architeSettingView->setSource(QUrl("qrc:/qml/TreeViewSetting.qml"));
    m_architeSettingView->setGeometry(500,50,m_architeSettingView->width(),m_architeSettingView->height());
    m_architeSettingView->rootContext()->setContextProperty("TreeView",this);

    header()->hide();
    setModel(m_stdModel);
}

void TreeView::initMenu()
{
    m_menu = new QMenu;
    m_addAction  = new QAction(tr("增加行"),m_menu);
    m_addChildAction = new QAction(tr("增加子节点"),m_menu);
    m_editAction = new QAction(tr("编辑"),m_menu);
    m_deleteAction= new QAction(tr("删除"),m_menu);
    m_clearAction= new QAction(tr("清空"),m_menu);
    m_closeAction = new QAction(tr("关闭"),m_menu);
    m_menu->addAction(m_addAction);
    m_menu->addAction(m_addChildAction);
    m_menu->addAction(m_editAction);
    m_menu->addAction(m_deleteAction);
    m_menu->addAction(m_clearAction);
    m_menu->addAction(m_closeAction);

}

int TreeView::getTotalCount()
{
    int rootCount= m_stdModel->rowCount();
    int totalRowCounts = rootCount;
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
