#ifndef LOGMSG_H
#define LOGMSG_H

#include <QThread>
#include <QString>
#include <QSharedPointer>
#include "datapool.h"
#include <QCoreApplication>

//设置日志文件的大小
#define LOGFILE_SIZE	(5*1024*1024)

//日志文件的级别
enum LOG_LEVEL
{
	LOG_LEVEL_ERROR = 0,	//错误日志
	LOG_LEVEL_INFO = 1,		//运行日志
	LOG_LEVEL_DEBUG = 2 	//调试日志
};

class  LogMsg : public QObject
{
   Q_OBJECT
public:
    LogMsg(LogMsg*logMag =nullptr);
    virtual ~LogMsg();
    //输出日志
    virtual void writeLog(LOG_LEVEL LogLevel,const QString &strLog)=0;
    void writeData(const QString &strLog);
    void saveLogData();
signals:
    void sendData();
public slots:
    void writeToFile();
private:
   static void deleteLaterThread(QThread*thread);

private:
    LogMsg*m_logMsg;
    QSharedPointer<QThread>m_thread;
    DataPool m_dataPool;
    const QString c_fileName = QCoreApplication::applicationDirPath()+"/msgLog.log";
};

#endif
//#define WRITE_DEBUG_LOG(pLog)		LogMsg::writeLog(LOG_LEVEL_DEBUG, QObject::tr(__FUNCTION__), __LINE__, pLog)
