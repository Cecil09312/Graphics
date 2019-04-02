#ifndef MONITORINGPROTOCOL_H
#define MONITORINGPROTOCOL_H

#include "abstractdataprotocol.h"
class MonitoringProtocol : public AbstractDataProtocol
{
public:
    MonitoringProtocol();
    ~MonitoringProtocol();
    QList<QByteArray>frameData(const QByteArray &array);
    QByteArray dataPackage(const QList<QByteArray> &arrayList);
private:
    QByteArray m_receiveDataArray;
};

#endif // MONITORINGPROTOCOL_H
