#include "indicatordataprotocol.h"

IndicatorDataProtocol::IndicatorDataProtocol()
{

}

IndicatorDataProtocol::~IndicatorDataProtocol()
{

}

QByteArray IndicatorDataProtocol::dataPackage(const QList<QByteArray> &arrayList)
{
    QByteArray dataArray;
    dataArray.push_back(0x0a);
    foreach (QByteArray array, arrayList)
    {
        dataArray.push_back(array);
    }
    dataArray.push_back(0x0b);
    return dataArray;
}

QList<QByteArray> IndicatorDataProtocol::frameData(QByteArray &array)
{
    QList<QByteArray> dataArrayList;
    dataArrayList.push_back(array);
    return dataArrayList;
}
