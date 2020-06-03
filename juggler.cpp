#include "juggler.h"
#include <QDebug>

Juggler::Juggler(QEntity *aRootEntity,
                 QMesh *aSkeletonMesh,
                 QEffect *aEffect,
                 float &aRoty,
                 QVector2D &aPosition,
                 QColor &aColor)
  :skeletonMaterial(new QMaterial()),
    diffuseColorParameter(new QParameter()),
    shininessParameter(new QParameter()),
    skeletonTransform(new Qt3DCore::QTransform()),
    color(aColor)
{

  //skeleton Transform
  skeletonTransform->setScale(JUGGLER_SCALE);
  eulerAngles = QVector3D(JUGGLER_ROT_X, aRoty, JUGGLER_ROT_Z);
  skeletonTransform->setRotation(QQuaternion::fromEulerAngles(eulerAngles));
  m_position = QVector3D(aPosition.x(), JUGGLER_TRANSLATION_Y, aPosition.y());
  skeletonTransform->setTranslation(m_position);
  //skeletonMaterial
  diffuseColorParameter->setName(QLatin1String(DIFFUSE_COLOR));
  shininessParameter->setName(QLatin1String(SHININESS));
  skeletonMaterial->addParameter(diffuseColorParameter);
  diffuseColorParameter->setValue(QVariant::fromValue(color));
  skeletonMaterial->addParameter(shininessParameter);
  shininessParameter->setValue(QVariant::fromValue(JUGGLER_SHININESS));
  skeletonMaterial->setEffect(aEffect);

  QEntity::setParent(aRootEntity);
  addComponent(skeletonTransform);
  addComponent(aSkeletonMesh);
  addComponent(skeletonMaterial);
  setEnabled(enabled);

  // on met à jour les positions des mains
  setPositionHands();

  connect(this, SIGNAL(positionChanged()), this, SLOT(setPositionHands()));
}

void Juggler::setPosition(QVector3D aPosition)
{
  if (m_position == aPosition)
    return;

  m_position = aPosition;
  emit positionChanged();
  updateTransform();
}

void Juggler::updateTransform()
{
  skeletonTransform->setTranslation(m_position);
}

void Juggler::setPositionLHext()
{
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(HAND_OFFSET_EXT, QVector3D(0, 1, 0));
  posLHext = rot * posLHmed;
}

void Juggler::setPositionLHint()
{
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(HAND_OFFSET_INT, QVector3D(0, 1, 0));
  posLHint = rot * posLHmed;
}

void Juggler::setPositionLHmed()
{
  QVector3D vecOffset(HAND_OFFSET_X, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(eulerAngles.y(), QVector3D(0, 1, 0));
  vecMain = rot * vecMain;
  posLHmed = vecMain;
}

void Juggler::setPositionRHext()
{
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(-HAND_OFFSET_EXT, QVector3D(0, 1, 0));
  posRHext = rot * posRHmed;
}

void Juggler::setPositionRHint()
{
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(-HAND_OFFSET_INT, QVector3D(0, 1, 0));
  posRHint = rot * posRHmed;
}

void Juggler::setPositionRHmed()
{
  QVector3D vecOffset(-HAND_OFFSET_X, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(eulerAngles.y(), QVector3D(0, 1, 0));
  vecMain = rot * vecMain;
  posRHmed = vecMain;
}

void Juggler::setPositionHands()
{
  // on crée d'abord les meds, les autres n'en sont qu'une rotation
  setPositionLHmed();
  setPositionRHmed();

  setPositionLHext();
  setPositionLHint();
  setPositionRHext();
  setPositionRHint();
}
