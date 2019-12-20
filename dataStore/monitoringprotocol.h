#ifndef MONITORINGPROTOCOL_H
#define MONITORINGPROTOCOL_H

#include "abstractdataprotocol.h"
class MonitoringProtocol : public AbstractDataProtocol
{
    Q_OBJECT
public:
    MonitoringProtocol();
    ~MonitoringProtocol();
    QList<QByteArray>frameData(const QByteArray &array);
    QByteArray dataPackage(const QList<QByteArray> &arrayList,int startIndex=1);
    int dataPackageNum(const QByteArray &dataArray);
private:
    QByteArray m_receiveDataArray;
    QHash<QByteArray,int>m_dataHash;
};

#endif // MONITORINGPROTOCOL_H
