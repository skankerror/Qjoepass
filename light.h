#ifndef LIGHT_H
#define LIGHT_H

#include <Qt3DCore>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DRender;


class Light: public QEntity
{
  Q_OBJECT
public:
  explicit Light(QEntity *aRootEntity,
                 QVector3D &aPosition,
                 QColor &aColor,
                 float &aIntensity);

private:
  QPointLight *light;
  Qt3DCore::QTransform *lightTransform;

  QEntity *rootEntity;
  QVector3D position;
  QColor color;
  float intensity;

  bool enabled = true;
};

#endif // LIGHT_H
