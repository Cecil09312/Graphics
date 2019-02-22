#ifndef INDICATORDATAPROTOCOL_H
#define INDICATORDATAPROTOCOL_H
#include "abstractdataprotocol.h"

class IndicatorDataProtocol : public AbstractDataProtocol
{
public:
    IndicatorDataProtocol();
    ~IndicatorDataProtocol();
    QByteArray dataPackage(const QList<QByteArray> &arrayList);
    QList<QByteArray>frameData(QByteArray &array);
};

#endif // INDICATORDATAPROTOCOL_H
