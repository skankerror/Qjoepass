#include "jugglingball.h"

JugglingBall::JugglingBall(QEntity *aRootEntity,
                           QSphereMesh *aSphereMesh,
                           QEffect *aEffect,
                           QColor &aColor)
  :sphereMaterial(new QMaterial()),
    diffuseColorParameter(new QParameter()),
    shininessParameter(new QParameter()),
    sphereTransform(new Qt3DCore::QTransform()),
    color(aColor)
{
    sphereTransform->setScale(BALL_SCALE);

    diffuseColorParameter->setName(QLatin1String("kd"));
    shininessParameter->setName(QLatin1String("shininess"));
    sphereMaterial->addParameter(diffuseColorParameter);
    diffuseColorParameter->setValue(QVariant::fromValue(color));
    sphereMaterial->addParameter(shininessParameter);
    shininessParameter->setValue(QVariant::fromValue(BALL_SHININESS));
    sphereMaterial->setEffect(aEffect);

    QEntity::setParent(aRootEntity);
    addComponent(aSphereMesh);
    addComponent(sphereMaterial);
    addComponent(sphereTransform);

}
