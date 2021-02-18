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

#include "jugglerleg.h"

JugglerLeg::JugglerLeg(QEntity *t_rootEntity,
                       QMetalRoughMaterial *t_jugglerMetalRoughMaterial,
                       QColor &t_color,
                       hand t_side)
  : m_legMaterial(t_jugglerMetalRoughMaterial),
    m_color(t_color),
    m_side(t_side),
    m_globalLegTransform(new Qt3DCore::QTransform()),
    m_haunchEntity(new QEntity(this)),
    m_haunchTransform(new Qt3DCore::QTransform()),
    m_thighEntity(new QEntity(m_haunchEntity)), // connect thigh to haunch
    m_kneeEntity(new QEntity(m_thighEntity)), // connect knee to thigh
    m_kneeTransform(new Qt3DCore::QTransform()),
    m_tibiaEntity(new QEntity(m_kneeEntity)) // connect tibia to knee
{
  // juggler will create legs with m_pelvisEntity as rootEntity
  QEntity::setParent(t_rootEntity);

  // translate
  m_globalLegTransform->setTranslation(GLOBAL_LEG_TRANSLATION);

  // rotation
  QVector3D legRotation;
  (m_side == leftHand) ?
        legRotation = LEFT_LEG_ROTATION :
      legRotation = RIGHT_LEG_ROTATION;
  m_globalLegTransform->setRotation(QQuaternion::fromEulerAngles(legRotation));

  // add transform to QEntity
  addComponent(m_globalLegTransform);

  // make haunch
  makeArticulation(new QSphereMesh(),
                   m_haunchTransform,
                   m_haunchEntity,
                   HAUNCH_TRANSLATION);

  // make thigh
  makeMember(new QCylinderMesh(),
             new Qt3DCore::QTransform(),
             m_thighEntity,
             THIGH_ROTATION,
             THIGH_TRANSLATION,
             THIGH_LENGHT);

  // make knee
  makeArticulation(new QSphereMesh(),
                   m_kneeTransform,
                   m_kneeEntity,
                   KNEE_TRANSLATION);

  // make tibia
  makeMember(new QCylinderMesh,
             new Qt3DCore::QTransform(),
             m_tibiaEntity,
             TIBIA_ROTATION,
             TIBIA_TRANSLATION,
             TIBIA_LENGHT);

  /************************* test zone ****************************/
  setHaunchRotationX(-20);
  setKneeRotationX(20);

}

void JugglerLeg::setHaunchRotationX(float t_angle)
{
  m_haunchTransform->setRotationX(t_angle);
}

void JugglerLeg::setHaunchRotationY(float t_angle)
{
  m_haunchTransform->setRotationY(t_angle);
}

void JugglerLeg::setKneeRotationX(float t_angle)
{
  m_kneeTransform->setRotationX(t_angle);
}

void JugglerLeg::makeMember(QCylinderMesh *t_member,
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
  t_memberEntity->addComponent(m_legMaterial);
}

void JugglerLeg::makeArticulation(QSphereMesh *t_sphere,
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
  t_sphereEntity->addComponent(m_legMaterial);

}
