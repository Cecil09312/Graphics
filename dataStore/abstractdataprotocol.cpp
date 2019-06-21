#include "abstractdataprotocol.h"

AbstractDataProtocol::AbstractDataProtocol()
{

}

AbstractDataProtocol::~AbstractDataProtocol()
{

}

quint8 AbstractDataProtocol::dataByte(QByteArray frameArray, int pos)
{
    if(frameArray.size()>pos)
    {
        return frameArray.mid(pos,1).toHex().toUShort(0,16)&0xff;
    }
    else
    {
        return 0;
    }
}

QByteArray AbstractDataProtocol::dataBytes(QByteArray frameArray, int start, int end)
{
    int min = qMin(start,end);
    int max = qMax(start,end);

    if(frameArray.size()>max && min>=0)
    {
        return frameArray.mid(start,qAbs(end-start)+1);
    }
    else
    {
        return QByteArray();
    }
}
