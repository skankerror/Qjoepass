#ifndef JUGGLINGRING_H
#define JUGGLINGRING_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
//using namespace Qt3DAnimation;


class JugglingRing: public QEntity
{
  Q_OBJECT
public:
  JugglingRing(QEntity *aRootEntity, QColor aColor);

private:
  QTorusMesh *ringMesh;
  Qt3DCore::QTransform *ringTransform;
  QDiffuseSpecularMaterial *ringMaterial;

  QEntity *rootEntity;
  QColor color;

  bool enabled = true;

};

#endif // JUGGLINGRING_H
