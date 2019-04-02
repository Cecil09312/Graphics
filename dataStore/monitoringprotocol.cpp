#include "monitoringprotocol.h"

MonitoringProtocol::MonitoringProtocol()
{

}

MonitoringProtocol::~MonitoringProtocol()
{

}

QList<QByteArray> MonitoringProtocol::frameData(const QByteArray &array)
{
    m_receiveDataArray.append(array);
    QList<QByteArray> arrayList;
    while (m_receiveDataArray.contains("START"))
    {
        int startIndex = m_receiveDataArray.indexOf("START");
        int endIndex = m_receiveDataArray.indexOf("END");
        int arraySize = m_receiveDataArray.size();
        if(startIndex>0)
        {
            m_receiveDataArray = m_receiveDataArray.right(arraySize-startIndex);
            startIndex = 0;
        }
        if(endIndex>0)
        {
            QByteArray frameArray = m_receiveDataArray.mid(startIndex,qAbs(endIndex-startIndex)+1);
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
