#ifndef JUGGLINGBALL_H
#define JUGGLINGBALL_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DAnimation>
#include "qjoepass.h"

class JugglingBall: public Qt3DCore::QEntity
{
  Q_OBJECT
public:
  JugglingBall(Qt3DCore::QEntity *aRootEntity,
               QColor aColor/*,
               Qt3DAnimation::QAnimationClipData *aclipData*/);

public slots:
  void updateAnim(Qt3DAnimation::QAnimationClipData clipData);

private:
  Qt3DExtras::QSphereMesh *sphereMesh;
  Qt3DCore::QTransform *sphereTransform;
  Qt3DExtras::QDiffuseSpecularMaterial *sphereMaterial;

  Qt3DAnimation::QClipAnimator *clipAnimator;
  Qt3DAnimation::QAnimationClip *animationClip;
  Qt3DAnimation::QChannelMapper *channelMapper;
  Qt3DAnimation::QChannelMapping *location;

  Qt3DCore::QEntity *rootEntity;
  QColor color;
  Qt3DAnimation::QAnimationClipData clipData;


  bool enabled = true;
};

#endif // JUGGLINGBALL_H
