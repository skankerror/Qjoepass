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
                   QSkeletonLoader *aSkeleton,
                   QMesh *aSkeletonMesh,
                   QEffect *aEffect,
                   float &aRoty,
                   QVector2D &aPosition,
                   QColor &aColor);

  QVector3D position() const {return m_position;}

private:
  QMaterial *skeletonMaterial;
  QParameter *diffuseColorParameter;
  QParameter *shininessParameter;
  Qt3DCore::QTransform *skeletonTransform;
  QArmature *skeletonArmature;

  QVector3D eulerAngles;
  QVector3D m_position;
  QColor color;

  bool enabled = true;
};

#endif // JUGGLER_H
