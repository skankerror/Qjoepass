#ifndef ANIMATIONVIEW_H
#define ANIMATIONVIEW_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QBasicTimer>

class animationView : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

public:
  animationView();
  ~animationView();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void cleanup();


private:
    QBasicTimer timer;
    QMatrix4x4 projection;



};

#endif // ANIMATIONVIEW_H
