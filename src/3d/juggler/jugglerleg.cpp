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
                       Qt3DExtras::QMetalRoughMaterial *t_jugglerMetalRoughMaterial,
                       QColor &t_color,
                       hand t_side)
  : JugglerPart(t_rootEntity, t_jugglerMetalRoughMaterial, t_color),
    m_side(t_side),
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
  m_globalTransform->setTranslation(GLOBAL_LEG_TRANSLATION);

  // rotation
  QVector3D legRotation;
  (m_side == leftHand) ?
        legRotation = LEFT_LEG_ROTATION :
      legRotation = RIGHT_LEG_ROTATION;
  m_globalTransform->setRotation(QQuaternion::fromEulerAngles(legRotation));

  // add transform to QEntity
  addComponent(m_globalTransform);

  // make haunch
  makeArticulation(new Qt3DExtras::QSphereMesh(),
                   m_haunchTransform,
                   m_haunchEntity,
                   HAUNCH_TRANSLATION);

  // make thigh
  makeMember(new Qt3DExtras::QCylinderMesh(),
             new Qt3DCore::QTransform(),
             m_thighEntity,
             THIGH_ROTATION,
             THIGH_TRANSLATION,
             THIGH_LENGHT);

  // make knee
  makeArticulation(new Qt3DExtras::QSphereMesh(),
                   m_kneeTransform,
                   m_kneeEntity,
                   KNEE_TRANSLATION);

  // make tibia
  makeMember(new Qt3DExtras::QCylinderMesh,
             new Qt3DCore::QTransform(),
             m_tibiaEntity,
             TIBIA_ROTATION,
             TIBIA_TRANSLATION,
             TIBIA_LENGHT);

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
