#ifndef GROUND_H
#define GROUND_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;


class Ground: public QEntity
{
  Q_OBJECT
public:
  explicit Ground(QEntity *aRootEntity,
                  QEffect *aEffect,
                  QColor &aColor);

  QColor getColor() const {return color;};
  void setColor(QColor aColor);

private:
  QPlaneMesh *planeMesh;
  Qt3DCore::QTransform *planeTransform;

  QMaterial *planeMaterial;
  QParameter *diffuseColorParameter;
  QParameter *shininessParameter;

  QColor color;
  bool enabled = true;

  QTextureMaterial *groundTextureMaterial;
  QTextureLoader *groundTextureLoader;
};

#endif // GROUND_H
