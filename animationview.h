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
  animationView(QWidget *parent = nullptr);
  ~animationView();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;


private:
    QBasicTimer timer;
    QMatrix4x4 projection;



};

#endif // ANIMATIONVIEW_H
