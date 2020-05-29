#ifndef JUGGLINGBALL_H
#define JUGGLINGBALL_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;
using namespace Qt3DAnimation;

class JugglingBall: public QEntity
{
  Q_OBJECT
public:
  JugglingBall(QEntity *aRootEntity,
               QSphereMesh *aSphereMesh,
               QEffect *aEffect,
               QColor &aColor);

public slots:

private:
  QMaterial *sphereMaterial;
  QParameter *diffuseColorParameter;
  QParameter *shininessParameter;
  Qt3DCore::QTransform *sphereTransform;

  QColor color;

  bool enabled = true;
};

#endif // JUGGLINGBALL_H
