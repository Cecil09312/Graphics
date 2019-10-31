#include "infologmsg.h"

InfoLogMsg::InfoLogMsg(LogMsg *logMsg)
{
    m_logMsg = logMsg;
}

InfoLogMsg::~InfoLogMsg()
{
    if(m_logMsg!=nullptr)
    {
        m_logMsg->deleteLater();
        m_logMsg = nullptr;
    }
}

void InfoLogMsg::writeLog(LOG_LEVEL LogLevel, const QString &strLog)
{
    if(LogLevel==LOG_LEVEL_INFO)
    {
        writeData("Info:"+strLog);
    }
    else
    {
        if(m_logMsg!=nullptr)
        {
            m_logMsg->writeLog(LogLevel,strLog);
        }
    }
}
