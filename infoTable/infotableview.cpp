#include "infotableview.h"
#include <QHBoxLayout>
#include <QHeaderView>

InfoTableView::InfoTableView(QWidget *parent)
    : QTableView(parent)
{
    initWidget();
    //parent->layout()->setSpacing(0);
}

InfoTableView::~InfoTableView()
{
    m_tableModel->setDbOpen(false);
}

void InfoTableView::initWidget()
{

    m_tableModel = new QmlTableModel(this);

    this->setModel(m_tableModel);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableModel->setDbDriver("QSQLITE");
    m_tableModel->setDbHost("sss");
    m_tableModel->setDbName("C:/Users/1/Desktop/mySqlite.db");
    m_tableModel->setDbConnectionName("defaultName3");
    m_tableModel->setDbPassword("rrr");
    m_tableModel->setDbPort(222);
    m_tableModel->setDbUser("song");
    m_tableModel->setDbOpen(true);
    m_tableModel->sqlCommit("select *from COMPANY");
}
