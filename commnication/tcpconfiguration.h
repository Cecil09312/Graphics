#ifndef TCPCONFIGURATION_H
#define TCPCONFIGURATION_H
#include "abstractconfiguration.h"

class TcpConfiguration : public AbstractConfiguration
{
public:
    TcpConfiguration();
    ~TcpConfiguration();
    QList<QVariant> getConfiguration();
    void setConfiguration(const QList<QVariant>&configurationList);
};

#endif // TCPCONFIGURATION_H
