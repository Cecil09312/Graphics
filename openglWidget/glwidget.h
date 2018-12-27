#ifndef GDelegateModelH
#define GLWIDGET_H

#include <QtOpenGL>
#include <QOpenGLFunctions_1_1>
#include <QMatrix4x4>
class GLWidget : public QGLWidget,public QOpenGLFunctions_1_1
{
    Q_OBJECT
public:
    GLWidget(const QGLFormat& format, QWidget* parent=nullptr,
             const QGLWidget* shareWidget = nullptr, Qt::WindowFlags f=Qt::WindowFlags());
protected:
//    void initializeGL();
//     void resizeGL(int w, int h);

//    void paintGL();
//private:
//    QMatrix4x4 m_projection;
};

#endif // GLWIDGET_H
