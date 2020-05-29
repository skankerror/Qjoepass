#include "juggler.h"


Juggler::Juggler(QEntity *aRootEntity,
                 QSkeletonLoader *aSkeleton,
                 QMesh *aSkeletonMesh,
                 QEffect *aEffect,
                 float &aRoty,
                 QVector2D &aPosition,
                 QColor &aColor)
  :skeletonMaterial(new QMaterial()),
    diffuseColorParameter(new QParameter()),
    shininessParameter(new QParameter()),
    skeletonTransform(new Qt3DCore::QTransform()),
    skeletonArmature(new QArmature()),
    color(aColor)
{
  //skeleton Transform
  skeletonTransform->setScale(JUGGLER_SCALE);
  eulerAngles = QVector3D(JUGGLER_ROT_X, aRoty, JUGGLER_ROT_Z);
  skeletonTransform->setRotation(QQuaternion::fromEulerAngles(eulerAngles));
  position = QVector3D(aPosition.x(), JUGGLER_TRANSLATION_Y, aPosition.y());
  skeletonTransform->setTranslation(position);
  //skeletonArmature
  skeletonArmature->setSkeleton(aSkeleton);
  //skeletonMaterial
  diffuseColorParameter->setName(QLatin1String("kd"));
  shininessParameter->setName(QLatin1String("shininess"));
  skeletonMaterial->addParameter(diffuseColorParameter);
  diffuseColorParameter->setValue(QVariant::fromValue(color));
  skeletonMaterial->addParameter(shininessParameter);
  shininessParameter->setValue(QVariant::fromValue(JUGGLER_SHININESS));
  skeletonMaterial->setEffect(aEffect);

  QEntity::setParent(aRootEntity);
  addComponent(skeletonTransform);
  addComponent(aSkeletonMesh);
  addComponent(skeletonArmature);
  addComponent(skeletonMaterial);
  setEnabled(enabled);

}
