#ifndef JUGGLINGBALL_H
#define JUGGLINGBALL_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include "qjoepass.h"

class JugglingBall: public Qt3DCore::QEntity
{
  Q_OBJECT
public:
  JugglingBall(Qt3DCore::QEntity *aRootEntity, QColor aColor);

private:
  Qt3DExtras::QSphereMesh *sphereMesh;
  Qt3DCore::QTransform *sphereTransform;
  Qt3DExtras::QPhongMaterial *sphereMaterial;

  Qt3DCore::QEntity *rootEntity;
  QColor color;

  bool enabled = true;
};

#endif // JUGGLINGBALL_H
