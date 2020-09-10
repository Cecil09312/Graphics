#ifndef SPEECHCOMCONFIGURATION_H
#define SPEECHCOMCONFIGURATION_H
#include "abstractconfiguration.h"

class SpeechComConfiguration : public AbstractConfiguration
{
public:
    SpeechComConfiguration();
    ~SpeechComConfiguration();
    QVariant getConfiguration();
     void setConfiguration(const QVariant &configurationValue);
};

#endif // INDICATORLIGHTCONFIGURATION_H
