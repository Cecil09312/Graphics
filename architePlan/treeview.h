#ifndef TREEVIEW_H
#define TREEVIEW_H
#include <QTreeView>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QQuickView>
class ItemDelegate;
class TreeView : public QTreeView
{
    Q_OBJECT
public:
    TreeView(QWidget *parent = nullptr);
    ~TreeView();
    QMap<QStandardItem *, int> &getTreeIndexMap();
signals:
    void treeIndex(QStandardItem*item);
    void deleteIndex(QStandardItem*item);
    void clearIndex();
    void insertAnchPixmap(QStandardItem*item,const QString &fileName);

public slots:
    void addRootItem();
    void addChildItem(QModelIndex index);
    void deleteTreeItem(QModelIndex index);
    void clearItem();
    void setItemName(const QString &name);
    void insertPixmap(const QString &fileName);

protected:
    void mousePressEvent(QMouseEvent *event);
private:
    void initWidget();
    void initMenu();
    int getTotalCount();


private:
    QMenu *m_menu;
    QAction* m_addAction;
    QAction *m_addChildAction;
    QAction *m_editAction;
    QAction *m_closeAction;
    QAction *m_deleteAction;
    QAction *m_clearAction;
    QStandardItemModel *m_stdModel;
    int m_index;
    QMap<int,QList<int> >m_childIndexMap;
    QList<int>m_parentIndexList;
    QPoint m_rootPoint;
    QMap<QStandardItem*,int>m_treeIndexMap;
    ItemDelegate *m_itemDelegate;
    QQuickView *m_architeSettingView;
    QStandardItem *m_currentItem;
    //QWidget *m_architeSettingContainer;
};

class ItemDelegate:public QItemDelegate
{
public:
    ItemDelegate(QObject *parent = nullptr);
    ~ItemDelegate();
protected:
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

};

#endif // TREEVIEW_H
