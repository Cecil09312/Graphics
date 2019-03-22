#ifndef OPERATORINFO_H
#define OPERATORINFO_H

#include <QObject>
#include "sqlitemanager.h"
#include "sqlmanager.h"

class OperatorInfo : public QObject
{
    Q_OBJECT
public:
    explicit OperatorInfo(QObject *parent = nullptr);
    ~OperatorInfo();
    Q_INVOKABLE QString operatorInfoDbPath();

signals:

public slots:
    void insertEvent(const QString &eventName);
private:
    SqlManager *m_sqlManager;
    QString m_operatorInfoDbPath;
};

#endif // OPERATORINFO_H
