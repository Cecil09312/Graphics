#ifndef INFOTABLEVIEW_H
#define INFOTABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include "qmlTableModel/qmltablemodel.h"
#include "database/sqlmanager.h"
#include <QSqlRecord>
#include <QMenu>
#include <QAction>
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
    void setAlarmNum(int totalNum,int curNum);
    void showAlalogValue();
    void showAlarmValue();
public slots:
    void toMaxPosition();
private:
    void initWidget();
private:
    QmlTableModel  *m_tableModel;
    QMenu *m_menu;
    QAction *m_analogValueShowAction;
    QAction *m_alarmInfoShowAction;
};

#endif // INFOTABLEVIEW_H
