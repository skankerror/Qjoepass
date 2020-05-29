#include "ground.h"

Ground::Ground(QEntity *aRootEntity, QEffect *aEffect, QColor &aColor)
  :planeMesh(new QPlaneMesh()),
    planeTransform(new Qt3DCore::QTransform()),
    planeMaterial(new QMaterial()),
    diffuseColorParameter(new QParameter()),
    shininessParameter(new QParameter()),
    color(aColor)
{
  // Plane shape data
  planeMesh->setWidth(GROUND_WIDTH);
  planeMesh->setHeight(GROUND_HEIGHT);
  // Plane mesh transform
  planeTransform->setTranslation(QVector3D(0.0f, GROUND_POSY, 0.0f));
  // Plane Material
  diffuseColorParameter->setName(QLatin1String("kd"));
  shininessParameter->setName(QLatin1String("shininess"));
  planeMaterial->addParameter(diffuseColorParameter);
  diffuseColorParameter->setValue(QVariant::fromValue(color));
  planeMaterial->addParameter(shininessParameter);
  shininessParameter->setValue(QVariant::fromValue(GROUND_SHININESS));
  planeMaterial->setEffect(aEffect);

  QEntity::setParent(aRootEntity);
  addComponent(planeMesh);
  addComponent(planeMaterial);
  addComponent(planeTransform);
  setEnabled(enabled);

}
