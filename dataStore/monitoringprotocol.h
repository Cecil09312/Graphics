#ifndef MONITORINGPROTOCOL_H
#define MONITORINGPROTOCOL_H

#include "abstractdataprotocol.h"
class MonitoringProtocol : public AbstractDataProtocol
{
public:
    MonitoringProtocol();
    ~MonitoringProtocol();
    QList<QByteArray>frameData(QByteArray &array);
    QByteArray dataPackage(const QList<QByteArray> &arrayList);
};

#endif // MONITORINGPROTOCOL_H
