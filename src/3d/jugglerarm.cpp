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
                       QMetalRoughMaterial *t_jugglerMetalRoughMaterial,
                       QColor &t_color,
                       hand t_side)
  : m_armMaterial(t_jugglerMetalRoughMaterial),
    m_color(t_color),
    m_side(t_side),
    m_globalArmTransform(new Qt3DCore::QTransform()),
    m_shoulderEntity(new QEntity(this)),
    m_shoulder(new QSphereMesh()),
    m_shoulderTransform(new Qt3DCore::QTransform()),
    m_armEntity(new QEntity(m_shoulderEntity)), // connect arm to shoulder
    m_arm(new QCylinderMesh()),
    m_armTransform(new Qt3DCore::QTransform()),
    m_elbowEntity(new QEntity(m_armEntity)), // connect elbow to arm
    m_elbow(new QSphereMesh()),
    m_elbowTransform(new Qt3DCore::QTransform()),
    m_forearmEntity(new QEntity(m_elbowEntity)), // connect foreaerm to elbow
    m_forearm(new QCylinderMesh()),
    m_forearmTransform(new Qt3DCore::QTransform())

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
  makeArticulation(m_shoulder,
                   m_shoulderTransform,
                   m_shoulderEntity,
                   CLAVICLES_TRANSLATION);

  // make arm
  makeMember(m_arm,
             m_armTransform,
             m_armEntity,
             ARM_ROTATION,
             ARM_TRANSLATION,
             ARM_LENGHT);

  // make elbow
  makeArticulation(m_elbow,
                   m_elbowTransform,
                   m_elbowEntity,
                   ELBOW_TRANSLATION);

  // make forearm
  makeMember(m_forearm,
             m_forearmTransform,
             m_forearmEntity,
             FOREARM_ROTATION,
             FOREARM_TRANSLATION,
             FOREARM_LENGHT);

  /**************************** test zone *********************************/

//  m_shoulderTransform->setRotationX(-10);
//  m_side == leftHand ?
//        m_shoulderTransform->setRotationY(45):
//        m_shoulderTransform->setRotationY(-45);
  m_elbowTransform->setRotationX(-90);

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

void JugglerArm::makeMember(QCylinderMesh *t_member,
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
  t_memberEntity->addComponent(m_armMaterial);
}

void JugglerArm::makeArticulation(QSphereMesh *t_sphere,
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
  t_sphereEntity->addComponent(m_armMaterial);

}

