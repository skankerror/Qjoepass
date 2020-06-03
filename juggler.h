#ifndef JUGGLER_H
#define JUGGLER_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>
#include "qjoepass.h"
//#include "animationfactory.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;
using namespace Qt3DAnimation;

class Juggler: public QEntity
{
  Q_OBJECT
  Q_PROPERTY(Qt3DCore::QTransform *skeletonTransform
             READ getSkeletonTransform
             WRITE setSkeletonTransform
             NOTIFY skeletonTransformChanged)
  Q_PROPERTY(QVector3D position
             READ position
             WRITE setPosition
             NOTIFY positionChanged)
public:
  explicit Juggler(QEntity *aRootEntity,
                   QMesh *aSkeletonMesh,
                   QEffect *aEffect,
                   float &aRoty,
                   QVector2D &aPosition,
                   QColor &aColor);

  QVector3D position() const {return m_position;}
  Qt3DCore::QTransform * getSkeletonTransform() const {return skeletonTransform;};
  void setPosition(QVector3D aPosition);
  void setSkeletonTransform(Qt3DCore::QTransform *aTransform) {skeletonTransform = aTransform;};

  // getters for hands positions ext to catch, int to launch, med for siteswap 2
  QVector3D getPositionLHext() const {return posLHext;};
  QVector3D getPositionLHint() const {return posLHint;};
  QVector3D getPositionLHmed() const {return posLHmed;};
  QVector3D getPositionRHext() const {return posRHext;};
  QVector3D getPositionRHint() const {return posRHint;};
  QVector3D getPositionRHmed() const {return posRHmed;};

  float getRotY() const {return eulerAngles.y();};

private:
  void updateTransform();

  // setters for hands pos
  void setPositionLHext();
  void setPositionLHint();
  void setPositionLHmed();
  void setPositionRHext();
  void setPositionRHint();
  void setPositionRHmed();

private slots:
  void setPositionHands();

signals:
  void skeletonTransformChanged();
  void positionChanged();

private:
  QMaterial *skeletonMaterial;
  QParameter *diffuseColorParameter;
  QParameter *shininessParameter;
  Qt3DCore::QTransform *skeletonTransform;

  QVector3D eulerAngles;
  QVector3D m_position;
  QColor color;

  bool enabled = true;

  // handPos
  QVector3D posLHext;
  QVector3D posLHint;
  QVector3D posLHmed;
  QVector3D posRHext;
  QVector3D posRHint;
  QVector3D posRHmed;

};

#endif // JUGGLER_H
