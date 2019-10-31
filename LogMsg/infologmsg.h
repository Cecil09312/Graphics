#ifndef INFOLOGMSG_H
#define INFOLOGMSG_H
#include "logmsg.h"

class InfoLogMsg : public LogMsg
{
public:
    InfoLogMsg(LogMsg *logMsg);
    ~InfoLogMsg();

    void writeLog(LOG_LEVEL LogLevel, const QString &strLog);
private:
    LogMsg *m_logMsg;
};

#endif // INFOLOGMSG_H
