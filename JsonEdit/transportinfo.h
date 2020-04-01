#ifndef TRANSPORTINFO_H
#define TRANSPORTINFO_H

#include <QObject>
#include "qmlforjson.h"

class TransportInfo : public QObject
{
    Q_OBJECT
public:
    explicit TransportInfo(QObject *parent = nullptr);
    ~TransportInfo();
    Q_INVOKABLE void setTransportInfo(const QString&name,const QString&value);
    Q_INVOKABLE void setControlCenterRight(bool value);
    Q_INVOKABLE void saveTransportInfoToJson();
    Q_INVOKABLE QVariant transportInfoFromJson(const QString&name);
   // Q_INVOKABLE QString transportInfoStr(const QString&name);
    Q_INVOKABLE QString transportInfo(const QString &name);
    Q_INVOKABLE bool controlCentrolEnable();

signals:

public slots:
private:
    QHash<QString,QVariant>m_transportInfoHash;
    const QString c_jsonFilePath =QCoreApplication::applicationDirPath()+"/transportInfo.json";
};

#endif // TRANSPORTINFO_H
