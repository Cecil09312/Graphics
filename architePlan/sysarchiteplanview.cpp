#include "sysarchiteplanview.h"
#include <QHBoxLayout>
#include <QDebug>
SysArchitePlanView::SysArchitePlanView(QWidget *parent)
    : QWidget(parent)
{
   init();
    connect(m_listView,&QListView::clicked,this,[=](const QModelIndex &index){
        QString value =m_strListModel->data(index).toString();
        m_stackedWidget->setCurrentWidget(m_graphicsViewHash[value]);
    });
}

void SysArchitePlanView::init()
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    m_stackedWidget = new QStackedWidget(this);
    m_listView = new QListView(this);
    m_listView->setMaximumWidth(150);
    m_listView->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    hLayout->addWidget(m_listView);
    hLayout->addWidget(m_stackedWidget);
    setLayout(hLayout);

    m_strListModel = new QStringListModel(this);
    QStringList sysViewNameList;
    sysViewNameList << tr("火灾自动报警系统")<< tr("联防控制系统")
                    << tr("自动喷水灭火系统")<< tr("消火栓系统")
                    << tr("气体灭火系统")<< tr("水喷雾灭火系统")
                    << tr("泡沫和干粉灭火系统")<< tr("防烟排烟系统")
                    << tr("消防应急照明系统") << tr("疏散指示系统");
    m_strListModel->setStringList(sysViewNameList);
    m_listView->setModel(m_strListModel);
    for(int i=0;i<sysViewNameList.size();i++)
    {
        GraphicsView *graphicsView =new GraphicsView(this,GraphicsView::SysArthitePlan);
        m_stackedWidget->addWidget(graphicsView);
        m_graphicsViewHash[sysViewNameList.at(i)] = graphicsView;
    }

}
