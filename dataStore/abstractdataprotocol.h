#ifndef ABSTRACTDATAPROTOCOL_H
#define ABSTRACTDATAPROTOCOL_H
#include <QByteArray>
#include <QDebug>
#include <QObject>

class AbstractDataProtocol:public QObject
{
    Q_OBJECT
public:
    AbstractDataProtocol();
    virtual ~AbstractDataProtocol();
    virtual QList<QByteArray>frameData(const QByteArray &array)=0;
    quint8 dataByte(QByteArray frameArray,int pos);
    virtual int dataPackageNum(const QByteArray &dataArray)=0;
    QByteArray dataBytes(QByteArray frameArray,int start,int end);
    QByteArray dataArray(const QList<QByteArray> &arrayList,int start=1);
private:
    virtual QByteArray dataPackage(const QList<QByteArray> &arrayList,int start)=0;
signals:
    void errorFrameData(const QByteArray &errorArray);
};

#endif // ABSTRACTDATAPROTOCOL_H
