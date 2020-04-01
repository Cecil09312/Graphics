#include "drawimagethread.h"
#include <QSvgRenderer>
#include <QImage>
#include <QPainter>
DrawImageThread::DrawImageThread(QObject *parent):
    QThread(parent)

{
   // m_painter.setPen(QPen(Qt::blue, 10));
     // 设置画刷
  // m_painter.setBrush(Qt::red);
    connect(this,&DrawImageThread::startDrawImages,this,[=]()
    {
        if(m_imageName.isEmpty())
            return;

        QImage image(m_imageName);
        QPainter painter;
        painter.begin(&image);

        if(m_imageName.endsWith(".svg"))
        {
            QSvgRenderer render;
            render.load(m_imageName);

            //m_renderer.load(m_svgName);
            render.render(&painter,m_rectF);
        }
        else
        {
            painter.drawPixmap(m_rectF.toRect(),QPixmap(m_imageName));
        }

        painter.end();
        emit currentImage(image);
    });
}

DrawImageThread::~DrawImageThread()
{

}

void DrawImageThread::drawImage()
{
    emit startDrawImages();
}

void DrawImageThread::setImageName(const QString &name)
{
    m_imageName = name;
}

void DrawImageThread::setImageRect(const QRectF &rectF)
{
    m_rectF = rectF;
}
