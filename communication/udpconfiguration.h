#ifndef UDPCONFIGURATION_H
#define UDPCONFIGURATION_H
#include "abstractconfiguration.h"
#include "jsonEdit/qmlforjson.h"
class UdpConfiguration : public AbstractConfiguration//UDP配置类
{
public:
    UdpConfiguration();
    ~UdpConfiguration();
    QVariant getConfiguration();
    void setConfiguration(const QVariant &configurationValue);

};

#endif // UDPCONFIGURATION_H
