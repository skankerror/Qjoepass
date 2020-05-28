#include "juggler.h"


Juggler::Juggler(Qt3DCore::QEntity *aRootEntity,
                 float &aRoty,
                 QVector2D &aPosition,
                 QColor &aColor)
  :rootEntity(aRootEntity),
    color(aColor)
{
  //skeleton Transform
  skeletonTransform = new Qt3DCore::QTransform();
  skeletonTransform->setScale(JUGGLER_SCALE);
  eulerAngles = QVector3D(JUGGLER_ROT_X, aRoty, JUGGLER_ROT_Z);
  skeletonTransform->setRotation(QQuaternion::fromEulerAngles(eulerAngles));
  position = QVector3D(aPosition.x(), JUGGLER_TRANSLATION_Y, aPosition.y());
  skeletonTransform->setTranslation(position);
  //skeletonMesh
  skeletonMesh = new Qt3DRender::QMesh();
  skeletonMesh->setSource(QUrl(SKELETON_MESH_SRC));
  //skeleton
  skeleton = new Qt3DCore::QSkeletonLoader();
  skeleton->setSource(QUrl(SKELETON_SRC));
  //skeletonArmature
  skeletonArmature = new Qt3DCore::QArmature();
  skeletonArmature->setSkeleton(skeleton);
  //skeletonMaterial
  skeletonMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
  skeletonMaterial->setDiffuse(color);
  Qt3DCore::QEntity::setParent(rootEntity);
  addComponent(skeletonTransform);
  addComponent(skeletonMesh);
  addComponent(skeletonArmature);
  addComponent(skeletonMaterial);
  setEnabled(enabled);

}
