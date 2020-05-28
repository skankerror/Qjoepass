#include "ground.h"

Ground::Ground(Qt3DCore::QEntity *aRootEntity, QColor &aColor)
  :rootEntity(aRootEntity),
    color(aColor)
{
  // Plane shape data
  planeMesh = new Qt3DExtras::QPlaneMesh();
  planeMesh->setWidth(GROUND_WIDTH);
  planeMesh->setHeight(GROUND_HEIGHT);
  // Plane mesh transform
  planeTransform = new Qt3DCore::QTransform();
  planeTransform->setTranslation(QVector3D(0.0f, GROUND_POSY, 0.0f));
  // Plane Material
  planeMaterial = new Qt3DExtras::QPhongMaterial();
  planeMaterial->setDiffuse(color);
  planeMaterial->setShininess(10);

  Qt3DCore::QEntity::setParent(rootEntity);
  addComponent(planeMesh);
  addComponent(planeMaterial);
  addComponent(planeTransform);
  setEnabled(enabled);

}
