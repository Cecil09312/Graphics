#ifndef DEBUGLOGMSG_H
#define DEBUGLOGMSG_H
#include "logmsg.h"

class DebugLogMsg : public LogMsg
{
public:
    DebugLogMsg(LogMsg*logMsg);
    ~DebugLogMsg();
    void writeLog(LOG_LEVEL LogLevel, const QString &strLog);
private:
    LogMsg *m_logMsg;
};

#endif // DEBUGLOGMSG_H
