#ifndef LIGHT_H
#define LIGHT_H

#include <Qt3DCore>
#include <Qt3DRender>
#include "qjoepass.h"


class Light: public Qt3DCore::QEntity
{
  Q_OBJECT
public:
  explicit Light(Qt3DCore::QEntity *aRootEntity,
                 QVector3D &aPosition,
                 QColor &aColor,
                 float &aIntensity);

private:
  Qt3DRender::QPointLight *light;
  Qt3DCore::QTransform *lightTransform;

  Qt3DCore::QEntity *rootEntity;
  QVector3D position;
  QColor color;
  float intensity;

  bool enabled = true;
};

#endif // LIGHT_H
