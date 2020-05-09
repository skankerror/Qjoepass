#include "mymainwindow.h"

#include <QApplication>
#include "mainGL.cpp"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MyMainWindow w;
  w.show();
  return a.exec();
}
