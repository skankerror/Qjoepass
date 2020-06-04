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

  // we update hands positions
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
  QVector3D vecOffset(HAND_OFFSET_X + HAND_OFFSET_EXT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(eulerAngles.y(), QVector3D(0, 1, 0));
  vecMain = rot * vecMain;
  posLHext = vecMain;
}

void Juggler::setPositionLHint()
{
  QVector3D vecOffset(HAND_OFFSET_X - HAND_OFFSET_INT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(eulerAngles.y(), QVector3D(0, 1, 0));
  vecMain = rot * vecMain;
  posLHint = vecMain;
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
  QVector3D vecOffset(-HAND_OFFSET_X - HAND_OFFSET_EXT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(eulerAngles.y(), QVector3D(0, 1, 0));
  vecMain = rot * vecMain;
  posRHext = vecMain;
}

void Juggler::setPositionRHint()
{
  QVector3D vecOffset(-HAND_OFFSET_X + HAND_OFFSET_INT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(eulerAngles.y(), QVector3D(0, 1, 0));
  vecMain = rot * vecMain;
  posRHint = vecMain;
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
  setPositionLHmed();
  setPositionRHmed();
  setPositionLHext();
  setPositionLHint();
  setPositionRHext();
  setPositionRHint();
  qDebug() << posRHext << posRHmed << posRHint;
  qDebug() << posLHext << posLHmed << posLHint;
}
