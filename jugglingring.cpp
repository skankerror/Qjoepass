#include "jugglingring.h"


JugglingRing::JugglingRing(QEntity *aRootEntity,
                           QTorusMesh *torusMesh,
                           QEffect *aEffect,
                           QColor aColor)
  :ringMaterial(new QMaterial()),
    diffuseColorParameter(new QParameter()),
    shininessParameter(new QParameter()),
    ringTransform(new Qt3DCore::QTransform()),
    color(aColor)

{
  ringTransform->setScale3D(QVector3D(RING_SCALE_X, RING_SCALE_Y, RING_SCALE_Z));

  diffuseColorParameter->setName(QLatin1String(DIFFUSE_COLOR));
  shininessParameter->setName(QLatin1String(SHININESS));
  ringMaterial->addParameter(diffuseColorParameter);
  diffuseColorParameter->setValue(QVariant::fromValue(color));
  ringMaterial->addParameter(shininessParameter);
  shininessParameter->setValue(QVariant::fromValue(RING_SHININESS));
  ringMaterial->setEffect(aEffect);

  QEntity::setParent(aRootEntity);
  addComponent(torusMesh);
  addComponent(ringTransform);
  addComponent(ringMaterial);
  setEnabled(enabled);

  //animation test
//  QAnimationClip animClip;
//  QAnimationClipData clipData;
//  QChannelMapper channelMapper;
//  QChannelMapping channelMapping;

//  QClipAnimator clipAnimator;
//  clipAnimator.setClip(&animClip);
//  animClip.setClipData(clipData);
//  addComponent(&clipAnimator);


}
