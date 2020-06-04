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

    diffuseColorParameter->setName(QLatin1String(DIFFUSE_COLOR));
    shininessParameter->setName(QLatin1String(SHININESS));
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

void JugglingBall::setPosition(QVector3D position)
{
  if (m_position == position)
    return;

  m_position = position;
  emit positionChanged(position);
  updateTransForm();
}

void JugglingBall::updateTransForm()
{
  sphereTransform->setTranslation(m_position);
}
