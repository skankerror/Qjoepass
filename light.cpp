#include "light.h"

Light::Light(Qt3DCore::QEntity *aRootEntity,
             QVector3D &aPosition,
             QColor &aColor,
             float &aIntensity)
  :rootEntity(aRootEntity),
    position(aPosition),
    color(aColor),
    intensity(aIntensity)
{
  Qt3DCore::QEntity::setParent(rootEntity);
  light = new Qt3DRender::QPointLight(this);
  light->setColor(color);
  light->setIntensity(intensity);
  addComponent(light);
  lightTransform = new Qt3DCore::QTransform(this);
  lightTransform->setTranslation(position);
  addComponent(lightTransform);
  setEnabled(enabled);
}
