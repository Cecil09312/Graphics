#ifndef INFOTABLEVIEW_H
#define INFOTABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>

class InfoTableView : public QTableView
{
    Q_OBJECT
public:
    explicit InfoTableView(QWidget *parent = nullptr);

signals:

public slots:
private:
    void initWidget();
private:
   // QTableView *m_tableView;
};

#endif // INFOTABLEVIEW_H
