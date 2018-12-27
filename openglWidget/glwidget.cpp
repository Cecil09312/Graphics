#include "glwidget.h"
#include <QDebug>
GLWidget::GLWidget(const QGLFormat &format, QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f):
    QGLWidget(format,parent,shareWidget,f)
{
  //resize(560,480);

}

//void GLWidget::initializeGL()
//{
//    initializeOpenGLFunctions();
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//    glShadeModel(GL_FLAT);
//    qDebug() << "initialize";
//   // paintGL();
//}

//void GLWidget::resizeGL(int w, int h)
//{
//    // Calculate aspect ratio
//    glViewport(0, 0, (GLint)w, (GLint)h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glFrustum(-1.0, 1.0, -1.0, 1.0, 5.0, 15.0);
//    glMatrixMode(GL_MODELVIEW);
//}

//void GLWidget::paintGL()
//{
//    //glClear(GL_SAMPLER_BUFFER);
//   // glClear(GL_COLOR_BUFFER_BIT);
//    glClear(GL_COLOR_BUFFER_BIT);
//    glLoadIdentity();
//    qDebug() << "paint";


//}
