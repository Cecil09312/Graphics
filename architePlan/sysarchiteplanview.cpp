#include "sysarchiteplanview.h"
#include <QHBoxLayout>
#include <QDebug>
SysArchitePlanView::SysArchitePlanView(QWidget *parent)
    : QWidget(parent)
{
    init();
    setSysArchitePlanInfo();
    connect(m_listView,&QListView::clicked,this,[=](const QModelIndex &index){
        QString value =m_itemModel->data(index,Qt::UserRole+1).toString();
        m_stackedWidget->setCurrentWidget(m_graphicsViewHash[value]);
    });
}

SysArchitePlanView::~SysArchitePlanView()
{

}

QVariant SysArchitePlanView::infoToJson()
{
    QHash<QString,QVariant> graphicsHash;
    QList<QString> graphicsNameList= m_graphicsViewHash.keys();
    foreach (QString graphicsName, graphicsNameList)
    {
        GraphicsView *view = m_graphicsViewHash[graphicsName];
        if(view!=nullptr)
        {
            graphicsHash[graphicsName] = view->pixmapName();
        }

    }
    return graphicsHash;
}

void SysArchitePlanView::setSysArchitePlanInfo()
{
    QVariant jsonValue = QmlForJson::readFile();
    QHash<QString,QVariant> infoHash= jsonValue.toHash();
    QHash<QString,QVariant> sysArchitePlanHash=infoHash["sysArchitePlan"].toHash();
    QList<QString> sysArchitePlanList = sysArchitePlanHash.keys();
    foreach (QString value, sysArchitePlanList)
    {
        GraphicsView *graphicsView=  m_graphicsViewHash[value];
        if(graphicsView!=nullptr)
        {
            graphicsView->loadPixmap(sysArchitePlanHash[value].toString());
        }
    }
}


void SysArchitePlanView::setSysDrawing(const QString &sysName, const QString &fileName)
{
    GraphicsView* graphicsView=m_graphicsViewHash[sysName];
    if(graphicsView!=nullptr)
    {
        graphicsView->loadPixmap(fileName);
    }
}

void SysArchitePlanView::setGraphicsViewScale(qreal scale)
{
    GraphicsView::zoom(scale);
}

void SysArchitePlanView::currentGraphicsViewZoom(bool isZoomIn)
{
    GraphicsView*graphicsView =dynamic_cast<GraphicsView*>(m_stackedWidget->currentWidget()) ;
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

void SysArchitePlanView::init()
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    m_stackedWidget = new QStackedWidget(this);
    m_listView = new QListView(this);
    m_delegate = new StyledItemDelegate(this);
    m_itemModel = new QStandardItemModel(this);
    m_listView->setModel(m_itemModel);
    m_listView->setEditTriggers(QListView::NoEditTriggers);
    m_listView->setItemDelegate(m_delegate);
    m_listView->setMaximumWidth(180);
    m_listView->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    hLayout->addWidget(m_listView);
    hLayout->addWidget(m_stackedWidget);
    setLayout(hLayout);


    QStringList sysViewNameList;
    sysViewNameList<< tr("火灾自动报警系统")<< tr("消防联动控制系统")
                   << tr("自动喷水灭火系统")<< tr("消火栓系统")
                   << tr("气体灭火系统")<< tr("水喷雾灭火系统")
                   << tr("泡沫和干粉灭火系统")<< tr("防烟排烟系统")
                   << tr("消防应急照明系统") << tr("疏散指示系统");
    // m_strListModel->setStringList(sysViewNameList);

    foreach(const QString &sysViewName,sysViewNameList)
    {
        GraphicsView *graphicsView =new GraphicsView(this,GraphicsView::SysArthitePlan);
        m_stackedWidget->addWidget(graphicsView);
        m_graphicsViewHash[sysViewName] = graphicsView;
        QStandardItem *item = new QStandardItem;
        item->setData(sysViewName);
        m_itemModel->appendRow(item);
    }

    // m_stackedWidget->currentWidget()
}

