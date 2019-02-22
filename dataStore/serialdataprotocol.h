#ifndef SERIALDATAPROTOCOL_H
#define SERIALDATAPROTOCOL_H
#include "abstractdataprotocol.h"

class SerialDataProtocol : public AbstractDataProtocol
{
public:
    SerialDataProtocol();
    ~SerialDataProtocol();
    QByteArray dataPackage(const QList<QByteArray> &arrayList);
    QList<QByteArray>frameData(QByteArray &array);
};

#endif // SERIALDATAPROTOCOL_H
