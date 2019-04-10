#include "transportinfo.h"
#include "control/controller.h"
TransportInfo::TransportInfo(QObject *parent) : QObject(parent)
{

}

TransportInfo::~TransportInfo()
{

}

void TransportInfo::setTransportInfo(const QString &name, const QString &value)
{
    m_transportInfoHash[name] = Controller::instance()->fileNameFromQml(value);
}

void TransportInfo::saveTransportInfoToJson()
{
    QmlForJson qmlForJson;
    qmlForJson.writeFile(m_transportInfoHash,c_jsonFilePath);
}

QString TransportInfo::transportInfoFromJson(const QString &name)
{
    QmlForJson qmlForJson;
    m_transportInfoHash=  qmlForJson.readFile(c_jsonFilePath).toHash();
    return m_transportInfoHash[name].toString();
}

QString TransportInfo::transportInfo(const QString &name)
{
    return m_transportInfoHash[name].toString();
}
