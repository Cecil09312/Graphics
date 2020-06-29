#ifndef OPERATORINFO_H
#define OPERATORINFO_H

#include <QObject>
#include "sqlitemanager.h"
#include "sqlmanager.h"
#include <QHash>
class OperatorInfo : public QObject
{
    Q_OBJECT
public:
    explicit OperatorInfo(QObject *parent = nullptr);
    ~OperatorInfo();
    Q_INVOKABLE QString operatorInfoDbPath();
    Q_INVOKABLE void saveMaintInfo();
    Q_INVOKABLE void setMaintInfoValue(const QString &key,const QString &value);

signals:

public slots:
    void insertEvent(const QString &eventName, const QString &result = tr("成功"));
private:
    SqliteManager *m_sqlManager;
    QString m_operatorInfoDbPath;
    QHash<QString,QString>m_maintInfoValueHash;
    int m_maintInfoTableSize;
};

#endif // OPERATORINFO_H
