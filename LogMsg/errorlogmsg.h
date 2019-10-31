#ifndef ERRORLOGMSG_H
#define ERRORLOGMSG_H
#include "logmsg.h"

class ErrorLogMsg : public LogMsg
{
public:
    ErrorLogMsg(LogMsg*logMsg);
    ~ErrorLogMsg();
    void writeLog(LOG_LEVEL LogLevel, const QString &strLog);
private:
    LogMsg *m_logMsg;
};

#endif // ERRORLOGMSG_H
