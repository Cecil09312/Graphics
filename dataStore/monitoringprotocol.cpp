#include "monitoringprotocol.h"

MonitoringProtocol::MonitoringProtocol()
{

}

MonitoringProtocol::~MonitoringProtocol()
{

}

QList<QByteArray> MonitoringProtocol::frameData(QByteArray &array)
{
    QList<QByteArray> arrayList;
    while (array.contains("START"))
    {
        int startIndex = array.indexOf("START");
        int endIndex = array.indexOf("END");
        int arraySize = array.size();
        if(startIndex>0)
        {
            array = array.right(arraySize-startIndex);
            startIndex = 0;
        }
        if(endIndex>0)
        {
            QByteArray frameArray = array.mid(startIndex,qAbs(endIndex-startIndex)+1);
            arrayList.push_back(frameArray);
        }
    }
    return arrayList;
}

QByteArray MonitoringProtocol::dataPackage(const QList<QByteArray> &arrayList)
{
    QByteArray startArray("START");
    QByteArray endArray("END");
    QByteArray array;
    array.push_back(startArray);
    foreach (QByteArray dataArray, arrayList)
    {
        array.push_back(dataArray);
    }
    array.push_back(endArray);
    return array;
}
