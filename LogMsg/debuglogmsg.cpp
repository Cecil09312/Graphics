#include "debuglogmsg.h"

DebugLogMsg::DebugLogMsg(LogMsg *logMsg)
{
    m_logMsg = logMsg;
}

DebugLogMsg::~DebugLogMsg()
{
    if(m_logMsg!=nullptr)
    {
        m_logMsg->deleteLater();
        m_logMsg = nullptr;
    }
}

void DebugLogMsg::writeLog(LOG_LEVEL LogLevel, const QString &strLog)
{
    if(LogLevel==LOG_LEVEL_DEBUG)
    {
        writeData("Debug:"+strLog);
    }
    else
    {
        if(m_logMsg!=nullptr)
        {
            m_logMsg->writeLog(LogLevel,strLog);
        }
    }
}
