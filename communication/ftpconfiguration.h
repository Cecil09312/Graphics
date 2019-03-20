#ifndef FTPCONFIGURATION_H
#define FTPCONFIGURATION_H
#include "abstractconfiguration.h"

class FtpConfiguration : public AbstractConfiguration
{
public:
    FtpConfiguration();
     QVariant getConfiguration();
     void setConfiguration(const QVariant &configurationValue);
};

#endif // FTPCONFIGURATION_H
