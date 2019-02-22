#ifndef SERIALCONFIGURATION_H
#define SERIALCONFIGURATION_H
#include "abstractconfiguration.h"

class SerialConfiguration:public AbstractConfiguration//串口配置类
{
public:

    SerialConfiguration();
    ~SerialConfiguration();
    QVariant getConfiguration();
    void setConfiguration(const QVariant &configurationValue);
};

#endif // SERIALCONFIGURATION_H
