#ifndef COMMOBJ_H
#define COMMOBJ_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <QSerialPortInfo>
class CommObj : public QObject
{
    Q_OBJECT
public:
    explicit CommObj(QObject *parent = nullptr);
    ~CommObj();
    static QList<QSerialPortInfo> getSerialPortInfo();
    static QList<QString> portName();
    static QList <qint32> baudRates();
    Q_INVOKABLE int portNameNum();
    Q_INVOKABLE int baudRatesNum();
    Q_INVOKABLE QString portNameValue(int pos);
    Q_INVOKABLE quint32 baudRatesValue(int pos);
    Q_INVOKABLE void setBaudRade(qint32 baud);
    Q_INVOKABLE void setDataBits(QSerialPort::DataBits dataBits);
    Q_INVOKABLE void setStopBits(QSerialPort::StopBits stopBits);
    Q_INVOKABLE void setPortName(const QString &portName);
    Q_INVOKABLE void setFlowContral(QSerialPort::FlowControl flowControl);
    Q_INVOKABLE void setParity(QSerialPort::Parity parity);

    void open();
    void close();
    bool isOpen();

signals:
    void send(const QByteArray &arry);
    void errorInfo(const QString &error);
    void serialSetting();
    void openSerialPort();
    void closeSerialPort();
public slots:
    void readData();
    void sendData(const QByteArray &array);

private :
    QSerialPort *m_serialPort;
    QThread *m_thread;
    qint32 m_baud;
    QSerialPort::DataBits m_dataBits;
    QSerialPort::StopBits m_stopBits;
    QString m_portName;
    QSerialPort::FlowControl m_flowControl;
    QSerialPort::Parity m_parity;
    bool m_isOpen;
};

#endif // COMMOBJ_H
