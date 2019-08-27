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
    setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(this->verticalHeader(),&QHeaderView::sectionDoubleClicked,this,[=](int logicalIndex)
    {
        QSqlRecord record = m_tableModel->record(logicalIndex);
        emit tableValue(record);
    }) ;

    connect(this->horizontalHeader(),&QHeaderView::sectionDoubleClicked,this,[=](int logicalIndex)
    {
        Q_UNUSED(logicalIndex) ;
        emit fitToWiew();
    }) ;

    connect(this,&InfoTableView::doubleClicked,this,[=](const QModelIndex &index)
    {
        QSqlRecord record = m_tableModel->record(index.row());
        emit tableValue(record);
    });
}

InfoTableView::~InfoTableView()
{
    m_tableModel->setDbOpen(false);
}



QmlTableModel *InfoTableView::tableModel()
{
    return m_tableModel;
}

void InfoTableView::toMaxPosition()
{
    Controller::instance()->delayMs(50);
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

    m_tableModel = new QmlTableModel(this);
    this->setModel(m_tableModel);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //    m_tableModel->setDbDriver("QSQLITE");
    //    m_tableModel->setDbHost("sss");
    //    m_tableModel->setDbName("C:/Users/1/Desktop/py.db");
    //    m_tableModel->setDbConnectionName("defaultName3");
    //    m_tableModel->setDbPassword("rrr");
    //    m_tableModel->setDbPort(222);
    //    m_tableModel->setDbUser("song");
    //    m_tableModel->setDbOpen(true);
    //    m_tableModel->sqlCommit("select *from pinyin");
}
