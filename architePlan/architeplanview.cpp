#include "architeplanview.h"
#include <QHBoxLayout>
#include <QLabel>

ArchitePlanView::ArchitePlanView(QWidget *parent)
    : QWidget(parent)
{

    initWidget();
    connect(m_treeView,&TreeView::treeIndex,this,[=](QStandardItem*item)
    {
        QMap<QStandardItem*,int>map= m_treeView->getTreeIndexMap();
        int page = map[item];
        if(m_widgetMap[page]==nullptr)
        {
            GraphicsWidget *widget = new GraphicsWidget(this);
            QLabel *label = new QLabel(widget);
            label->setText(QString("widget:%1").arg(page));
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
                GraphicsWidget*childWidget = m_widgetMap[chileItemPage];
                m_stackedWidget->removeWidget(childWidget);
                m_widgetMap.remove(chileItemPage);
                m_treeView->getTreeIndexMap().remove(childItem);
            }

        }

        int page =itemMap[item];
        GraphicsWidget*widget = m_widgetMap[page];
        m_stackedWidget->removeWidget(widget);
        m_widgetMap.remove(page);
        m_treeView->getTreeIndexMap().remove(item);
    });

}

ArchitePlanView::~ArchitePlanView()
{

}



void ArchitePlanView::initWidget()
{
    m_treeView = new TreeView(this);
    m_stackedWidget = new QStackedWidget(this);
    m_tabWidget = new QTabWidget(this);
    // m_graphicsWidget = new GraphicsWidget(this);


    //    m_quickView = new QQuickView;

    //    m_quickView->setSource(QUrl("qrc:/qml/CustomTreeView.qml"));
    //    m_widget = QWidget::createWindowContainer(m_quickView,this);
    //    m_widget->setMinimumWidth(180);
    //    m_widget->setMaximumWidth(180);
    //    m_widget->setMinimumHeight(500);

    m_treeView->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    m_treeView->setMaximumWidth(180);
    m_stackedWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_stackedWidget->setStyleSheet("QStackedWidget{border:1px solid black}");
    m_treeView->setStyleSheet("QTreeView{border:1px solid black}");

    QHBoxLayout*hLayout = new QHBoxLayout;
    QHBoxLayout*globalHLayout = new QHBoxLayout;
    hLayout->addWidget(m_stackedWidget);
    hLayout->addWidget(m_treeView);
    //hLayout->addWidget(m_widget);
    hLayout->setContentsMargins(QMargins(0,0,0,0));
    hLayout->setSpacing(0);
    QWidget *firstPageWidget = new QWidget(this);
    firstPageWidget->setLayout(hLayout);

    //m_stackedWidget->addWidget(m_graphicsWidget);
    //m_widgetList.push_back(m_graphicsWidget);
    m_tabWidget->addTab(firstPageWidget,tr("建筑平面图"));
    globalHLayout->addWidget(m_tabWidget);
    globalHLayout->setContentsMargins(QMargins(0,0,0,0));
    setLayout(globalHLayout);

    //    m_architeSettingView = new QQuickView;
    //   m_architeSettingView->setSource(QUrl("qrc:/qml/TreeViewSetting.qml"));

}
