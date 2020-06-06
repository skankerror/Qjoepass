#include "juggler.h"
#include <QDebug>

Juggler::Juggler(QEntity *aRootEntity,
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

  eulerAngles = QVector3D(JUGGLER_ROT_X, aRoty, JUGGLER_ROT_Z);
  diffuseColorParameter->setName(QLatin1String(DIFFUSE_COLOR));
  shininessParameter->setName(QLatin1String(SHININESS));
  skeletonMaterial->addParameter(diffuseColorParameter);
  diffuseColorParameter->setValue(QVariant::fromValue(color));
  skeletonMaterial->addParameter(shininessParameter);
  shininessParameter->setValue(QVariant::fromValue(JUGGLER_SHININESS));
  skeletonMaterial->setEffect(aEffect);


  aHeadEntity = new Qt3DCore::QEntity(this);
  Head = new Qt3DExtras::QSphereMesh();
  headTransform = new Qt3DCore::QTransform();
  Head->setRadius(0.5f);
  Head->setRings(32);
  Head->setSlices(32);

  QMatrix4x4 headMatrix = headTransform->matrix();
  headMatrix.translate(QVector3D(0.0f, 7.0f, 0.0f));
  headTransform->setMatrix(headMatrix);

  aHeadEntity->addComponent(Head);
  aHeadEntity->addComponent(headTransform);
  aHeadEntity->addComponent(skeletonMaterial);

  aShoulderEntity = new Qt3DCore::QEntity(this);
  Shoulders = new Qt3DExtras::QCylinderMesh();
  shouldersTransform = new Qt3DCore::QTransform();
  makeMember(Shoulders,shouldersTransform, aShoulderEntity,
             QVector3D(0.0f, 0.0f, 90.0f),
             QVector3D (0.0f, 6.3f, 0.0f),
             aRoty, 2.0f);

  aLeftShoulderEntity = new Qt3DCore::QEntity(this);
  LeftShoulder = new Qt3DExtras::QSphereMesh();
  leftShoulderTransform = new Qt3DCore::QTransform();
  makeArticulation(LeftShoulder, leftShoulderTransform, aLeftShoulderEntity,
             QVector3D (-1.0f, 6.3f, 0.0f), aRoty);

  aRightShoulderEntity = new Qt3DCore::QEntity(this);
  RightShoulder = new Qt3DExtras::QSphereMesh();
  rightShoulderTransform = new Qt3DCore::QTransform();
  makeArticulation(RightShoulder, rightShoulderTransform, aRightShoulderEntity,
             QVector3D (1.0f, 6.3f, 0.0f), aRoty);

  aLeftArmEntity = new Qt3DCore::QEntity(this);
  LeftArm = new Qt3DExtras::QCylinderMesh();
  leftArmTransform = new Qt3DCore::QTransform();
  makeMember(LeftArm,leftArmTransform, aLeftArmEntity,
             QVector3D(0.0f, 0.0f, 0.0f),
             QVector3D(-1.0f, 5.55f, 0.0f), aRoty, 1.5f);

  aRightArmEntity = new Qt3DCore::QEntity(this);
  RightArm = new Qt3DExtras::QCylinderMesh();
  rightArmTransform = new Qt3DCore::QTransform();
  makeMember(RightArm, rightArmTransform, aRightArmEntity,
             QVector3D(0.0f, 0.0f, 0.0f),
             QVector3D (1.0f, 5.55f, 0.0f), aRoty, 1.5f);

  aLeftElbowEntity = new Qt3DCore::QEntity(this);
  LeftElbow = new Qt3DExtras::QSphereMesh();
  leftElbowTransform = new Qt3DCore::QTransform();
  makeArticulation(LeftElbow, leftElbowTransform, aLeftElbowEntity,
             QVector3D (-1.0f, 4.8f, 0.0f), aRoty);

  aRightElbowEntity = new Qt3DCore::QEntity(this);
  RightElbow = new Qt3DExtras::QSphereMesh();
  rightElbowTransform = new Qt3DCore::QTransform();
  makeArticulation(RightElbow, rightElbowTransform, aRightElbowEntity,
             QVector3D (1.0f, 4.8f, 0.0f), aRoty);

  aLeftForearmEntity = new Qt3DCore::QEntity(this);
  LeftForearm = new Qt3DExtras::QCylinderMesh();
  leftForearmTransform = new Qt3DCore::QTransform();
  makeMember(LeftForearm,leftForearmTransform, aLeftForearmEntity,
             QVector3D(90.0f, 0.0f, 0.0f),
             QVector3D(-1.0f, 4.8f, 0.75f), aRoty, 1.5f);
  leftForearmMatrix = leftArmTransform->matrix();

  aRightForearmEntity = new Qt3DCore::QEntity(this);
  RightForearm = new Qt3DExtras::QCylinderMesh();
  rightForearmTransform = new Qt3DCore::QTransform();
  makeMember(RightForearm, rightForearmTransform, aRightForearmEntity,
             QVector3D(90.0f, 0.0f, 0.0f),
             QVector3D (1.0f, 4.8f, 0.75f), aRoty, 1.5f);
  rightForearmMatrix = rightArmTransform->matrix();

  aTrunkEntity = new Qt3DCore::QEntity(this);
  Trunk = new Qt3DExtras::QCylinderMesh();
  trunkTransform = new Qt3DCore::QTransform();
  makeMember(Trunk, trunkTransform, aTrunkEntity,
             QVector3D(0.0f, 0.0f, 0.0f),
             QVector3D (0.0f, 5.3f, 0.0f), aRoty, 2.5f);

  aLeftThighEntity = new Qt3DCore::QEntity(this);
  LeftThigh = new Qt3DExtras::QCylinderMesh();
  leftThighTransform = new Qt3DCore::QTransform();
  makeMember(LeftThigh,leftThighTransform, aLeftThighEntity,
             QVector3D(0.0f, 0.0f, -13.5f),
             QVector3D(-0.25f, 3.05f, 0.0f), aRoty, 2.2f);

  aRightThighEntity = new Qt3DCore::QEntity(this);
  RightThigh = new Qt3DExtras::QCylinderMesh();
  rightThighTransform = new Qt3DCore::QTransform();
  makeMember(RightThigh, rightThighTransform, aRightThighEntity,
             QVector3D(0.0f, 0.0f, 13.5f),
             QVector3D (0.25f, 3.05f, 0.0f), aRoty, 2.2f);

  aLeftKneeEntity = new Qt3DCore::QEntity(this);
  LeftKnee = new Qt3DExtras::QSphereMesh();
  leftKneeTransform = new Qt3DCore::QTransform();
  makeArticulation(LeftKnee, leftKneeTransform, aLeftKneeEntity,
             QVector3D (-0.5f, 2.0f, 0.0f), aRoty);

  aRightKneeEntity = new Qt3DCore::QEntity(this);
  RightKnee = new Qt3DExtras::QSphereMesh();
  rightKneeTransform = new Qt3DCore::QTransform();
  makeArticulation(RightKnee, rightKneeTransform, aRightKneeEntity,
             QVector3D (0.5f, 2.0f, 0.0f), aRoty);

  aLeftLegEntity = new Qt3DCore::QEntity(this);
  LeftLeg = new Qt3DExtras::QCylinderMesh();
  leftLegTransform = new Qt3DCore::QTransform();
  makeMember(LeftLeg,leftLegTransform, aLeftLegEntity,
             QVector3D(0.0f, 0.0f, -13.5f),
             QVector3D(-0.75f, 1.0f, 0.0f), aRoty, 2.0f);

  aRightLegEntity = new Qt3DCore::QEntity(this);
  RightLeg = new Qt3DExtras::QCylinderMesh();
  rightLegTransform = new Qt3DCore::QTransform();
  makeMember(RightLeg, rightLegTransform, aRightLegEntity,
             QVector3D(0.0f, 0.0f, 13.5f),
             QVector3D (0.75f, 1.0f, 0.0f), aRoty, 2.0f);

  m_position = QVector3D(aPosition.x(), JUGGLER_TRANSLATION_Y, aPosition.y());
  skeletonTransform->setTranslation(m_position);
  skeletonTransform->setScale(JUGGLER_SCALE);

  QEntity::setParent(aRootEntity);
  addComponent(skeletonTransform);
//  material is not applied on children!
  addComponent(skeletonMaterial);
  setEnabled(enabled);

  // we update hands positions
  setPositionHands();

  connect(this, SIGNAL(positionChanged()), this, SLOT(setPositionHands()));
}
void Juggler::setLeftForearmPosition(float rot)
{
  QMatrix4x4 aMatrix = leftForearmMatrix;
  aMatrix.translate(0.0f, 0.0f, 0.75f);
  aMatrix.rotate(rot, QVector3D(0.0f, 0.5f, 0.5f));
  aMatrix.translate(0.0f, 0.0f, -0.75f);
  leftForearmTransform->setMatrix(aMatrix);
}

void Juggler::setRightForearmPosition(float rot)
{
  QMatrix4x4 aMatrix = rightForearmMatrix;
  aMatrix.translate(0.0f, 0.0f, 0.75f);
  aMatrix.rotate(rot, QVector3D(0.0f, 0.5f, 0.5f));
  aMatrix.translate(0.0f, 0.0f, -0.75f);
  rightForearmTransform->setMatrix(aMatrix);

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

void Juggler::setPosHead()
{
  posHead = QVector3D(m_position.x(), HEAD_POS_Y, m_position.z());
}

void Juggler::setPositionHands()
{
  setPositionLHmed();
  setPositionRHmed();
  setPositionLHext();
  setPositionLHint();
  setPositionRHext();
  setPositionRHint();
  setPosHead();
}

void Juggler::makeMember(QCylinderMesh *aMember,
                Qt3DCore::QTransform *aMemberTransform,
                QEntity *aMemberEntity,
                QVector3D aRot,
                QVector3D aTrans,
                float globalRotation,
                float length)
{

  aMemberEntity->addComponent(aMember);

  aMember->setRadius(0.1f);
  aMember->setRings(8);
  aMember->setSlices(32);
  aMember->setLength(length);

  QMatrix4x4 aMatrix = aMemberTransform->matrix();
  aMatrix.rotate(globalRotation, QVector3D(0.0f, 1.0f, 0.0f));
  aMatrix.translate(aTrans);
  aMatrix.rotate(QQuaternion::fromEulerAngles(aRot));
  aMemberTransform->setMatrix(aMatrix);

//  aMemberTransform->setRotation(QQuaternion::fromEulerAngles(aRot));


  aMemberEntity->addComponent(aMemberTransform);
  aMemberEntity->addComponent(skeletonMaterial);
}

void Juggler::makeArticulation(QSphereMesh *aSphere,
                   Qt3DCore::QTransform *aSphereTransform,
                   QEntity *aSphereEntity,
                   QVector3D aTrans,
                   float globalRotation)
{
//  aSphereTransform->setScale(JUGGLER_SCALE);
  aSphere->setRadius(0.1f);
  aSphere->setRings(8);
  aSphere->setSlices(32);

  QMatrix4x4 aMatrix = aSphereTransform->matrix();
  aMatrix.rotate(globalRotation, QVector3D(0.0f, 1.0f, 0.0f));
  aMatrix.translate(aTrans);
  aSphereTransform->setMatrix(aMatrix);

  aSphereEntity->addComponent(aSphere);
  aSphereEntity->addComponent(aSphereTransform);
  aSphereEntity->addComponent(skeletonMaterial);
}
