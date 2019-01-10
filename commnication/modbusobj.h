#ifndef MODBUSOBJ_H
#define MODBUSOBJ_H

#include <QObject>

class ModbusObj : public QObject
{
    Q_OBJECT
public:
    explicit ModbusObj(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MODBUSOBJ_H