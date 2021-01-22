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

Juggler::Juggler(QEntity *t_rootEntity,
                 float &t_rotY,
                 QVector2D &t_position,
                 QColor &t_color)
  : m_jugglerMetalRoughMaterial(new QMetalRoughMaterial()),
    m_skeletonTransform(new Qt3DCore::QTransform()),
    m_color(t_color),
    m_rotY(t_rotY)
{
  m_jugglerMetalRoughMaterial->setBaseColor(m_color);
  m_jugglerMetalRoughMaterial->setMetalness(JUGGLER_METALNESS);
  m_jugglerMetalRoughMaterial->setRoughness(JUGGLER_ROUGHNESS);

  createHead();
  createShoulders();
  createArms();
  createElbows();
  createForearms();
  createTrunk();
  createThighs();
  createKnees();
  createLegs();

  QMatrix4x4 aMatrix = m_skeletonTransform->matrix();
  aMatrix.rotate(m_rotY, QVector3D(0, 1, 0));
  m_skeletonTransform->setMatrix(aMatrix);

  m_position = QVector3D(t_position.x(), JUGGLER_TRANSLATION_Y, t_position.y());
  m_skeletonTransform->setTranslation(m_position);
  m_skeletonTransform->setScale(JUGGLER_SCALE);

  QEntity::setParent(t_rootEntity);
  addComponent(m_skeletonTransform);

  // we update hands positions and head
  setPositionHands();

  connect(this, SIGNAL(positionChanged()), this, SLOT(setPositionHands()));

  /************************* testing zone ********************************/

  setHandPosition(m_posLHextPlus, hand(leftHand));
  setHandPosition(m_posRHextPlus, hand(rightHand));

}

void Juggler::createHead()
{
  m_headEntity = new Qt3DCore::QEntity(this);
  m_head = new Qt3DExtras::QSphereMesh();
  m_headTransform = new Qt3DCore::QTransform();
  m_head->setRadius(HEAD_RADIUS);
  m_head->setRings(HEAD_RINGS);
  m_head->setSlices(HEAD_SLICES);

  QMatrix4x4 headMatrix = m_headTransform->matrix();
  headMatrix.translate(HEAD_TRANSLATE);
  m_headTransform->setMatrix(headMatrix);

  m_headEntity->addComponent(m_head);
  m_headEntity->addComponent(m_headTransform);
  m_headEntity->addComponent(m_jugglerMetalRoughMaterial);

}

void Juggler::createShoulders()
{
  m_shoulderEntity = new Qt3DCore::QEntity(this);
  m_shoulders = new Qt3DExtras::QCylinderMesh();
  m_shouldersTransform = new Qt3DCore::QTransform();
  makeMember(m_shoulders,
             m_shouldersTransform,
             m_shoulderEntity,
             SHOULDERS_ROTATION,
             SHOULDERS_TRANSLATION,
             SHOULDERS_LENGHT);

  m_leftShoulderEntity = new Qt3DCore::QEntity(this);
  m_leftShoulder = new Qt3DExtras::QSphereMesh();
  m_leftShoulderTransform = new Qt3DCore::QTransform();
  makeArticulation(m_leftShoulder,
                   m_leftShoulderTransform,
                   m_leftShoulderEntity,
                   LEFT_SHOULDER_TRANSLATION);

  m_rightShoulderEntity = new Qt3DCore::QEntity(this);
  m_rightShoulder = new Qt3DExtras::QSphereMesh();
  m_rightShoulderTransform = new Qt3DCore::QTransform();
  makeArticulation(m_rightShoulder,
                   m_rightShoulderTransform,
                   m_rightShoulderEntity,
                   RIGHT_SHOULDER_TRANSLATION);
}

void Juggler::createArms()
{
  m_leftArmEntity = new Qt3DCore::QEntity(this);
  m_leftArm = new Qt3DExtras::QCylinderMesh();
  m_leftArmTransform = new Qt3DCore::QTransform();
  makeMember(m_leftArm,
             m_leftArmTransform,
             m_leftArmEntity,
             ARM_ROTATION,
             LEFT_ARM_TRANSLATION,
             ARM_LENGHT);

  m_rightArmEntity = new Qt3DCore::QEntity(this);
  m_rightArm = new Qt3DExtras::QCylinderMesh();
  m_rightArmTransform = new Qt3DCore::QTransform();
  makeMember(m_rightArm,
             m_rightArmTransform,
             m_rightArmEntity,
             ARM_ROTATION,
             RIGHT_ARM_TRANSLATION,
             ARM_LENGHT);
}

void Juggler::createElbows()
{
  m_leftElbowEntity = new Qt3DCore::QEntity(this);
  m_leftElbow = new Qt3DExtras::QSphereMesh();
  m_leftElbowTransform = new Qt3DCore::QTransform();
  makeArticulation(m_leftElbow,
                   m_leftElbowTransform,
                   m_leftElbowEntity,
                   LEFT_ELBOW_TRANSLATION);

  m_rightElbowEntity = new Qt3DCore::QEntity(this);
  m_rightElbow = new Qt3DExtras::QSphereMesh();
  m_rightElbowTransform = new Qt3DCore::QTransform();
  makeArticulation(m_rightElbow,
                   m_rightElbowTransform,
                   m_rightElbowEntity,
                   RIGHT_ELBOW_TRANSLATION);
}

void Juggler::createForearms()
{
  m_leftForearmEntity = new Qt3DCore::QEntity(this);
  m_leftForearm = new Qt3DExtras::QCylinderMesh();
  m_leftForearmTransform = new Qt3DCore::QTransform();
  makeMember(m_leftForearm,
             m_leftForearmTransform,
             m_leftForearmEntity,
             FOREARM_ROTATION,
             LEFT_FOREARM_TRANSLATION,
             FOREARM_LENGHT);

  m_rightForearmEntity = new Qt3DCore::QEntity(this);
  m_rightForearm = new Qt3DExtras::QCylinderMesh();
  m_rightForearmTransform = new Qt3DCore::QTransform();
  makeMember(m_rightForearm,
             m_rightForearmTransform,
             m_rightForearmEntity,
             FOREARM_ROTATION,
             RIGHT_FOREARM_TRANSLATION,
             FOREARM_LENGHT);
}

void Juggler::createTrunk()
{
  m_trunkEntity = new Qt3DCore::QEntity(this);
  m_trunk = new Qt3DExtras::QCylinderMesh();
  m_trunkTransform = new Qt3DCore::QTransform();
  makeMember(m_trunk,
             m_trunkTransform,
             m_trunkEntity,
             TRUNK_ROTATION,
             TRUNK_TRANLATION,
             TRUNK_LENGHT);
}

void Juggler::createThighs()
{
  m_leftThighEntity = new Qt3DCore::QEntity(this);
  m_leftThigh = new Qt3DExtras::QCylinderMesh();
  m_leftThighTransform = new Qt3DCore::QTransform();
  makeMember(m_leftThigh,
             m_leftThighTransform,
             m_leftThighEntity,
             LEFT_THIGH_ROTATION,
             LEFT_THIGH_TRANSLATION,
             THIGH_LENGHT);

  m_rightThighEntity = new Qt3DCore::QEntity(this);
  m_rightThigh = new Qt3DExtras::QCylinderMesh();
  m_rightThighTransform = new Qt3DCore::QTransform();
  makeMember(m_rightThigh,
             m_rightThighTransform,
             m_rightThighEntity,
             RIGHT_THIGH_ROTATION,
             RIGHT_THIGH_TRANLATION,
             THIGH_LENGHT);
}

void Juggler::createKnees()
{
  m_leftKneeEntity = new Qt3DCore::QEntity(this);
  m_leftKnee = new Qt3DExtras::QSphereMesh();
  m_leftKneeTransform = new Qt3DCore::QTransform();
  makeArticulation(m_leftKnee,
                   m_leftKneeTransform,
                   m_leftKneeEntity,
                   LEFT_KNEE_TRANLATION);

  m_rightKneeEntity = new Qt3DCore::QEntity(this);
  m_rightKnee = new Qt3DExtras::QSphereMesh();
  m_rightKneeTransform = new Qt3DCore::QTransform();
  makeArticulation(m_rightKnee,
                   m_rightKneeTransform,
                   m_rightKneeEntity,
                   RIGHT_KNEE_TRANLATION);
}

void Juggler::createLegs()
{
  m_leftLegEntity = new Qt3DCore::QEntity(this);
  m_leftLeg = new Qt3DExtras::QCylinderMesh();
  m_leftLegTransform = new Qt3DCore::QTransform();
  makeMember(m_leftLeg,
             m_leftLegTransform,
             m_leftLegEntity,
             LEFT_LEG_ROTATION,
             LEFT_LEG_TRANSLATION,
             LEG_LENGHT);

  m_rightLegEntity = new Qt3DCore::QEntity(this);
  m_rightLeg = new Qt3DExtras::QCylinderMesh();
  m_rightLegTransform = new Qt3DCore::QTransform();
  makeMember(m_rightLeg,
             m_rightLegTransform,
             m_rightLegEntity,
             RIGHT_LEG_ROTATION,
             RIGHT_LEG_TRANSLATION,
             LEG_LENGHT);
}

void Juggler::setLeftHandPosition(QVector3D t_pos)
{
  hand aHand = leftHand;
  setHandPosition(t_pos, aHand);
}

void Juggler::setRightHandPosition(QVector3D t_pos)
{
  hand aHand = rightHand;
  setHandPosition(t_pos, aHand);
}

void Juggler::setHandPosition(QVector3D t_pos, hand t_hand)
{
  // declare variables differently defined between left and hand
  float hand_offset_x;
  float shoulder_x;
  QVector3D arm_translation;
  QVector3D elbow_translation;
  QVector3D forearm_translation;
  Qt3DCore::QTransform *armTransform;
  Qt3DCore::QTransform *elbowTransform;
  Qt3DCore::QTransform *forearmTransform;

  // set variables depending on witch hand
  if (t_hand == leftHand)
  {
    hand_offset_x = - HAND_OFFSET_X;
    shoulder_x = LEFT_SHOULDER_X;
    arm_translation = LEFT_ARM_TRANSLATION;
    elbow_translation = LEFT_ELBOW_TRANSLATION;
    forearm_translation = LEFT_FOREARM_TRANSLATION;
    armTransform = m_leftArmTransform;
    elbowTransform = m_leftElbowTransform;
    forearmTransform = m_leftForearmTransform;
  }
  else
  {
    hand_offset_x = HAND_OFFSET_X;
    shoulder_x = RIGHT_SHOULDER_X;
    arm_translation = RIGHT_ARM_TRANSLATION;
    elbow_translation = RIGHT_ELBOW_TRANSLATION;
    forearm_translation = RIGHT_FOREARM_TRANSLATION;
    armTransform = m_rightArmTransform;
    elbowTransform = m_rightElbowTransform;
    forearmTransform = m_rightForearmTransform;
  }

  // get relative coordonate of our position
  QVector3D relativePos(t_pos.x() - m_position.x(), t_pos.y(), t_pos.z() - m_position.z());
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(-m_rotY, QVector3D(0, 1, 0));
  relativePos = rot * relativePos;

  // Get angle on (y) for arm and forearm
  float angleY = qRadiansToDegrees(qAtan2(relativePos.x() + hand_offset_x, relativePos.z()));
  qDebug() << "angleY" << angleY;

  // find global Angle between z axis and [shoulder, prop's position]
  float globalAngle1 = qRadiansToDegrees(qAtan2(SHOULDER_Y - relativePos.y(), relativePos.z()));

  // get distance between shoulder and position
  float dist = qSqrt(
        qPow(shoulder_x - relativePos.x(), 2) +
        qPow(SHOULDER_Y - relativePos.y(), 2) +
        qPow(SHOULDER_Z - relativePos.z(), 2));

  // find angle between arm and [shoulder, position]
  // NOTE: simple because arm and forearm have same lenght
  float globalAngle2 = qRadiansToDegrees(qAcos(dist / (2 * FOREARM_LENGHT)));

  // find arm angle (on (x))
  float armAngle = - (90 - globalAngle1 - globalAngle2);

  // find forearm angle
  float forearmAngle = 90 - globalAngle1 + globalAngle2;

  // let's move arm
  // create new matrix
  QMatrix4x4 armMatrix;
  // translate as in creation
  armMatrix.translate(arm_translation);
  // rotate as in creation (currently useless as arm doesn't have initial rotation)
  armMatrix.rotate(QQuaternion::fromEulerAngles(ARM_ROTATION));
  // then rotate from shoulder
  armMatrix.translate(QVector3D(0, ARM_LENGHT / 2, 0));
  armMatrix.rotate(QQuaternion::fromEulerAngles(QVector3D(armAngle, angleY, 0)));
  armMatrix.translate(QVector3D(0, - ARM_LENGHT / 2, 0));
  armTransform->setMatrix(armMatrix);

  // translate elbow position
  // create matrix
  QMatrix4x4 elbowMatrix;
  // translate as in creation
  elbowMatrix.translate(elbow_translation);
  // rotate from shoulder
  elbowMatrix.translate(QVector3D(0, ARM_LENGHT, 0));
  elbowMatrix.rotate(QQuaternion::fromEulerAngles(QVector3D(armAngle, angleY, 0)));
  elbowMatrix.translate(QVector3D(0, - ARM_LENGHT, 0));
  elbowTransform->setMatrix(elbowMatrix);

  // let's move forearm
  // create a vec3 to follow elbow
  QVector3D translateToElbow = elbowTransform->translation() - elbow_translation;
  // create Matrix
  QMatrix4x4 forearmMatrix;
  // translate as in creation + follow elbow
  forearmMatrix.translate(forearm_translation + translateToElbow);
//  // rotate as in creation
  forearmMatrix.rotate(QQuaternion::fromEulerAngles(FOREARM_ROTATION));
  // rotate from elbow
  forearmMatrix.translate(QVector3D(0, FOREARM_LENGHT / 2, 0));
  forearmMatrix.rotate(QQuaternion::fromEulerAngles(QVector3D(90 - forearmAngle, 0, angleY)));
  forearmMatrix.translate(QVector3D(0, - FOREARM_LENGHT / 2, 0));
  forearmTransform->setMatrix(forearmMatrix);
}

void Juggler::setPosition(QVector3D t_position)
{
  if (m_position == t_position)
    return;

  m_position = t_position;
  emit positionChanged();
  updateTransform();
}

void Juggler::updateTransform()
{
  m_skeletonTransform->setTranslation(m_position);
}

QMatrix4x4 Juggler::getRotMatrix()
{
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.translate(m_position);
  rot.rotate(m_rotY, QVector3D(0, 1, 0));
  rot.translate(-m_position);
  return rot;
}

void Juggler::setPositionLHextPlus()
{
  QVector3D vecOffset(HAND_OFFSET_X + HAND_OFFSET_EXT_PLUS, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  m_posLHextPlus = vecMain;
}

void Juggler::setPositionLHext()
{
  QVector3D vecOffset(HAND_OFFSET_X + HAND_OFFSET_EXT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  m_posLHext = vecMain;
}

void Juggler::setPositionLHint()
{
  QVector3D vecOffset(HAND_OFFSET_X - HAND_OFFSET_INT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  m_posLHint = vecMain;
}

void Juggler::setPositionLHmed()
{
  QVector3D vecOffset(HAND_OFFSET_X, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  m_posLHmed = vecMain;
}

void Juggler::setPositionRHextPlus()
{
  QVector3D vecOffset(-HAND_OFFSET_X - HAND_OFFSET_EXT_PLUS, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  m_posRHextPlus = vecMain;
}

void Juggler::setPositionRHext()
{
  QVector3D vecOffset(-HAND_OFFSET_X - HAND_OFFSET_EXT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  m_posRHext = vecMain;
}

void Juggler::setPositionRHint()
{
  QVector3D vecOffset(-HAND_OFFSET_X + HAND_OFFSET_INT, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  m_posRHint = vecMain;
}

void Juggler::setPositionRHmed()
{
  QVector3D vecOffset(-HAND_OFFSET_X, HAND_OFFSET_Y, HAND_OFFSET_Z);
  QVector3D vecMain = m_position + vecOffset;
  QMatrix4x4 rot = getRotMatrix();
  vecMain = rot * vecMain;
  m_posRHmed = vecMain;
}

void Juggler::setPosHead()
{
  m_posHead = QVector3D(m_position.x(), HEAD_POS_Y, m_position.z());

  QVector3D temp = m_posHead + QVector3D(0, 0, 5);
  QMatrix4x4 rot = getRotMatrix();
  temp = rot * temp;
  m_headLookAt = temp;
}

QVector3D Juggler::getAbsoluteLeftShoulderPosition() const
{
  QVector3D retVector = LEFT_SHOULDER_TRANSLATION;
  // rajouter position puis faire tourner suivant rotY
  return retVector;
}

QVector3D Juggler::getAbsoluteLeftElbowPosition() const
{
  QVector3D retVector = LEFT_ELBOW_TRANSLATION;
  // rajouter position puis faire tourner suivant rotY
  return retVector;
}

QVector3D Juggler::getAbsoluteRightShoulderPosition() const
{
  QVector3D retVector = RIGHT_SHOULDER_TRANSLATION;
  // rajouter position puis faire tourner suivant rotY
  return retVector;
}

QVector3D Juggler::getAbsoluteRightElbowPosition() const
{
  QVector3D retVector = RIGHT_ELBOW_TRANSLATION;
  // rajouter position puis faire tourner suivant rotY
  return retVector;
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

void Juggler::makeMember(QCylinderMesh *t_member,
                         Qt3DCore::QTransform *t_memberTransform,
                         QEntity *t_memberEntity,
                         QVector3D t_rot,
                         QVector3D t_trans,
                         float t_length)
{

  t_memberEntity->addComponent(t_member);

  t_member->setRadius(MEMBERS_RADIUS);
  t_member->setRings(MEMBERS_RINGS);
  t_member->setSlices(MEMBERS_SLICES);
  t_member->setLength(t_length);

  QMatrix4x4 aMatrix = t_memberTransform->matrix();
  aMatrix.translate(t_trans);
  aMatrix.rotate(QQuaternion::fromEulerAngles(t_rot));
  t_memberTransform->setMatrix(aMatrix);
  t_memberEntity->addComponent(t_memberTransform);
  t_memberEntity->addComponent(m_jugglerMetalRoughMaterial);
}

void Juggler::makeArticulation(QSphereMesh *t_sphere,
                               Qt3DCore::QTransform *t_sphereTransform,
                               QEntity *t_sphereEntity,
                               QVector3D t_trans)
{
  t_sphere->setRadius(ARTICULATION_RADIUS);
  t_sphere->setRings(ARTICULATION_RINGS);
  t_sphere->setSlices(ARTICULATION_SLICES);

  QMatrix4x4 aMatrix = t_sphereTransform->matrix();
  aMatrix.translate(t_trans);
  t_sphereTransform->setMatrix(aMatrix);

  t_sphereEntity->addComponent(t_sphere);
  t_sphereEntity->addComponent(t_sphereTransform);
  t_sphereEntity->addComponent(m_jugglerMetalRoughMaterial);
}
