#ifndef JUGGLINGRING_H
#define JUGGLINGRING_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;


class JugglingRing: public QEntity
{
  Q_OBJECT
public:
  JugglingRing(QEntity *aRootEntity,
               QTorusMesh *torusMesh,
               QEffect *aEffect,
               QColor aColor);

private:
  QMaterial *ringMaterial;
  QParameter *diffuseColorParameter;
  QParameter *shininessParameter;
  Qt3DCore::QTransform *ringTransform;

  QColor color;

  bool enabled = true;

};

#endif // JUGGLINGRING_H
