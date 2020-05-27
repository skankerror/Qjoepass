#include "pirouette.h"

Pirouette::Pirouette(Qt3DCore::QEntity *aRootEntity, QColor aColor)
  :rootEntity(aRootEntity),
    color(aColor)
{
  pirouetteTransform = new Qt3DCore::QTransform();
  pirouetteTransform->setScale(CLUB_SCALE);
  pirouetteMesh = new Qt3DRender::QMesh();
  pirouetteMesh->setSource(QUrl(CLUB_MESH_SRC));
  pirouetteMaterial = new Qt3DExtras::QPhongMaterial();
  pirouetteMaterial->setDiffuse(color);
  Qt3DCore::QEntity::setParent(rootEntity);
  addComponent(pirouetteTransform);
  addComponent(pirouetteMesh);
  addComponent(pirouetteMaterial);
  setEnabled(enabled);
}

void Pirouette::setPosition(QVector3D aPosition)
{
  position = aPosition;
  pirouetteTransform->setTranslation(position); // A voir...
}
