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
    m_rotY(t_rotY),
    m_headEntity(new QEntity(this)),
    m_head(new QSphereMesh()),
    m_headTransform(new Qt3DCore::QTransform()),
    m_claviclesEntity(new QEntity(this)),
    m_clavicles(new QCylinderMesh()),
    m_claviclesTransform(new Qt3DCore::QTransform()),
    m_trunkEntity(new QEntity(this)),
    m_trunk(new QCylinderMesh()),
    m_trunkTransform(new Qt3DCore::QTransform()),
    m_leftThighEntity(new QEntity(this)),
    m_leftThigh(new QCylinderMesh()),
    m_leftThighTransform(new Qt3DCore::QTransform()),
    m_rightThighEntity(new QEntity(this)),
    m_rightThigh(new QCylinderMesh),
    m_rightThighTransform(new Qt3DCore::QTransform()),
    m_leftKneeEntity(new QEntity(this)),
    m_leftKnee(new QSphereMesh()),
    m_leftKneeTransform(new Qt3DCore::QTransform()),
    m_rightKneeEntity(new QEntity(this)),
    m_rightKnee(new QSphereMesh()),
    m_rightKneeTransform(new Qt3DCore::QTransform()),
    m_leftLegEntity(new QEntity(this)),
    m_leftLeg(new QCylinderMesh()),
    m_leftLegTransform(new Qt3DCore::QTransform()),
    m_rightLegEntity(new QEntity(this)),
    m_rightLeg(new QCylinderMesh()),
    m_rightLegTransform(new Qt3DCore::QTransform())

{
  // set global material apllied on each entities
  m_jugglerMetalRoughMaterial->setBaseColor(m_color);
  m_jugglerMetalRoughMaterial->setMetalness(JUGGLER_METALNESS);
  m_jugglerMetalRoughMaterial->setRoughness(JUGGLER_ROUGHNESS);

  // create all the parts
  createHead();
  createBody();
  createArms();

  // translate our juggler
  m_position = QVector3D(t_position.x(), JUGGLER_TRANSLATION_Y, t_position.y());
  m_skeletonTransform->setTranslation(m_position);
  m_skeletonTransform->setScale(JUGGLER_SCALE);

  // rot our juggler
  m_skeletonTransform->setRotationY(m_rotY);

  // hello world here I am
  QEntity::setParent(t_rootEntity);
  addComponent(m_skeletonTransform);

  // we update hands positions and head
  setBodyPositions();

  // connect to always have correct send and receive props positions
  connect(this, SIGNAL(positionChanged()), this, SLOT(setBodyPositions()));

  /*********************** testing zone *********************************/
}

void Juggler::createHead()
{
  m_head->setRadius(HEAD_RADIUS);
  m_head->setRings(HEAD_RINGS);
  m_head->setSlices(HEAD_SLICES);

  m_headTransform->setTranslation(HEAD_TRANSLATE);

  m_headEntity->addComponent(m_head);
  m_headEntity->addComponent(m_headTransform);
  m_headEntity->addComponent(m_jugglerMetalRoughMaterial);
}

void Juggler::createBody()
{
  // clavicles
  makeMember(m_clavicles,
             m_claviclesTransform,
             m_claviclesEntity,
             CLAVICLES_ROTATION,
             CLAVICLES_TRANSLATION,
             CLAVICLES_LENGHT);

  // trunk
  makeMember(m_trunk,
             m_trunkTransform,
             m_trunkEntity,
             TRUNK_ROTATION,
             TRUNK_TRANLATION,
             TRUNK_LENGHT);

  // thighs
  makeMember(m_leftThigh,
             m_leftThighTransform,
             m_leftThighEntity,
             LEFT_THIGH_ROTATION,
             LEFT_THIGH_TRANSLATION,
             THIGH_LENGHT);

  makeMember(m_rightThigh,
             m_rightThighTransform,
             m_rightThighEntity,
             RIGHT_THIGH_ROTATION,
             RIGHT_THIGH_TRANLATION,
             THIGH_LENGHT);

  // knees
  makeArticulation(m_leftKnee,
                   m_leftKneeTransform,
                   m_leftKneeEntity,
                   LEFT_KNEE_TRANLATION);

  makeArticulation(m_rightKnee,
                   m_rightKneeTransform,
                   m_rightKneeEntity,
                   RIGHT_KNEE_TRANLATION);

  // legs
  makeMember(m_leftLeg,
             m_leftLegTransform,
             m_leftLegEntity,
             LEFT_LEG_ROTATION,
             LEFT_LEG_TRANSLATION,
             LEG_LENGHT);
  makeMember(m_rightLeg,
             m_rightLegTransform,
             m_rightLegEntity,
             RIGHT_LEG_ROTATION,
             RIGHT_LEG_TRANSLATION,
             LEG_LENGHT);
}

void Juggler::createArms()
{
  m_leftArm = new JugglerArm(m_claviclesEntity,
                             m_jugglerMetalRoughMaterial,
                             m_color,
                             hand(leftHand));

  m_rightArm = new JugglerArm(m_claviclesEntity,
                              m_jugglerMetalRoughMaterial,
                              m_color,
                              hand(rightHand));
}

void Juggler::setLeftHandPosition(QVector3D t_pos)
{
  // get relative coordonate of our position
  QVector3D relativePos = worldVecToJugglerVec(t_pos);

  // send to our arm
  m_leftArm->setHandPosition(relativePos);
}

void Juggler::setRightHandPosition(QVector3D t_pos)
{
  // get relative coordonate of our position
  QVector3D relativePos = worldVecToJugglerVec(t_pos);

  // send to our arm
  m_rightArm->setHandPosition(relativePos);
}

void Juggler::setPosition(QVector3D t_position)
{
  if (m_position == t_position)
    return;

  m_position = t_position;
  m_skeletonTransform->setTranslation(m_position);
  emit positionChanged();
}

QMatrix4x4 Juggler::getRotMatrix() const
{
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.translate(m_position);
  rot.rotate(m_rotY, QVector3D(0, 1, 0));
  rot.translate(-m_position);
  return rot;
}

QVector3D Juggler::worldVecToJugglerVec(const QVector3D t_pos) const
{
  // we translate
  QVector3D relativePos(t_pos.x() - m_position.x(), t_pos.y(), t_pos.z() - m_position.z());

  // we rotate
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(-m_rotY, QVector3D(0, 1, 0));
  relativePos = rot * relativePos;

  // we return
  return relativePos;
}

QVector3D Juggler::jugglerVecToWorldVec(const QVector3D t_pos) const
{
  // we translate
  QVector3D retPos = m_position + t_pos;
  // we rot
  QMatrix4x4 rot = getRotMatrix();
  retPos = rot * retPos;
  // we return
  return retPos;
}

void Juggler::setBodyPositions()
{
  // set all values to know where to and from the props will move
  m_posLHextPlus = jugglerVecToWorldVec(QVector3D(HAND_OFFSET_X + HAND_OFFSET_EXT_PLUS,
                                                  HAND_OFFSET_Y,
                                                  HAND_OFFSET_Z));

  m_posLHext = jugglerVecToWorldVec(QVector3D(HAND_OFFSET_X + HAND_OFFSET_EXT,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  m_posLHmed = jugglerVecToWorldVec(QVector3D(HAND_OFFSET_X,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  m_posLHint = jugglerVecToWorldVec(QVector3D(HAND_OFFSET_X - HAND_OFFSET_INT,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  m_posRHextPlus = jugglerVecToWorldVec(QVector3D(-HAND_OFFSET_X - HAND_OFFSET_EXT_PLUS,
                                                  HAND_OFFSET_Y,
                                                  HAND_OFFSET_Z));

  m_posRHext = jugglerVecToWorldVec(QVector3D(-HAND_OFFSET_X - HAND_OFFSET_EXT,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  m_posRHmed = jugglerVecToWorldVec(QVector3D(-HAND_OFFSET_X,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  m_posRHint = jugglerVecToWorldVec(QVector3D(-HAND_OFFSET_X + HAND_OFFSET_INT,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  // set head position for juggler camera
  m_posHead = QVector3D(m_position.x(),
                        HEAD_POS_Y,
                        m_position.z());

  QVector3D temp = m_posHead + LOOK_AT_VECTOR;
  QMatrix4x4 rot = getRotMatrix();
  temp = rot * temp;
  m_headLookAt = temp;
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

  t_memberTransform->setTranslation(t_trans);
  t_memberTransform->setRotation(QQuaternion::fromEulerAngles(t_rot));

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

  t_sphereTransform->setTranslation(t_trans);

  t_sphereEntity->addComponent(t_sphere);
  t_sphereEntity->addComponent(t_sphereTransform);
  t_sphereEntity->addComponent(m_jugglerMetalRoughMaterial);
}
