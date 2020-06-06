#ifndef MYCURVES_H
#define MYCURVES_H

#include <QVector>
#include <QVector3D>

class MyCurves
{
public:
  MyCurves();

  static QVector<QVector3D> curveParabolic(QVector3D &velocity,
                                           QVector3D &startPos,
                                           int &frameCount);

};

#endif // MYCURVES_H
