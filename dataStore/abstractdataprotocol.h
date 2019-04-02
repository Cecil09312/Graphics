#ifndef ABSTRACTDATAPROTOCOL_H
#define ABSTRACTDATAPROTOCOL_H
#include <QByteArray>
#include <QDebug>

class AbstractDataProtocol
{
public:
    AbstractDataProtocol();
    virtual ~AbstractDataProtocol();
    virtual QList<QByteArray>frameData(const QByteArray &array)=0;
    quint8 dataByte(QByteArray frameArray,int pos);
    QByteArray dataBytes(QByteArray frameArray,int start,int end);
    virtual QByteArray dataPackage(const QList<QByteArray> &arrayList)=0;
};

#endif // ABSTRACTDATAPROTOCOL_H
