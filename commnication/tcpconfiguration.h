#ifndef TCPCONFIGURATION_H
#define TCPCONFIGURATION_H
#include "abstractconfiguration.h"

class TcpConfiguration : public AbstractConfiguration
{
public:
    TcpConfiguration();
    ~TcpConfiguration();
    QVariant getConfiguration();
    void setConfiguration(const QVariant &configurationValue);
};

#endif // TCPCONFIGURATION_H
