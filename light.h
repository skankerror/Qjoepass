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
                 QPointLight *aLight,
                 QVector3D &aPosition);

private:
  Qt3DCore::QTransform *lightTransform;

  QEntity *rootEntity;
  QPointLight *light;
  QVector3D position;

  bool enabled = true;
};

#endif // LIGHT_H
