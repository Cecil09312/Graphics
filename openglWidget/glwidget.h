#ifndef GDelegateModelH
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
class GLWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
public:
    GLWidget(QWidget *parent = nullptr);
protected:
    void initializeGL();
    void resizeGL(int w,int h);
    void paintGL();
private:
    QMatrix4x4 m_projection;
};

#endif // GLWIDGET_H
