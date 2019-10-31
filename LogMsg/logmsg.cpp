#include "logmsg.h"
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#ifdef Q_OS_MAC
#include <mach-o/dyld.h>   
#endif

LogMsg::LogMsg(LogMsg*logMag)
{
    m_logMsg = logMag;
    m_thread = QSharedPointer<QThread>(new QThread,&LogMsg::deleteLaterThread);
    moveToThread(m_thread.data());
    m_thread.data()->start();
    connect(this,&LogMsg::sendData,this,&LogMsg::writeToFile);
}

/*
功能：析构函数，包括停止线程和释放内存
*/
LogMsg::~LogMsg()
{
    if(m_logMsg!=nullptr)
    {
        m_logMsg->deleteLater();
        m_logMsg = nullptr;
    }
    // m_thread.clear();

}

void LogMsg::writeData(const QString &strLog)
{
    QDateTime dataTime = QDateTime::currentDateTime();
    QString dataTimeStr = dataTime.toString("yyyy-MM-dd hh:mm:ss");
    QString qstrLog = dataTimeStr+QString(tr("[函数:%1,所在行:%2] %3")).arg(tr(__PRETTY_FUNCTION__)).arg(__LINE__).arg(strLog);
    QByteArray byteLog = qstrLog.toLocal8Bit();
    unsigned int freeSize= m_dataPool.getFreeSize();
    if(freeSize>0)
    {
        m_dataPool.putPool(byteLog.constData(),byteLog.size());
        m_dataPool.putPool("\r\n",2);
    }
}

void LogMsg::saveLogData()
{
    emit sendData();
}

void LogMsg::writeToFile()
{
    //如果日志文件名为空，返回
    if ( c_fileName.isEmpty() )
    {
        return;
    }
    unsigned int dataSize =m_dataPool.getDataSize();
    if(dataSize==0)
    {
        return;
    }
    QFile file(c_fileName);
    if ( file.open(QFile::WriteOnly | QFile::Append) )
    {
        file.write(QByteArray(m_dataPool.getData(dataSize), dataSize));
        m_dataPool.outPool(dataSize);
        file.flush();
        file.close();
        //如果写完后，日志文件大小大于配置的日志文件最大值，就将本次写入的日志文件重命名，这样下次写就写入了新的文件
        QFileInfo FileInfo(c_fileName);
        if(FileInfo.size() > LOGFILE_SIZE)
        {
            QDateTime CurDateTime = QDateTime::currentDateTime();
            QString strCurDataTime = CurDateTime.toString("yyyyMMddhhmmss");
            QString strBaseName = FileInfo.baseName();
            QString strFilePath = FileInfo.absolutePath();
            QString strExt = FileInfo.suffix();
            QString strNewFileName = QString("%1/%2_%3.%4").arg(strFilePath).arg(strBaseName).arg(strCurDataTime).arg(strExt);
            file.rename(strNewFileName);
        }

    }
}

void LogMsg::deleteLaterThread(QThread *thread)
{
    if(thread!=nullptr)
    {
        if(thread->isRunning())
        {
            thread->quit();
        }
        thread->deleteLater();
    }
}


