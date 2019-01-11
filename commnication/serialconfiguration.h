#ifndef SERIALCONFIGURATION_H
#define SERIALCONFIGURATION_H
#include "abstractconfiguration.h"

class SerialConfiguration:public AbstractConfiguration
{
public:

    SerialConfiguration();
    ~SerialConfiguration();
    QVariant getConfiguration();
    void setConfiguration(const QVariant &configurationValue);
};

#endif // SERIALCONFIGURATION_H
