#ifndef CANCONFIGURATION_H
#define CANCONFIGURATION_H
#include "abstractconfiguration.h"
#include "jsonEdit/qmlforjson.h"
class CanConfiguration : public AbstractConfiguration//CAN总线配置类
{
public:
    CanConfiguration();
    ~CanConfiguration();
    QVariant getConfiguration();
    void setConfiguration(const QVariant &configurationValue);

};

#endif // CANCONFIGURATION_H
