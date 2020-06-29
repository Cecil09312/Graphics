#ifndef SQLITEMANAGER_H
#define SQLITEMANAGER_H
#include "sqlmanager.h"
#include <QTimer>

class SqliteManager : public SqlManager
{
    Q_OBJECT
public:
    SqliteManager(QObject *parent = nullptr);
    ~SqliteManager();
    QStringList getDatabases();
    QStringList executeQuery(const QString &sql);
    void quitThread();
    void processAnalogData(bool isAnalog);

signals:
    void finishedProcessData();
    void selectData(const QSqlQuery &query);
    void startProcessData(const QString&sql);
    void finishUpdateData();

private:
    bool m_startRunning;
    QList<QString>m_sqlList;
    QStringList  m_selectAlarmInfoList;
    QMutex m_mutex;
    QThread *m_thread;
    QTimer *m_timer;
    bool m_isAnalog;
};

#endif // SQLITEMANAGER_H
