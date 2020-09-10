#ifndef SERIALDATAPROTOCOL_H
#define SERIALDATAPROTOCOL_H
#include "abstractdataprotocol.h"
#include <QThread>
#include <QMutex>
#include <QTimer>
class SerialDataProtocol : public AbstractDataProtocol
{
    Q_OBJECT
public:
    SerialDataProtocol(QObject*parent=nullptr);
    ~SerialDataProtocol();

    QList<QByteArray>frameData(const QByteArray &array);
    int dataPackageNum(const QByteArray &dataArray);
public slots:
    void startProcessData(const QByteArray&array);
//protected:
//    void run();

private:
    QByteArray dataPackage(const QList<QByteArray> &arrayList,int start);

private:
    QByteArray m_receiveDataArray;
    QHash<QByteArray,int>m_dataHash;
    bool m_threadState;
    //bool m_startThread;
    QList<QByteArray>m_dataArrayList;
    QMutex m_mutex;
    QThread *m_thread;
    QTimer*m_processDataTimer;
};

#endif // SERIALDATAPROTOCOL_H
