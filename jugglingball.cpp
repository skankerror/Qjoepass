#include "jugglingball.h"

JugglingBall::JugglingBall(Qt3DCore::QEntity *aRootEntity,
                           QColor aColor/*,
                           Qt3DAnimation::QAnimationClipData *aclipData*/)
  :rootEntity(aRootEntity),
    color(aColor)/*,
    clipData(aclipData)*/
{
  // Sphere shape data
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRings(BALL_RINGS);
    sphereMesh->setSlices(BALL_SLICES);
    sphereMesh->setRadius(BALL_RADIUS);

    // Sphere mesh transform
    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform();

    sphereTransform->setScale(BALL_SCALE);
//    sphereTransform->setTranslation(QVector3D(-5.0f, -4.0f, 0.0f));

    sphereMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
    sphereMaterial->setDiffuse(color);

    Qt3DCore::QEntity::setParent(rootEntity);
    addComponent(sphereMesh);
    addComponent(sphereMaterial);
    addComponent(sphereTransform);

    location = new Qt3DAnimation::QChannelMapping();
    location->setChannelName("Location");
    location->setTarget(sphereTransform);
    location->setProperty("translation");
    channelMapper = new Qt3DAnimation::QChannelMapper();
    channelMapper->addMapping(location);

    animationClip = new Qt3DAnimation::QAnimationClip();
//    animationClip->setClipData(*clipData);

    clipAnimator = new Qt3DAnimation::QClipAnimator(this);
    clipAnimator->setLoopCount(3);
    clipAnimator->setClip(animationClip);
    clipAnimator->setChannelMapper(channelMapper);

    // for testing
//    clipAnimator->setRunning(true);
}

void JugglingBall::updateAnim(Qt3DAnimation::QAnimationClipData clipData)
{
  animationClip->setClipData(clipData);
  clipAnimator->setRunning(true);
}
