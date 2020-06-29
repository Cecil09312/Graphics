#include "sysarchiteplanview.h"
#include <QHBoxLayout>
#include <QDebug>
SysArchitePlanView::SysArchitePlanView(QWidget *parent)
    : QWidget(parent)
{
    init();
    // setSysArchitePlanInfo();
    connect(m_listView,&QListView::clicked,this,[=](const QModelIndex &index)
    {
        QString value =m_itemModel->data(index,Qt::UserRole+1).toString();

        m_graphicsView->loadPixmap(m_imageNameHash.value(value).toString());
        m_graphicsView->updateSvg();
        if(m_graphicsView->sysScene()!=nullptr)
        {
           m_graphicsView->sysScene()->update();
        }

    });
}

SysArchitePlanView::~SysArchitePlanView()
{

}

QVariant SysArchitePlanView::infoToJson()
{
    //    QHash<QString,QVariant> imageHash;
    //    foreach (QString graphicsImageName, m_imageNameHash)
    //    {
    //        imageHash[graphicsImageName] = m_imageNameHash.value(graphicsImageName);
    //    }
    return m_imageNameHash;
}

void SysArchitePlanView::retranslate()
{

    if(m_itemModel->rowCount()>9)
    {

        m_itemModel->item(0)->setData(tr("火灾自动报警系统"));
        m_itemModel->item(1)->setData(tr("消防联动控制系统"));
        m_itemModel->item(2)->setData(tr("自动喷水灭火系统"));
        m_itemModel->item(3)->setData(tr("消火栓系统"));
        m_itemModel->item(4)->setData(tr("气体灭火系统"));
        m_itemModel->item(5)->setData(tr("水喷雾灭火系统"));
        m_itemModel->item(6)->setData(tr("泡沫和干粉灭火系统"));
        m_itemModel->item(7)->setData(tr("防烟排烟系统"));
        m_itemModel->item(8)->setData(tr("消防应急照明系统"));
        m_itemModel->item(9)->setData(tr("疏散指示系统"));

    }

}

void SysArchitePlanView::setSysArchitePlanInfo()
{

    QmlForJson qmlForJoson;
    QVariant jsonValue = qmlForJoson.readFile();
    QHash<QString,QVariant> infoHash= jsonValue.toHash();
    QHash<QString,QVariant> sysArchitePlanHash=infoHash["sysArchitePlan"].toHash();
    QList<QString> sysArchitePlanList = sysArchitePlanHash.keys();
    foreach (QString value, sysArchitePlanList)
    {
        m_imageNameHash[value]=sysArchitePlanHash[value].toString();
    }

    if(m_imageNameHash.contains(tr("火灾自动报警系统")))
    {
        m_graphicsView->loadPixmap(m_imageNameHash.value(tr("火灾自动报警系统")).toString());
        m_graphicsView->updateSvg();
        if(m_graphicsView->sysScene()!=nullptr)
        {
           m_graphicsView->sysScene()->update();
        }

    }
}


void SysArchitePlanView::setSysDrawing(const QString &sysName, const QString &fileName)
{
    m_imageNameHash[sysName] = fileName;
    QModelIndex curIndex=  m_listView->currentIndex();
    QString value =m_itemModel->data(curIndex,Qt::UserRole+1).toString();
    if(sysName.contains(value))
    {
        m_graphicsView->loadPixmap(fileName);
        m_graphicsView->updateSvg();
        if(m_graphicsView->sysScene()!=nullptr)
        {
           m_graphicsView->sysScene()->update();
        }
    }
}

void SysArchitePlanView::setGraphicsViewScale(qreal scale)
{
    GraphicsView::zoom(scale);
}

void SysArchitePlanView::currentGraphicsViewZoom(bool isZoomIn)
{

    if(m_graphicsView!=nullptr)
    {
        if(isZoomIn)
        {
            m_graphicsView->zoomIn();
        }
        else
        {
            m_graphicsView->zoomOut();
        }
    }
}

void SysArchitePlanView::init()
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    m_graphicsView = new GraphicsView(this,GraphicsView::SysArthitePlan);
    m_listView = new QListView(this);
    m_delegate = new StyledItemDelegate(this);
    m_itemModel = new QStandardItemModel(this);
    m_listView->setModel(m_itemModel);
    m_listView->setEditTriggers(QListView::NoEditTriggers);
    m_listView->setItemDelegate(m_delegate);
    m_listView->setMaximumWidth(180);
    m_listView->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    hLayout->addWidget(m_listView);
    hLayout->addWidget(m_graphicsView);
    setLayout(hLayout);


    m_sysViewNameList<< tr("火灾自动报警系统")<< tr("消防联动控制系统")
                     << tr("自动喷水灭火系统")<< tr("消火栓系统")
                     << tr("气体灭火系统")<< tr("水喷雾灭火系统")
                     << tr("泡沫和干粉灭火系统")<< tr("防烟排烟系统")
                     << tr("消防应急照明系统") << tr("疏散指示系统");
    // m_strListModel->setStringList(sysViewNameList);

    foreach(const QString &sysViewName,m_sysViewNameList)
    {
        QStandardItem *item = new QStandardItem;
        item->setData(sysViewName);
        m_itemModel->appendRow(item);
    }

}

