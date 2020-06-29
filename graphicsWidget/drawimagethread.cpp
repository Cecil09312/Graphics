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

 //qDebug() << "mainThread" << QThread::currentThread();

 connect(this,&DrawImageThread::startDrawImage,this,[=](const QString&name)
 {


     if(!m_imageHash.contains(name))
     {
         QImage image(name);
         int imageSize = image.size().width();
        // qDebug() << image.byteCount();
         QImage curImage;
         if(imageSize>36)
         {
              curImage = image.scaled(36,36);
         }
         else
         {
             curImage = image.scaled(imageSize,imageSize);
         }

         m_imageHash[name] = curImage;
         emit drawCurrentImage();
         m_thread->msleep(5);
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



