#include "pirouette.h"

Pirouette::Pirouette(QEntity *aRootEntity,
                     QMesh *aPirouetteMesh,
                     QEffect *aEffect,
                     QColor aColor)
  :pirouetteMaterial(new QMaterial()),
    diffuseColorParameter(new QParameter()),
    shininessParameter(new QParameter()),
    pirouetteTransform(new Qt3DCore::QTransform()),
    color(aColor)
{
  pirouetteTransform->setScale(CLUB_SCALE);

  diffuseColorParameter->setName(QLatin1String(DIFFUSE_COLOR));
  shininessParameter->setName(QLatin1String(SHININESS));
  pirouetteMaterial->addParameter(diffuseColorParameter);
  diffuseColorParameter->setValue(QVariant::fromValue(color));
  pirouetteMaterial->addParameter(shininessParameter);
  shininessParameter->setValue(QVariant::fromValue(CLUB_SHININESS));
  pirouetteMaterial->setEffect(aEffect);


  QEntity::setParent(aRootEntity);
  addComponent(pirouetteTransform);
  addComponent(aPirouetteMesh);
  addComponent(pirouetteMaterial);
  setEnabled(enabled);
}

void Pirouette::setPosition(QVector3D aPosition)
{
  position = aPosition;
  pirouetteTransform->setTranslation(position); // A voir...
}
