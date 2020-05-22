#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QBasicTimer>

class MyMainWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

public:
  MyMainWindow(QWidget *parent = nullptr);
  ~MyMainWindow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QBasicTimer timer;
    QMatrix4x4 projection;
};
#endif // MYMAINWINDOW_H
