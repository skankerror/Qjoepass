#ifndef JUGGLER_H
#define JUGGLER_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>

#define JUGGLER_SCALE 3.0f
#define JUGGLER_ROT_X 0
#define JUGGLER_ROT_Z 0
#define JUGGLER_TRANSLATION_Y -4
#define SKELETON_MESH_SRC "qrc:/models/juggler.qgltf"
#define SKELETON_SRC "qrc:/models/RiggedFigure.gltf"

class Juggler: public Qt3DCore::QEntity
{
  Q_OBJECT
public:
  explicit Juggler(Qt3DCore::QEntity *aRootEntity,
                   float aRoty,
                   QVector2D aPosition,
                   QColor aColor);

private:
  Qt3DCore::QEntity *skeletonEntity;
  Qt3DCore::QSkeletonLoader *skeleton;
  Qt3DRender::QMesh *skeletonMesh;
  Qt3DExtras::QPhongMaterial *skeletonMaterial;
  Qt3DCore::QTransform *skeletonTransform;
  Qt3DCore::QArmature *skeletonArmature;

  Qt3DCore::QEntity *rootEntity;
  QVector3D eulerAngles;
  QVector3D position;
  QColor color;

  bool enabled = true;
};

#endif // JUGGLER_H
