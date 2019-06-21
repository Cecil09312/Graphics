#include "indicatordataprotocol.h"
#include <QFuture>
#include <QtConcurrent>
IndicatorDataProtocol::IndicatorDataProtocol()
{

}

IndicatorDataProtocol::~IndicatorDataProtocol()
{

}

QByteArray IndicatorDataProtocol::dataPackage(const QList<QByteArray> &arrayList)
{
    QByteArray dataArray;
    QFuture<void> future= QtConcurrent::run([&]()
    {
        dataArray.push_back(0x0a);
        foreach (QByteArray array, arrayList)
        {
            dataArray.push_back(array);//数据
        }
        dataArray.push_back(0x0b);
    });
    future.waitForFinished();
    return dataArray;
}

QList<QByteArray> IndicatorDataProtocol::frameData(const QByteArray &array)
{
    QList<QByteArray> dataArrayList;
    QFuture<void> future= QtConcurrent::run([&]()
    {
      dataArrayList.push_back(array);
    });
    future.waitForFinished();

    return dataArrayList;
}

int IndicatorDataProtocol::dataPackageNum(const QByteArray &dataArray)
{
    Q_UNUSED(dataArray);
    return 0;
}
