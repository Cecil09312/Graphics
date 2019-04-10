#ifndef INDICATORLIGHTCONFIGURATION_H
#define INDICATORLIGHTCONFIGURATION_H
#include "abstractconfiguration.h"

class IndicatorLightConfiguration : public AbstractConfiguration
{
public:
    IndicatorLightConfiguration();
    ~IndicatorLightConfiguration();
    QVariant getConfiguration();
     void setConfiguration(const QVariant &configurationValue);
};

#endif // INDICATORLIGHTCONFIGURATION_H
