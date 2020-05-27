#include "juggler.h"


Juggler::Juggler(Qt3DCore::QEntity *aRootEntity,
                 float aRoty,
                 QVector2D aPosition,
                 QColor aColor)
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
  //  skeletonMesh->setSource(QUrl::fromLocalFile(SKELETON_MODEL));
  skeletonMesh->setSource(QUrl(SKELETON_MESH_SRC));
  //skeleton
  skeleton = new Qt3DCore::QSkeletonLoader();
  //  skeleton->setSource(QUrl::fromLocalFile(SKELETON_MODEL));
  skeleton->setSource(QUrl(SKELETON_SRC));
  //skeletonArmature
  skeletonArmature = new Qt3DCore::QArmature();
  skeletonArmature->setSkeleton(skeleton);
  //skeletonMaterial
  skeletonMaterial = new Qt3DExtras::QPhongMaterial();
  skeletonMaterial->setDiffuse(color);
  //skeletonEntity
  skeletonEntity = new Qt3DCore::QEntity(rootEntity);
  skeletonEntity->addComponent(skeletonTransform);
  skeletonEntity->addComponent(skeletonMesh);
  skeletonEntity->addComponent(skeletonArmature);
  skeletonEntity->addComponent(skeletonMaterial);
  skeletonEntity->setEnabled(enabled);

}
