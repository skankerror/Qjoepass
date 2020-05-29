#ifndef GROUND_H
#define GROUND_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;


class Ground: public QEntity
{
  Q_OBJECT
public:
  explicit Ground(QEntity *aRootEntity, QColor &aColor);

private:
  QPlaneMesh *planeMesh;
  Qt3DCore::QTransform *planeTransform;
  QDiffuseSpecularMaterial *planeMaterial;

  QEntity *rootEntity;
  QColor color;
  bool enabled = true;
};

#endif // GROUND_H
