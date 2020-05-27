#include "pirouette.h"

Pirouette::Pirouette(Qt3DCore::QEntity *aRootEntity, QColor aColor)
  :rootEntity(aRootEntity),
    color(aColor)
{
  pirouetteTransform = new Qt3DCore::QTransform();
  pirouetteTransform->setScale(0.1);
  pirouetteMesh = new Qt3DRender::QMesh();
  pirouetteMesh->setSource(QUrl("qrc:/models/club.qgltf"));
  pirouetteMaterial = new Qt3DExtras::QPhongMaterial();
  pirouetteMaterial->setDiffuse(color);
  Qt3DCore::QEntity::setParent(rootEntity);
  addComponent(pirouetteTransform);
  addComponent(pirouetteMesh);
  addComponent(pirouetteMaterial);
  setEnabled(enabled);
}
