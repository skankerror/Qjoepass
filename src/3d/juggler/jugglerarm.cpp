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

#include "jugglerarm.h"

JugglerArm::JugglerArm(QEntity *t_rootEntity,
                       Qt3DExtras::QMetalRoughMaterial *t_jugglerMetalRoughMaterial,
                       QColor &t_color,
                       hand t_side)
  : m_armMaterial(t_jugglerMetalRoughMaterial),
    m_color(t_color),
    m_side(t_side),
    m_globalArmTransform(new Qt3DCore::QTransform()),
    m_shoulderEntity(new QEntity(this)),
    m_shoulderTransform(new Qt3DCore::QTransform()),
    m_armEntity(new QEntity(m_shoulderEntity)), // connect arm to shoulder
    m_elbowEntity(new QEntity(m_armEntity)), // connect elbow to arm
    m_elbowTransform(new Qt3DCore::QTransform()),
    m_forearmEntity(new QEntity(m_elbowEntity)) // connect foreaerm to elbow

{
  // juggler will create arms with m_claviclesEntity as rootEntity
  QEntity::setParent(t_rootEntity);

  // translate according left or right
  m_side == leftHand ?
        m_globalArmTransform->setTranslation(GLOBAL_LEFT_ARM_TRANSLATION):
  m_globalArmTransform->setTranslation(GLOBAL_RIGHT_ARM_TRANSLATION);

  // rot inverse of shoulders' juggler
  m_globalArmTransform->setRotation(QQuaternion::fromEulerAngles(-CLAVICLES_ROTATION));

  // add transform to QEntity
  addComponent(m_globalArmTransform);

  // make shoulder
  makeArticulation(new Qt3DExtras::QSphereMesh(),
                   m_shoulderTransform,
                   m_shoulderEntity,
                   CLAVICLES_TRANSLATION);

  // make arm
  makeMember(new Qt3DExtras::QCylinderMesh(),
             new Qt3DCore::QTransform(),
             m_armEntity,
             ARM_ROTATION,
             ARM_TRANSLATION,
             ARM_LENGHT);

  // make elbow
  makeArticulation(new Qt3DExtras::QSphereMesh(),
                   m_elbowTransform,
                   m_elbowEntity,
                   ELBOW_TRANSLATION);

  // make forearm
  makeMember(new Qt3DExtras::QCylinderMesh,
             new Qt3DCore::QTransform(),
             m_forearmEntity,
             FOREARM_ROTATION,
             FOREARM_TRANSLATION,
             FOREARM_LENGHT);

}

void JugglerArm::setHandPosition(QVector3D &t_pos)
{
  // declare variables differently defined between left and right
  float hand_offset_x;
  float shoulder_x;

  // set variables depending on witch hand
  if (m_side == leftHand)
  {
    hand_offset_x = - HAND_OFFSET_X;
    shoulder_x = LEFT_SHOULDER_X;
  }
  else
  {
    hand_offset_x = HAND_OFFSET_X;
    shoulder_x = RIGHT_SHOULDER_X;
  }
  // Get angle on (y) for arm and forearm
  float angleY = qRadiansToDegrees(qAtan2(t_pos.x() + hand_offset_x, t_pos.z()));

  // find global Angle between (z) axis and [shoulder, prop's position]
  float globalAngle1 = qRadiansToDegrees(qAtan2(SHOULDER_Y - t_pos.y(), t_pos.z()));

  // get distance between shoulder and position
  float dist = qSqrt(
        qPow(shoulder_x - t_pos.x(), 2) +
        qPow(SHOULDER_Y - t_pos.y(), 2) +
        qPow(SHOULDER_Z - t_pos.z(), 2));

  // find angle between arm and [shoulder, position]
  // NOTE: simple because arm and forearm have same lenght
  float globalAngle2 = qRadiansToDegrees(qAcos(dist / (2 * FOREARM_LENGHT)));

  // find arm angle on (x)
  float armAngle = - (90 - globalAngle1 - globalAngle2);

  // find forearm angle on (x)
  float forearmAngle = - 90 + globalAngle1 - globalAngle2;

  m_shoulderTransform->setRotationX(armAngle);
  m_shoulderTransform->setRotationY(angleY);
  m_elbowTransform->setRotationX(forearmAngle);
}

void JugglerArm::setShoulderRotationX(float t_angle)
{
  m_shoulderTransform->setRotationX(t_angle);
}

void JugglerArm::setShoulderRotationY(float t_angle)
{
  m_shoulderTransform->setRotationY(t_angle);
}

void JugglerArm::setElbowRotationX(float t_angle)
{
  m_elbowTransform->setRotationX(t_angle);
}

void JugglerArm::makeMember(Qt3DExtras::QCylinderMesh *t_member,
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
  t_memberEntity->addComponent(m_armMaterial);
}

void JugglerArm::makeArticulation(Qt3DExtras::QSphereMesh *t_sphere,
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
  t_sphereEntity->addComponent(m_armMaterial);

}

