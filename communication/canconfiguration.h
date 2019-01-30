#ifndef CANCONFIGURATION_H
#define CANCONFIGURATION_H
#include "abstractconfiguration.h"

class CanConfiguration : public AbstractConfiguration
{
public:
    CanConfiguration();
    ~CanConfiguration();
    QVariant getConfiguration();
    void setConfiguration(const QVariant &configurationValue);
};

#endif // CANCONFIGURATION_H
