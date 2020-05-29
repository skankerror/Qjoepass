#ifndef JUGGLER_H
#define JUGGLER_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;

class Juggler: public QEntity
{
  Q_OBJECT
public:
  explicit Juggler(QEntity *aRootEntity,
                   float &aRoty,
                   QVector2D &aPosition,
                   QColor &aColor);

private:
  QSkeletonLoader *skeleton;
  QMesh *skeletonMesh;
  QDiffuseSpecularMaterial *skeletonMaterial;
  Qt3DCore::QTransform *skeletonTransform;
  QArmature *skeletonArmature;

  QEntity *rootEntity;
  QVector3D eulerAngles;
  QVector3D position;
  QColor color;

  bool enabled = true;
};

#endif // JUGGLER_H
