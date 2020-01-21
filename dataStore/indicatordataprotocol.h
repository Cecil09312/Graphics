#ifndef INDICATORDATAPROTOCOL_H
#define INDICATORDATAPROTOCOL_H
#include "abstractdataprotocol.h"

class IndicatorDataProtocol : public AbstractDataProtocol
{
    Q_OBJECT
public:
    IndicatorDataProtocol();
    ~IndicatorDataProtocol();

    QList<QByteArray>frameData(const QByteArray &array);
    int dataPackageNum(const QByteArray &dataArray);
private:
    QByteArray dataPackage(const QList<QByteArray> &arrayList,int start);
};

#endif // INDICATORDATAPROTOCOL_H
