#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>

class MyMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MyMainWindow(QWidget *parent = nullptr);
  ~MyMainWindow();
};
#endif // MYMAINWINDOW_H
