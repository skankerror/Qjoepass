#ifndef JUGGLINGBALL_H
#define JUGGLINGBALL_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;
using namespace Qt3DAnimation;

class JugglingBall: public QEntity
{
  Q_OBJECT
  Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)

public:
  JugglingBall(QEntity *aRootEntity,
               QSphereMesh *aSphereMesh,
               QEffect *aEffect,
               QColor &aColor);

  QVector3D position() const {return m_position;};

public slots:
  void setPosition(QVector3D position);

signals:
  void positionChanged(QVector3D position);

private:
  void updateTransForm();

private:
  QMaterial *sphereMaterial;
  QParameter *diffuseColorParameter;
  QParameter *shininessParameter;
  Qt3DCore::QTransform *sphereTransform;

  QVector3D m_position;

  QColor color;

  bool enabled = true;
};

#endif // JUGGLINGBALL_H
