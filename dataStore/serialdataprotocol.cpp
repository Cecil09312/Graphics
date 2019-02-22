#include "serialdataprotocol.h"

SerialDataProtocol::SerialDataProtocol()
{

}

SerialDataProtocol::~SerialDataProtocol()
{

}

QByteArray SerialDataProtocol::dataPackage(const QList<QByteArray> &arrayList)
{
    QByteArray array;
    array.push_back(0x40);
    array.push_back(0x40);
    foreach (QByteArray customArray, arrayList)
    {
        array.push_back(customArray);
    }
    array.push_back(0x23);
    array.push_back(0x23);
    return array;
}

QList<QByteArray> SerialDataProtocol::frameData(QByteArray &array)
{
    QList<QByteArray> arrayList;
    int size = array.size();
    while (array.contains(0x7e))
    {
        int startIndex=array.indexOf(0x7e);
        if(startIndex>0)
        {
            array = array.right(size-startIndex);
            startIndex =0;
        }
        if(array.size()>3)
        {
            quint8 frameLen = dataByte(array,2);
            if(frameLen%10==0)
            {
                if(array.size()>=frameLen+5)
                {
                    QByteArray frameArray = dataBytes(array,0,frameLen+4);
                    quint32 sum =0;
                    quint8 average =0;
                    for(int i=0;i<frameLen;i++)
                    {
                        sum +=dataByte(frameArray,i+3);
                    }
                    average = sum&0xff;
                    if(average==dataByte(frameArray,frameLen+3)&&dataByte(frameArray,frameLen+4)==0x7e)//校验码和帧尾验证
                    {
                        array.remove(0,frameArray.size());
                        QByteArray dataArray = dataBytes(frameArray,3,frameLen+2);
                        arrayList.push_back(dataArray);
                       // qDebug() << dataArray.toHex();
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                array.remove(startIndex+1,1);
            }
        }
        else
        {
            break;
        }
    }
    return arrayList;
}

