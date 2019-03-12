#ifndef TCPCONFIGURATION_H
#define TCPCONFIGURATION_H
#include "abstractconfiguration.h"
#include "jsonEdit/qmlforjson.h"
class TcpConfiguration : public AbstractConfiguration//TCP配置类
{
public:
    TcpConfiguration();
    ~TcpConfiguration();
    QVariant getConfiguration();
    void setConfiguration(const QVariant &configurationValue);

};

#endif // TCPCONFIGURATION_H
