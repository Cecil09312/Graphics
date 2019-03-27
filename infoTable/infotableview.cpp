#include "infotableview.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>


InfoTableView::InfoTableView(QWidget *parent)
    : QTableView(parent)
{
    initWidget();
    connect(this->verticalHeader(),&QHeaderView::sectionDoubleClicked,this,[=](int logicalIndex)
    {
        QSqlRecord record = m_tableModel->record(logicalIndex);
        emit tableValue(record);
    }) ;
}

InfoTableView::~InfoTableView()
{
    m_tableModel->setDbOpen(false);
}



QmlTableModel *InfoTableView::tableModel()
{
    return m_tableModel;
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
