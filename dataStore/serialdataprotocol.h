#ifndef SERIALDATAPROTOCOL_H
#define SERIALDATAPROTOCOL_H
#include "abstractdataprotocol.h"

class SerialDataProtocol : public AbstractDataProtocol
{
public:
    SerialDataProtocol();
    ~SerialDataProtocol();
    QByteArray dataPackage(const QList<QByteArray> &arrayList);
    QList<QByteArray>frameData(const QByteArray &array);
    int dataPackageNum(const QByteArray &dataArray);
private:
    QByteArray m_receiveDataArray;
    QHash<QByteArray,int>m_dataHash;
};

#endif // SERIALDATAPROTOCOL_H
