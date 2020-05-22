#include "animationview.h"

animationView::animationView(QWidget *parent)
    : QOpenGLWidget(parent)
  {

  }

animationView::~animationView()
{
    makeCurrent();
//    delete texture;
//    delete geometries;
    doneCurrent();
}

void animationView::initializeGL()
{
    initializeOpenGLFunctions();

//    initPrefs();
    glClearColor(0, 0, 1, 1);


    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

//    geometries = new GeometryEngine;

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}
void animationView::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
void animationView::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}
