#include "jugglingball.h"

JugglingBall::JugglingBall(QEntity *aRootEntity,
                           QColor aColor/*,
                           QAnimationClipData *aclipData*/)
  :rootEntity(aRootEntity),
    color(aColor)/*,
    clipData(aclipData)*/
{
  // Sphere shape data
    QSphereMesh *sphereMesh = new QSphereMesh();
    sphereMesh->setRings(BALL_RINGS);
    sphereMesh->setSlices(BALL_SLICES);
    sphereMesh->setRadius(BALL_RADIUS);

    // Sphere mesh transform
    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform();

    sphereTransform->setScale(BALL_SCALE);
//    sphereTransform->setTranslation(QVector3D(-5.0f, -4.0f, 0.0f));

    sphereMaterial = new QDiffuseSpecularMaterial();
    sphereMaterial->setDiffuse(color);

    QEntity::setParent(rootEntity);
    addComponent(sphereMesh);
    addComponent(sphereMaterial);
    addComponent(sphereTransform);

    location = new QChannelMapping();
    location->setChannelName("Location");
    location->setTarget(sphereTransform);
    location->setProperty("translation");
    channelMapper = new QChannelMapper();
    channelMapper->addMapping(location);

    animationClip = new QAnimationClip();
//    animationClip->setClipData(*clipData);

    clipAnimator = new QClipAnimator(this);
    clipAnimator->setLoopCount(3);
    clipAnimator->setClip(animationClip);
    clipAnimator->setChannelMapper(channelMapper);

    // for testing
//    clipAnimator->setRunning(true);
}

void JugglingBall::updateAnim(QAnimationClipData clipData)
{
  animationClip->setClipData(clipData);
  clipAnimator->setRunning(true);
}
