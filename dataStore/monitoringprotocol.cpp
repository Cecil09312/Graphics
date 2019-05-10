#include "monitoringprotocol.h"
#include <QFuture>
#include <QtConcurrent>

MonitoringProtocol::MonitoringProtocol()
{

}

MonitoringProtocol::~MonitoringProtocol()
{

}

QList<QByteArray> MonitoringProtocol::frameData(const QByteArray &array)
{
    QList<QByteArray> arrayList;
    QFuture<void> futute = QtConcurrent::run([&]()
    {
        m_receiveDataArray.append(array);

        char start[] = "START";
        char end[] = "END";
        while (m_receiveDataArray.contains(start))
        {
            int startIndex = m_receiveDataArray.indexOf("START");
           // int endIndex = m_receiveDataArray.indexOf("END");
            //qDebug() << startIndex << endIndex;
            int arraySize = m_receiveDataArray.size();


            if(startIndex>0)
            {
                m_receiveDataArray = m_receiveDataArray.right(arraySize-startIndex);
                startIndex = 0;
            }
            if(m_receiveDataArray.size()>=9)
            {
                QByteArray packageLenArray= dataBytes(m_receiveDataArray,5,8);
                int packageLen= packageLenArray.toInt();
                qDebug() << "packageLen" << packageLen;
                if(m_receiveDataArray.size()>=packageLen)
                {
                    QByteArray frameArray = m_receiveDataArray.mid(startIndex,packageLen);
                    qDebug() << frameArray;

                    if(frameArray.right(3)==QByteArray(end))
                    {
                        QByteArray curArray = dataBytes(frameArray,5,packageLen-4);
                        arrayList.push_back(curArray);
                        m_dataHash[curArray] = dataBytes(frameArray,9,12).toInt();
                        m_receiveDataArray.remove(startIndex,packageLen);
                    }
                    else
                    {
                        m_receiveDataArray.remove(startIndex,5);//删除START
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    });
    futute.waitForFinished();


    return arrayList;
}

QByteArray MonitoringProtocol::dataPackage(const QList<QByteArray> &arrayList)
{
    char start[] = "START";
    char end[]="END";
    QByteArray startArray(start);
    QByteArray endArray(end);
    QByteArray array;
    QFuture<void> futute = QtConcurrent::run([&]()
    {
        array.push_back(startArray);//包括包长、索引等
        foreach (QByteArray dataArray, arrayList)
        {
            array.push_back(dataArray);
        }
        array.push_back(endArray);
    });
    futute.waitForFinished();

    return array;
}

int MonitoringProtocol::dataPackageNum(const QByteArray &dataArray)
{
    if(m_dataHash.contains(dataArray))
    {
        return m_dataHash[dataArray];
    }
    else
    {
        return 0;
    }
}
