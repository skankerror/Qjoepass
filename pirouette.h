#ifndef PIROUETTE_H
#define PIROUETTE_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

class Pirouette: public Qt3DCore::QEntity
{
  Q_OBJECT
public:
  Pirouette(Qt3DCore::QEntity *aRootEntity, QColor aColor);

  void setPosition(QVector3D aPosition);

private:
  Qt3DRender::QMesh *pirouetteMesh;
  Qt3DExtras::QDiffuseSpecularMaterial *pirouetteMaterial;
  Qt3DCore::QTransform *pirouetteTransform;

  Qt3DCore::QEntity *rootEntity;
  QColor color;

  QVector3D position;

  bool enabled = true;
};

#endif // PIROUETTE_H
