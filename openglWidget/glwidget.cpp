#include "glwidget.h"
#include <QDebug>
GLWidget::GLWidget(QWidget *parent):
    QOpenGLWidget(parent)
{
  //resize(560,480);

}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1, 0, 0, 1);
    //! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
   // glEnable(GL_SAMPLER);
    //! [2]
    //!
    qDebug() << "initialize";
   // paintGL();
}

void GLWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    m_projection.setToIdentity();

    // Set perspective projection
    m_projection.perspective(fov, aspect, zNear, zFar);
    qDebug() << "resize";
}

void GLWidget::paintGL()
{
    //glClear(GL_SAMPLER_BUFFER);
    glClear(GL_COLOR_BUFFER_BIT);
    qDebug() << "paint";


}
