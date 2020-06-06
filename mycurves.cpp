#include "mycurves.h"
#include "qjoepass.h"

MyCurves::MyCurves()
{

}

QVector<QVector3D> MyCurves::curveParabolic(QVector3D &velocity,
                                            QVector3D &startPos,
                                            int &frameCount)
{
  QVector<QVector3D> retVector;
  retVector.append(startPos);
  for (int i = 0; i <= frameCount; i++)
  {
    startPos += DELTA_TIME * velocity;
    retVector.append(startPos);
    velocity += DELTA_TIME * GRAVITY;
  }
  return retVector;
}
