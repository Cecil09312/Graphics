#ifndef INFOTABLEVIEW_H
#define INFOTABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include "qmlTableModel/qmltablemodel.h"

class InfoTableView : public QTableView
{
    Q_OBJECT
public:
    explicit InfoTableView(QWidget *parent = nullptr);
    ~InfoTableView();

signals:

public slots:
private:
    void initWidget();
private:
   // QTableView *m_tableView;
    QmlTableModel  *m_tableModel;
};

#endif // INFOTABLEVIEW_H
