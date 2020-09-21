#include <QCoreApplication>
#include "logmsg.h"
#include "debuglogmsg.h"
#include "infologmsg.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    LogMsg *logMsg = new DebugLogMsg(nullptr);
    LogMsg *infoLogMsg = new InfoLogMsg(logMsg);

    infoLogMsg->saveLogData();
  infoLogMsg->deleteLater();
    //logMsg->deleteLater();

    return a.exec();
}
