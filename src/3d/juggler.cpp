/*
 * (c) 2020 Pat Co / M. C.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "juggler.h"
#include <QDebug>

Juggler::Juggler(QEntity *aRootEntity,
                 float &aRoty,
                 QVector2D &aPosition,
                 QColor &aColor)
  : jugglerMetalRoughMaterial(new QMetalRoughMaterial()),
    skeletonTransform(new Qt3DCore::QTransform()),
    color(aColor),
    rotY(aRoty)
{
  jugglerMetalRoughMaterial->setBaseColor(color);
  jugglerMetalRoughMaterial->setMetalness(JUGGLER_METALNESS);
  jugglerMetalRoughMaterial->setRoughness(JUGGLER_ROUGHNESS);

  eulerAngles = QVector3D(JUGGLER_ROT_X, rotY, JUGGLER_ROT_Z);

  mySkeleton = new QSkeleton();
  mySkeleton->setEnabled(true);
  rootJoint = new QJoint(this);
  rootJoint->setEnabled(true);
  mySkeleton->setRootJoint(rootJoint);
  rootJoint->setName("root_joint");
  myArmature = new QArmature();
  myArmature->setSkeleton(mySkeleton);
  myArmature->setEnabled(true);


  createHead();
  createShoulders();
  createArms();
  createElbows();
  createForearms();
  createTrunk();
  createThighs();
  createKnees();
  createLegs();

  m_position = QVector3D(aPosition.x(), JUGGLER_TRANSLATION_Y, aPosition.y());
  skeletonTransform->setTranslation(m_position);
  skeletonTransform->setScale(JUGGLER_SCALE);

  QEntity::setParent(aRootEntity);
  addComponent(skeletonTransform);
  //  material is not applied on children!
  //  addComponent(skeletonMaterial);

  // skeleton test
  addComponent(myArmature);
  setEnabled(enabled);

  qDebug() << myArmature->childNodes();
  qDebug() << mySkeleton->jointCount();
  qDebug() << rootJoint->name();
  qDebug() << headJoint->name();

  // we update hands positions and head
  setPositionHands();

  connect(this, SIGNAL(positionChanged()), this, SLOT(setPositionHands()));
}
void Juggler::setHandPosition(QVector3D pos)
{
  float rot = pos.x();
  bool hand = pos.z();
  QMatrix4x4 aMatrix;
  QVector3D elbowCurve;
  if (!hand) {
    elbowCurve = QVector3D(0, 1, 1);
    aMatrix = leftForearmMatrix;
  }
  else {
    elbowCurve = QVector3D(0, -1, -1);
    aMatrix = rightForearmMatrix;
  }
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(45, QVector3D(1, 0, 0));
  elbowCurve = rotAxis * elbowCurve;

  aMatrix.translate(0.0f, -0.74f, 1.06f);
  aMatrix.rotate(rot, elbowCurve);
  aMatrix *= rotAxis;
  aMatrix.translate(0.0f, 0.0f, -0.75f);

  hand ?
        leftForearmTransform->setMatrix(aMatrix):
        rightForearmTransform->setMatrix(aMatrix);
}

void Juggler::createHead()
{
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
  aHeadEntity->addComponent(jugglerMetalRoughMaterial);

  //skeleton test
  headJoint = new QJoint(aHeadEntity);
  headJoint->setEnabled(true);
  headJoint->setName("head_joint");
  headJoint->setInverseBindMatrix(headMatrix);
  rootJoint->addChildJoint(headJoint);
}

void Juggler::createShoulders()
{
  aShoulderEntity = new Qt3DCore::QEntity(this);
  Shoulders = new Qt3DExtras::QCylinderMesh();
  shouldersTransform = new Qt3DCore::QTransform();
  makeMember(Shoulders,shouldersTransform, aShoulderEntity,
             QVector3D(0.0f, 0.0f, 90.0f),
             QVector3D (0.0f, 6.3f, 0.0f),
             rotY, 2.0f);

  aLeftShoulderEntity = new Qt3DCore::QEntity(this);
  LeftShoulder = new Qt3DExtras::QSphereMesh();
  leftShoulderTransform = new Qt3DCore::QTransform();
  makeArticulation(LeftShoulder, leftShoulderTransform, aLeftShoulderEntity,
                   QVector3D (1.0f, 6.3f, 0.0f), rotY);

  aRightShoulderEntity = new Qt3DCore::QEntity(this);
  RightShoulder = new Qt3DExtras::QSphereMesh();
  rightShoulderTransform = new Qt3DCore::QTransform();
  makeArticulation(RightShoulder, rightShoulderTransform, aRightShoulderEntity,
                   QVector3D (-1.0f, 6.3f, 0.0f), rotY);
}

void Juggler::createArms()
{
  aLeftArmEntity = new Qt3DCore::QEntity(this);
  LeftArm = new Qt3DExtras::QCylinderMesh();
  leftArmTransform = new Qt3DCore::QTransform();
  makeMember(LeftArm,leftArmTransform, aLeftArmEntity,
             QVector3D(0.0f, 0.0f, 0.0f),
             QVector3D(1.0f, 5.55f, 0.0f), rotY, 1.5f);
  leftArmMatrix = leftArmTransform->matrix();

  aRightArmEntity = new Qt3DCore::QEntity(this);
  RightArm = new Qt3DExtras::QCylinderMesh();
  rightArmTransform = new Qt3DCore::QTransform();
  makeMember(RightArm, rightArmTransform, aRightArmEntity,
             QVector3D(0.0f, 0.0f, 0.0f),
             QVector3D (-1.0f, 5.55f, 0.0f), rotY, 1.5f);
  leftArmMatrix = leftArmTransform->matrix();
}

void Juggler::createElbows()
{
  aLeftElbowEntity = new Qt3DCore::QEntity(this);
  LeftElbow = new Qt3DExtras::QSphereMesh();
  leftElbowTransform = new Qt3DCore::QTransform();
  makeArticulation(LeftElbow, leftElbowTransform, aLeftElbowEntity,
                   QVector3D (1.0f, 4.8f, 0.0f), rotY);

  aRightElbowEntity = new Qt3DCore::QEntity(this);
  RightElbow = new Qt3DExtras::QSphereMesh();
  rightElbowTransform = new Qt3DCore::QTransform();
  makeArticulation(RightElbow, rightElbowTransform, aRightElbowEntity,
                   QVector3D (-1.0f, 4.8f, 0.0f), rotY);
}

void Juggler::createForearms()
{
  aLeftForearmEntity = new Qt3DCore::QEntity(this);
  LeftForearm = new Qt3DExtras::QCylinderMesh();
  leftForearmTransform = new Qt3DCore::QTransform();
  makeMember(LeftForearm,leftForearmTransform, aLeftForearmEntity,
             QVector3D(90.0f, 0.0f, 0.0f),
             QVector3D(1.0f, 4.8f, 0.75f), rotY, 1.5f);
  leftForearmMatrix = leftArmTransform->matrix();

  aRightForearmEntity = new Qt3DCore::QEntity(this);
  RightForearm = new Qt3DExtras::QCylinderMesh();
  rightForearmTransform = new Qt3DCore::QTransform();
  makeMember(RightForearm, rightForearmTransform, aRightForearmEntity,
             QVector3D(90.0f, 0.0f, 0.0f),
             QVector3D (-1.0f, 4.8f, 0.75f), rotY, 1.5f);
  rightForearmMatrix = rightArmTransform->matrix();
}

void Juggler::createTrunk()
{
  aTrunkEntity = new Qt3DCore::QEntity(this);
  Trunk = new Qt3DExtras::QCylinderMesh();
  trunkTransform = new Qt3DCore::QTransform();
  makeMember(Trunk, trunkTransform, aTrunkEntity,
             QVector3D(0.0f, 0.0f, 0.0f),
             QVector3D (0.0f, 5.3f, 0.0f), rotY, 2.5f);
}

void Juggler::createThighs()
{
  aLeftThighEntity = new Qt3DCore::QEntity(this);
  LeftThigh = new Qt3DExtras::QCylinderMesh();
  leftThighTransform = new Qt3DCore::QTransform();
  makeMember(LeftThigh,leftThighTransform, aLeftThighEntity,
             QVector3D(0.0f, 0.0f, 13.5f),
             QVector3D(0.25f, 3.05f, 0.0f), rotY, 2.2f);

  aRightThighEntity = new Qt3DCore::QEntity(this);
  RightThigh = new Qt3DExtras::QCylinderMesh();
  rightThighTransform = new Qt3DCore::QTransform();
  makeMember(RightThigh, rightThighTransform, aRightThighEntity,
             QVector3D(0.0f, 0.0f, -13.5f),
             QVector3D (-0.25f, 3.05f, 0.0f), rotY, 2.2f);
}

void Juggler::createKnees()
{
  aLeftKneeEntity = new Qt3DCore::QEntity(this);
  LeftKnee = new Qt3DExtras::QSphereMesh();
  leftKneeTransform = new Qt3DCore::QTransform();
  makeArticulation(LeftKnee, leftKneeTransform, aLeftKneeEntity,
                   QVector3D (0.5f, 2.0f, 0.0f), rotY);

  aRightKneeEntity = new Qt3DCore::QEntity(this);
  RightKnee = new Qt3DExtras::QSphereMesh();
  rightKneeTransform = new Qt3DCore::QTransform();
  makeArticulation(RightKnee, rightKneeTransform, aRightKneeEntity,
                   QVector3D (-0.5f, 2.0f, 0.0f), rotY);
}

void Juggler::createLegs()
{
  aLeftLegEntity = new Qt3DCore::QEntity(this);
  LeftLeg = new Qt3DExtras::QCylinderMesh();
  leftLegTransform = new Qt3DCore::QTransform();
  makeMember(LeftLeg,leftLegTransform, aLeftLegEntity,
             QVector3D(0.0f, 0.0f, 13.5f),
             QVector3D(0.75f, 1.0f, 0.0f), rotY, 2.0f);

  aRightLegEntity = new Qt3DCore::QEntity(this);
  RightLeg = new Qt3DExtras::QCylinderMesh();
  rightLegTransform = new Qt3DCore::QTransform();
  makeMember(RightLeg, rightLegTransform, aRightLegEntity,
             QVector3D(0.0f, 0.0f, -13.5f),
             QVector3D (-0.75f, 1.0f, 0.0f), rotY, 2.0f);
}

void Juggler::setLeftHandPosition(QVector2D rot)
{

  QVector3D elbowCurve = QVector3D(0, 1, 1);
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(90, QVector3D(1, 0, 0));
  elbowCurve = rotAxis * elbowCurve;

  QMatrix4x4 aMatrix = leftForearmMatrix;
  aMatrix.rotate(-45, QVector3D(1, 0, 0));
  aMatrix.translate(0.0f, -1.28f, 0.23f);
  aMatrix.rotate(rot.x(), elbowCurve);
  aMatrix.translate(0.0f, 0.0f, -0.75f);

  leftForearmTransform->setMatrix(aMatrix);


//    QVector3D shoulderCurve = QVector3D(0 , 1 , 1);
//    QMatrix4x4 shoulderAxis;
//    shoulderAxis.setToIdentity();
//    shoulderAxis.rotate(250, QVector3D(1, 0, 0));
//    shoulderCurve = shoulderAxis * shoulderCurve;
//    QMatrix4x4 armMatrix = leftArmMatrix;
//  //  armMatrix.rotate(90,0,0,1);
//    armMatrix.translate(0.0f, 0.75f, 0.0f);
//  //  armMatrix *= shoulderAxis;
//    armMatrix.rotate(rot.x(), shoulderCurve);
//    armMatrix.translate(0.0f, -0.75f, 0.0f);

//    leftArmTransform->setMatrix(armMatrix);
}

void Juggler::setRightHandPosition(QVector2D rot)
{

  QVector3D elbowCurve = QVector3D(0, -1, -1);
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(90, QVector3D(1, 0, 0));
  elbowCurve = rotAxis * elbowCurve;

  QMatrix4x4 aMatrix = rightForearmMatrix;
  aMatrix.rotate(-45, QVector3D(1, 0, 0));
  aMatrix.translate(0.0f, -1.28f, 0.23f);
  aMatrix.rotate(rot.x(), elbowCurve);
  //  aMatrix *= rotAxis;
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

QMatrix4x4 Juggler::getRotMatrix()
{
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.translate(m_position);
  rot.rotate(eulerAngles.y(), QVector3D(0, 1, 0));
  rot.translate(-m_position);
  return rot;
}

void Juggler::setPositionLHextPlus()
{
  QVector3D vecOffset(HAND_OFFSET_X + HAND_OFFSET_EXT_PLUS, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  posLHextPlus = vecMain;
}

void Juggler::setPositionLHext()
{
  QVector3D vecOffset(HAND_OFFSET_X + HAND_OFFSET_EXT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  posLHext = vecMain;
}

void Juggler::setPositionLHint()
{
  QVector3D vecOffset(HAND_OFFSET_X - HAND_OFFSET_INT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  posLHint = vecMain;
}

void Juggler::setPositionLHmed()
{
  QVector3D vecOffset(HAND_OFFSET_X, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  posLHmed = vecMain;
}

void Juggler::setPositionRHextPlus()
{
  QVector3D vecOffset(-HAND_OFFSET_X - HAND_OFFSET_EXT_PLUS, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  posRHextPlus = vecMain;
}

void Juggler::setPositionRHext()
{
  QVector3D vecOffset(-HAND_OFFSET_X - HAND_OFFSET_EXT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  posRHext = vecMain;
}

void Juggler::setPositionRHint()
{
  QVector3D vecOffset(-HAND_OFFSET_X + HAND_OFFSET_INT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  posRHint = vecMain;
}

void Juggler::setPositionRHmed()
{
  QVector3D vecOffset(-HAND_OFFSET_X, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  posRHmed = vecMain;
}

void Juggler::setPosHead()
{
  posHead = QVector3D(m_position.x(), HEAD_POS_Y, m_position.z());

  QVector3D temp = posHead + QVector3D(0, 0, 5);
  QMatrix4x4 rot = getRotMatrix();
  temp = rot * temp;
  headLookAt = temp;
}

void Juggler::setPositionHands()
{
  setPositionLHextPlus();
  setPositionLHext();
  setPositionLHmed();
  setPositionLHint();
  setPositionRHextPlus();
  setPositionRHext();
  setPositionRHmed();
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
  aMemberEntity->addComponent(jugglerMetalRoughMaterial);
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
  aSphereEntity->addComponent(jugglerMetalRoughMaterial);
}
