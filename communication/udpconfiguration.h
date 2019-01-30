#ifndef UDPCONFIGURATION_H
#define UDPCONFIGURATION_H
#include "abstractconfiguration.h"

class UdpConfiguration : public AbstractConfiguration
{
public:
    UdpConfiguration();
    ~UdpConfiguration();
    QVariant getConfiguration();
    void setConfiguration(const QVariant &configurationValue);
};

#endif // UDPCONFIGURATION_H
