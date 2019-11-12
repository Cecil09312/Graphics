#ifndef SERIALDATAPROTOCOL_H
#define SERIALDATAPROTOCOL_H
#include "abstractdataprotocol.h"
#include <QThread>

class SerialDataProtocol : public AbstractDataProtocol
{
    Q_OBJECT
public:
    SerialDataProtocol();
    ~SerialDataProtocol();
    QByteArray dataPackage(const QList<QByteArray> &arrayList);
    QList<QByteArray>frameData(const QByteArray &array);
    int dataPackageNum(const QByteArray &dataArray);

private:
    QByteArray m_receiveDataArray;
    QHash<QByteArray,int>m_dataHash;
    bool m_threadState;
};

#endif // SERIALDATAPROTOCOL_H
