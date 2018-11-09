#include "infotableview.h"
#include <QHBoxLayout>
InfoTableView::InfoTableView(QWidget *parent)
    : QTableView(parent)
{
  initWidget();
  //parent->layout()->setSpacing(0);
}

void InfoTableView::initWidget()
{
//    m_tableView = new QTableView(this);
//    QHBoxLayout *hLayout = new QHBoxLayout;
//    hLayout->addWidget(m_tableView);
//    hLayout->setContentsMargins(QMargins(0,0,0,0));
//    setLayout(hLayout);
}
