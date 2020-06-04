#include "light.h"

Light::Light(QEntity *aRootEntity,
             QPointLight *aLight,
             QVector3D &aPosition)
  :rootEntity(aRootEntity),
    light(aLight),
    position(aPosition)
{
  QEntity::setParent(rootEntity);
  addComponent(light);
  lightTransform = new Qt3DCore::QTransform(this);
  lightTransform->setTranslation(position);
  addComponent(lightTransform);
  setEnabled(enabled);
}
