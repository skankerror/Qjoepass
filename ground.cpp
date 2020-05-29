#include "ground.h"

Ground::Ground(QEntity *aRootEntity, QColor &aColor)
  :rootEntity(aRootEntity),
    color(aColor)
{
  // Plane shape data
  planeMesh = new QPlaneMesh();
  planeMesh->setWidth(GROUND_WIDTH);
  planeMesh->setHeight(GROUND_HEIGHT);
  // Plane mesh transform
  planeTransform = new Qt3DCore::QTransform();
  planeTransform->setTranslation(QVector3D(0.0f, GROUND_POSY, 0.0f));
  // Plane Material
  planeMaterial = new QDiffuseSpecularMaterial();
  planeMaterial->setDiffuse(color);
  planeMaterial->setShininess(10);

  QEntity::setParent(rootEntity);
  addComponent(planeMesh);
  addComponent(planeMaterial);
  addComponent(planeTransform);
  setEnabled(enabled);

}
