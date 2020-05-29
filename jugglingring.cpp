#include "jugglingring.h"


JugglingRing::JugglingRing(QEntity *aRootEntity, QColor aColor)
  :rootEntity(aRootEntity),
    color(aColor)
{
  ringMesh = new QTorusMesh();
  ringMesh->setRadius(RING_RADIUS);
  ringMesh->setMinorRadius(RING_MINOR_RADIUS);
  ringMesh->setRings(RING_RING_NUMBER);
  ringMesh->setSlices(RING_SLICE_NUMBER);

  ringTransform = new Qt3DCore::QTransform();
  ringTransform->setScale3D(QVector3D(RING_SCALE_X, RING_SCALE_Y, RING_SCALE_Z));
//  ringTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 25.0f));
//  ringTransform->setTranslation(QVector3D(5.0f, 4.0f, 0.0f));

  ringMaterial = new QDiffuseSpecularMaterial();
  ringMaterial->setDiffuse(color);

  QEntity::setParent(rootEntity);
  addComponent(ringMesh);
  addComponent(ringTransform);
  addComponent(ringMaterial);
  setEnabled(enabled);
}
