#ifndef JUGGLER_H
#define JUGGLER_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>
#include "qjoepass.h"
#include "animationfactory.h"

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
  Q_PROPERTY(QJoint *rootJoint
             READ getRootJoint
             WRITE setRootJoint
             NOTIFY rootJointChanged)
  Q_PROPERTY(QSkeletonLoader *skeleton
             READ getSkeleton
             WRITE setSkeleton
             NOTIFY skeletonChanged)
  Q_PROPERTY(QVector3D position
             READ position
             WRITE setPosition
             NOTIFY positionChanged)
public:
  explicit Juggler(QEntity *aRootEntity,
                   QSkeletonLoader *aSkeleton,
                   QMesh *aSkeletonMesh,
                   QEffect *aEffect,
                   float &aRoty,
                   QVector2D &aPosition,
                   QColor &aColor);

  QVector3D position() const {return m_position;}
  Qt3DCore::QTransform * getSkeletonTransform() const {return skeletonTransform;};
  QJoint * getRootJoint() const {return rootJoint;};
  QSkeletonLoader * getSkeleton() const {return skeleton;};
  void setPosition(QVector3D aPosition);
  void setSkeletonTransform(Qt3DCore::QTransform *aTransform) {skeletonTransform = aTransform;};
  void setRootJoint(QJoint * aJoint) {rootJoint = aJoint;};
  void setSkeleton(QSkeletonLoader * aSkeleton) {skeleton = aSkeleton;};

private:
  void updateTransform();

signals:
  void skeletonTransformChanged();
  void rootJointChanged();
  void skeletonChanged();
  void positionChanged(QVector3D position);

private:
  QSkeletonLoader *skeleton;
  QMaterial *skeletonMaterial;
  QParameter *diffuseColorParameter;
  QParameter *shininessParameter;
  Qt3DCore::QTransform *skeletonTransform;
  QArmature *skeletonArmature;

  // pour tester
//  QClipAnimator *animator;
//  QAnimationClip *clip;
//  AnimationFactory *factory;
//  QChannelMapper *channelMapper;
//  QChannelMapping *positionMapping;

  QVector3D eulerAngles;
  QVector3D m_position;
  QColor color;

  QJoint *rootJoint;

  bool enabled = true;
};

#endif // JUGGLER_H
