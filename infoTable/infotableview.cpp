#include "infotableview.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QScrollBar>
#include <QtMath>
#include "control/controller.h"

InfoTableView::InfoTableView(QWidget *parent)
    : QTableView(parent)
{
    initWidget();

    setContextMenuPolicy(Qt::CustomContextMenu);

   // horizontalHeader()->setSectionsMovable(true);
   // horizontalHeader()->setDragEnabled(true);
    //horizontalHeader()->setDragDropMode(QAbstractItemView::InternalMove);

    connect(this,&InfoTableView::customContextMenuRequested,this,[=](const QPoint&/*pos*/)
    {
        m_menu->exec(QCursor::pos());
    });

    connect(m_analogValueShowAction,&QAction::triggered,this,&InfoTableView::showAlalogValue);
    connect(m_alarmInfoShowAction,&QAction::triggered,this,&InfoTableView::showAlarmValue);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(this->verticalHeader(),&QHeaderView::sectionDoubleClicked,this,[=](int logicalIndex)
    {
        QSqlRecord record = m_tableModel->record(logicalIndex);
        emit tableValue(record);
        emit setAlarmNum(m_tableModel->rowCount(),logicalIndex+1);
    }) ;

    connect(this->horizontalHeader(),&QHeaderView::sectionClicked,this,[=](int logicalIndex)
    {
        Q_UNUSED(logicalIndex) ;
        emit fitToWiew();
    }) ;

    connect(this,&InfoTableView::clicked,this,[=](const QModelIndex &index)
    {
        QSqlRecord record = m_tableModel->record(index.row());
        emit tableValue(record);
        emit setAlarmNum(m_tableModel->rowCount(),index.row()+1);
    });
}

InfoTableView::~InfoTableView()
{
    //m_tableModel->setDbOpen(false);
    delete m_menu;
}



QmlTableModel *InfoTableView::tableModel()
{
    return m_tableModel;
}

void InfoTableView::retranslate()
{
    setTableHeader();
    m_analogValueShowAction->setText(tr("模拟量"));
    m_alarmInfoShowAction->setText(tr("报警信息"));


}

void InfoTableView::setTableHeader()
{
    QList<QString>alarmInfoList;
    alarmInfoList <<tr("网络号")<< tr("分机号")<<tr("回路号")<<tr("地址号")<<tr("电源地址")<< tr("设备")
                 <<tr("事件类型")<<tr("时间")<<tr("系统")<< tr("建筑名称")<<tr("楼层")<<tr("位置")<<tr("备注");
    for(int i=0;i<alarmInfoList.size();i++)
    {
        tableModel()->setHeaderData(i,Qt::Horizontal,alarmInfoList.at(i));
    }
}



void InfoTableView::toMaxPosition()
{
   // Controller::instance()->delayMs(50);
    int rowCount = m_tableModel->rowCount();
    int currentHeight=0;
    if(rowCount>0)
    {
        int singleHeght = rowHeight(0);
        for(int i=0;i<rowCount;i++)
        {
            currentHeight+=rowHeight(i);
        }

        if(currentHeight>height()-horizontalHeader()->height())
        {
          qreal maxStep=  (currentHeight-height()+horizontalHeader()->height())*1.0/singleHeght;
          verticalScrollBar()->setValue(qCeil(maxStep)*verticalScrollBar()->singleStep());
        }

    }
}

void InfoTableView::initWidget()
{


    m_tableModel = new QmlTableModel (this);
    m_menu = new QMenu;
    m_analogValueShowAction = new QAction(tr("模拟量"),m_menu);
    m_alarmInfoShowAction = new QAction(tr("报警信息"),m_menu);
    m_menu->addAction(m_analogValueShowAction);
    m_menu->addAction(m_alarmInfoShowAction);
    this->setModel(m_tableModel);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->horizontalHeader()->setStretchLastSection(true);
   // horizontalHeader()->setMinimumSectionSize(60);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
