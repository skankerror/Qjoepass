#ifndef JUGGLINGRING_H
#define JUGGLINGRING_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include "qjoepass.h"

class JugglingRing: public Qt3DCore::QEntity
{
  Q_OBJECT
public:
  JugglingRing(Qt3DCore::QEntity *aRootEntity, QColor aColor);

private:
  Qt3DExtras::QTorusMesh *ringMesh;
  Qt3DCore::QTransform *ringTransform;
  Qt3DExtras::QDiffuseSpecularMaterial *ringMaterial;

  Qt3DCore::QEntity *rootEntity;
  QColor color;

  bool enabled = true;

};

#endif // JUGGLINGRING_H
