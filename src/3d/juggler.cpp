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

  m_eulerAngles = QVector3D(JUGGLER_ROT_X, m_rotY, JUGGLER_ROT_Z);

  createHead();
  createShoulders();
  createArms();
  createElbows();
  createForearms();
  createTrunk();
  createThighs();
  createKnees();
  createLegs();

  // pourquoi ne pas faire une rotation sur le skeleton_transform avant la translation ?
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

  setLeftHandPosition(m_posLHext);

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

  float angleY = 45;
  float angleX = 20;

  QMatrix4x4 aMatrix = m_leftForearmTransform->matrix();
  aMatrix.translate(LEFT_ELBOW_TRANSLATION);
  aMatrix.rotate(QQuaternion::fromEulerAngles(angleX, angleY, 0));
  aMatrix.rotate(angleY, QVector3D(0,1,0));
  aMatrix.translate(QVector3D(-1, -4.8, 0));
  m_leftForearmTransform->setMatrix(aMatrix);
  // prendre la matrix
  // translate à l'emplacement du coude
  // rotationner
  // translate - emplacement du coude

//  m_leftForearmMatrix.setToIdentity();
//  m_leftForearmMatrix.rotate(m_rotY + angleY, QVector3D(0,1,0));
//  m_leftForearmMatrix.rotate(90 + angleX, QVector3D(1,0,0));
//  m_leftForearmMatrix.translate(LEFT_FOREARM_TRANSLATION);

//  m_leftForearmMatrix.rotate(QQuaternion::fromEulerAngles(90 + angleX, 0, 0));

//  m_leftForearmMatrix.translate(QVector3D(-0.25,+0.6, 0));// 45°
//  m_leftForearmMatrix.rotate((FOREARM_EULER_ROTATION));
//  m_leftForearmTransform->setMatrix(m_leftForearmMatrix);

//  QVector3D medPos = getPositionLHmed();
//  QMatrix4x4 aMatrix = m_leftForearmMatrix;
//  aMatrix.translate(LEFT_FOREARM_TRANSLATION);
//  aMatrix.rotate(FOREARM_EULER_ROTATION);
//  QMatrix4x4 rot = getRotMatrix();
//  float angle = qRadiansToDegrees(qAtan(t_pos.y() / t_pos.z()));
//  qDebug() << angle;
//  medPos = rot * medPos;
//  aMatrix.translate (QVector3D(0.75,0.0,1.5));
//  aMatrix.rotate (-angle,QVector3D(1,0,1));
//  aMatrix.rotate(45, QVector3D(0.75, 0, 1.5));
//  aMatrix.translate (QVector3D(-0.75,0.75,-0.75));
//  aMatrix.translate (QVector3D(-0.75,0.0,-1.5));
//  m_leftForearmTransform->setMatrix(aMatrix);

  emit leftHandPositionChanged();
}

void Juggler::setRightHandPosition(QVector3D t_pos)
{
  QVector3D medPos = getPositionRHmed();
  QMatrix4x4 aMatrix = m_rightForearmTransform->matrix();

  aMatrix.rotate(QQuaternion::fromEulerAngles(90.0f, 0.0f, 0.0));
  QMatrix4x4 rot = getRotMatrix();
  float angle = qRadiansToDegrees(qAtan(t_pos.y() / t_pos.z()));

  medPos = rot * medPos;
  t_pos = rot * t_pos;
  t_pos = medPos - t_pos;
  aMatrix.translate (QVector3D(0.75,0.0,1.5));
  aMatrix.rotate (-angle,QVector3D(1,0,1));
  aMatrix.translate (QVector3D(-0.75,0.75,-0.75));
  m_rightForearmTransform->setMatrix(aMatrix);
  emit rightHandPositionChanged();
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
  rot.rotate(m_eulerAngles.y(), QVector3D(0, 1, 0));
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
