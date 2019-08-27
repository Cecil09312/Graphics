#ifndef INFOTABLEVIEW_H
#define INFOTABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include "qmlTableModel/qmltablemodel.h"
#include "database/sqlmanager.h"
#include <QSqlRecord>
class InfoTableView : public QTableView
{
    Q_OBJECT
public:
    explicit InfoTableView(QWidget *parent = nullptr);
    ~InfoTableView();
    QmlTableModel  *tableModel();

signals:
    void tableValue(QSqlRecord record);
    void fitToWiew();
public slots:
    void toMaxPosition();
private:
    void initWidget();
private:
    QmlTableModel  *m_tableModel;
};

#endif // INFOTABLEVIEW_H
