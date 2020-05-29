#include "light.h"

Light::Light(QEntity *aRootEntity,
             QVector3D &aPosition,
             QColor &aColor,
             float &aIntensity)
  :rootEntity(aRootEntity),
    position(aPosition),
    color(aColor),
    intensity(aIntensity)
{
  QEntity::setParent(rootEntity);
  light = new QPointLight(this);
  light->setColor(color);
  light->setIntensity(intensity);
  addComponent(light);
  lightTransform = new Qt3DCore::QTransform(this);
  lightTransform->setTranslation(position);
  addComponent(lightTransform);
  setEnabled(enabled);
}
