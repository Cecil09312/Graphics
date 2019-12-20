#include "serialdataprotocol.h"
#include <QFuture>
#include <QtConcurrent>

SerialDataProtocol::SerialDataProtocol()
{

}

SerialDataProtocol::~SerialDataProtocol()
{

}

QByteArray SerialDataProtocol::dataPackage(const QList<QByteArray> &arrayList, int start)
{
    QByteArray array;
    QFuture<void> future=  QtConcurrent::run([&]()
    {
        quint8 verifyData =0;
        array.push_back(0x40);
        array.push_back(0x40);
        foreach (QByteArray customArray, arrayList)
        {
            array.push_back(customArray);//包括包序号、包长、数据。
        }
        if(arrayList.size()>1)
        {
            quint32 sum=0;
            for(int i=start;i<arrayList.size();i++)
            {
                sum +=dataByte(arrayList.at(i),0);
            }
            verifyData = sum&0xff;
        }
        array.push_back(verifyData);//数据校验
        array.push_back(0x23);
        array.push_back(0x23);

    });
    future.waitForFinished();
    return array;
}

QList<QByteArray> SerialDataProtocol::frameData(const QByteArray &array)
{
    QList<QByteArray> arrayList;
    QFuture<void> future=  QtConcurrent::run([&]()
    {
        m_receiveDataArray.append(array);

        while (m_receiveDataArray.contains(0x7e))
        {
            int size = m_receiveDataArray.size();
            int startIndex=m_receiveDataArray.indexOf(0x7e);
            if(startIndex>0)
            {
                m_receiveDataArray = m_receiveDataArray.right(size-startIndex);
                startIndex =0;
            }
            if(m_receiveDataArray.size()>3)
            {
                quint8 frameLen = dataByte(m_receiveDataArray,2);
                if(frameLen%10==0)
                {
                    if(m_receiveDataArray.size()>=frameLen+5)
                    {
                        QByteArray frameArray = dataBytes(m_receiveDataArray,0,frameLen+4);
                        quint32 sum =0;
                        quint8 average =0;
                        for(int i=0;i<frameLen;i++)
                        {
                            sum +=dataByte(frameArray,i+3);
                        }
                        average = sum&0xff;
                        if(average==dataByte(frameArray,frameLen+3)&&dataByte(frameArray,frameLen+4)==0x7e)//校验码和帧尾验证
                        {
                            m_receiveDataArray.remove(0,frameArray.size());
                            QByteArray dataArray = dataBytes(frameArray,3,frameLen+2);
                            while(dataArray.size()>=10)
                            {
                                arrayList.push_back(dataArray.left(10));
                                m_dataHash[dataArray.left(10)]=dataByte(frameArray,1);
                                dataArray.remove(0,10);
                            }

                        }
                        else
                        {
                             m_receiveDataArray.remove(0,frameLen+5);
                             emit errorFrameData(frameArray);
                             if(m_receiveDataArray.isEmpty())
                             {
                                 break;
                             }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    m_receiveDataArray.remove(startIndex+1,1);
                }
            }
            else
            {
                break;
            }
        }
    });
    future.waitForFinished();

    return arrayList;
}

int SerialDataProtocol::dataPackageNum(const QByteArray &dataArray)
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

