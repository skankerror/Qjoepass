#ifndef PIROUETTE_H
#define PIROUETTE_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
//using namespace Qt3DAnimation;
using namespace Qt3DRender;

class Pirouette: public QEntity
{
  Q_OBJECT
public:
  Pirouette(QEntity *aRootEntity, QColor aColor);

  void setPosition(QVector3D aPosition);

private:
  QMesh *pirouetteMesh;
  QDiffuseSpecularMaterial *pirouetteMaterial;
  Qt3DCore::QTransform *pirouetteTransform;

  QEntity *rootEntity;
  QColor color;

  QVector3D position;

  bool enabled = true;
};

#endif // PIROUETTE_H
