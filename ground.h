#ifndef GROUND_H
#define GROUND_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include "qjoepass.h"

class Ground: public Qt3DCore::QEntity
{
  Q_OBJECT
public:
  explicit Ground(Qt3DCore::QEntity *aRootEntity, QColor &aColor);

private:
  Qt3DExtras::QPlaneMesh *planeMesh;
  Qt3DCore::QTransform *planeTransform;
  Qt3DExtras::QPhongMaterial *planeMaterial;

  Qt3DCore::QEntity *rootEntity;
  QColor color;
  bool enabled = true;
};

#endif // GROUND_H
