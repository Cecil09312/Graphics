#include "drawimagethread.h"
#include <QSvgRenderer>
#include <QImage>
#include <QPainter>
#include <QDebug>

DrawImageThread::DrawImageThread(QObject *parent):
    QObject(parent)

{
 m_thread = new QThread();
 this->moveToThread(m_thread);

 connect(this,&DrawImageThread::startDrawImage,this,[=](const QString&name)
 {
     if(!name.endsWith(".svg"))
     {
         if(!m_imageHash.contains(name))
         {
             QImage image(name);
             m_imageHash[name] = image;
             emit drawCurrentImage();
         }
     }

 });


}

DrawImageThread::~DrawImageThread()
{
 if(m_thread!=nullptr)
 {
     m_thread->quit();
     m_thread->deleteLater();
 }
}

void DrawImageThread::drawImage(const QString&name)
{
   // qDebug() << "m_thread->isRunning()" << m_thread->isRunning();
    if(!m_thread->isRunning())
    {
        m_thread->start();
    }
    emit startDrawImage(name);
}

QImage DrawImageThread::getImageFromName(const QString &name)
{
    return m_imageHash.value(name);
}



