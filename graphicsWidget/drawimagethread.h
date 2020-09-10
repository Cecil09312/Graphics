#ifndef DRAWIMAGETHREAD_H
#define DRAWIMAGETHREAD_H
#include <QThread>
#include <QRectF>
#include <QImage>
#include <QPainter>
#include <QtConcurrent>
#include <QSvgRenderer>

class DrawImageThread : public QObject
{
    Q_OBJECT
public:
    DrawImageThread(QObject*parent = nullptr);
    ~DrawImageThread();
    void drawImage(const QString &name);
    QImage getImageFromName(const QString &name);
signals:
     void startDrawImage(const QString&name);
     void drawCurrentImage();
private:
     QThread *m_thread;
     QHash<QString,QImage>m_imageHash;



};

#endif // DRAWIMAGETHREAD_H
