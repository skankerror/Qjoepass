#ifndef GROUND_H
#define GROUND_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;


class Ground: public QEntity
{
  Q_OBJECT
public:
  explicit Ground(QEntity *aRootEntity,
                  QEffect *aEffect,
                  QColor &aColor);

private:
  QPlaneMesh *planeMesh;
  Qt3DCore::QTransform *planeTransform;

  QMaterial *planeMaterial;
  QParameter *diffuseColorParameter;
  QParameter *shininessParameter;

  QColor color;
  bool enabled = true;
};

#endif // GROUND_H
