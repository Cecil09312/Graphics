#ifndef DRAWIMAGETHREAD_H
#define DRAWIMAGETHREAD_H
#include <QThread>
#include <QRectF>
#include <QImage>
#include <QPainter>
class DrawImageThread : public QThread
{
    Q_OBJECT
public:
    DrawImageThread(QObject*parent = nullptr);
    ~DrawImageThread();
    void drawImage();
    void setImageName(const QString &name);
    void setImageRect(const QRectF& rectF);
signals:
    void startDrawImages();
    void currentImage(QImage image);
private:
    QString m_imageName;
    QRectF m_rectF;

};

#endif // DRAWIMAGETHREAD_H
