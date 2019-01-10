#ifndef SERIALCONFIGURATION_H
#define SERIALCONFIGURATION_H
#include "abstractconfiguration.h"

class SerialConfiguration:public AbstractConfiguration
{
public:

    SerialConfiguration();
    ~SerialConfiguration();
    QList<QVariant> getConfiguration();
    void setConfiguration(const QList<QVariant>&configurationList);
};

#endif // SERIALCONFIGURATION_H
