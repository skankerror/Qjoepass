#ifndef JUGGLINGBALL_H
#define JUGGLINGBALL_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DAnimation>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DAnimation;

class JugglingBall: public QEntity
{
  Q_OBJECT
public:
  JugglingBall(QEntity *aRootEntity,
               QColor aColor/*,
               Qt3DAnimation::QAnimationClipData *aclipData*/);

public slots:
  void updateAnim(QAnimationClipData clipData);

private:
  QSphereMesh *sphereMesh;
  Qt3DCore::QTransform *sphereTransform;
  QDiffuseSpecularMaterial *sphereMaterial;

  QClipAnimator *clipAnimator;
  QAnimationClip *animationClip;
  QChannelMapper *channelMapper;
  QChannelMapping *location;

  QEntity *rootEntity;
  QColor color;
  QAnimationClipData clipData;


  bool enabled = true;
};

#endif // JUGGLINGBALL_H
