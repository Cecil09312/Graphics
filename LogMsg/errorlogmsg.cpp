#include "errorlogmsg.h"

ErrorLogMsg::ErrorLogMsg(LogMsg *logMsg)
{
    m_logMsg = logMsg;
}

ErrorLogMsg::~ErrorLogMsg()
{
    if(m_logMsg!=nullptr)
    {
        m_logMsg->deleteLater();
        m_logMsg = nullptr;
    }
}

void ErrorLogMsg::writeLog(LOG_LEVEL LogLevel, const QString &strLog)
{
    if(LogLevel==LOG_LEVEL_ERROR)
    {
       writeData("Error:"+strLog);
    }
    else
    {
        if(m_logMsg!=nullptr)
        {
            m_logMsg->writeLog(LogLevel,strLog);
        }
    }
}
