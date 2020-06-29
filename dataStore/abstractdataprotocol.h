#ifndef ABSTRACTDATAPROTOCOL_H
#define ABSTRACTDATAPROTOCOL_H
#include <QByteArray>
#include <QDebug>
#include <QThread>

class AbstractDataProtocol:public QObject
{
    Q_OBJECT
public:
    AbstractDataProtocol(QObject*parent=nullptr);
    virtual ~AbstractDataProtocol();
    virtual QList<QByteArray>frameData(const QByteArray &array)=0;
    quint8 dataByte(QByteArray frameArray,int pos);
    virtual int dataPackageNum(const QByteArray &dataArray)=0;
    QByteArray dataBytes(QByteArray frameArray,int start,int end);
    QByteArray dataArray(const QList<QByteArray> &arrayList,int start=1);
public slots:
    virtual void startProcessData(const QByteArray&array);
private:
    virtual QByteArray dataPackage(const QList<QByteArray> &arrayList,int start)=0;
signals:
    void errorFrameData(const QByteArray &errorArray);
    void finishProcessData(const QByteArray&array);
    void processDataFinished();
    void storeData(const QByteArray&array);
    void startDealWithData();
};

#endif // ABSTRACTDATAPROTOCOL_H
