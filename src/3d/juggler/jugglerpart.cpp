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

#include "jugglerpart.h"

JugglerPart::JugglerPart(QEntity *t_rootEntity,
                         Qt3DExtras::QMetalRoughMaterial *t_jugglerMetalRoughMaterial,
                         QColor &t_color)
  : m_material(t_jugglerMetalRoughMaterial)
  , m_color(t_color)
  , m_globalTransform(new Qt3DCore::QTransform())
{
  QEntity::setParent(t_rootEntity);
}

void JugglerPart::makeMember(Qt3DExtras::QCylinderMesh *t_member,
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
  t_memberEntity->addComponent(m_material);
}

void JugglerPart::makeArticulation(Qt3DExtras::QSphereMesh *t_sphere,
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
  t_sphereEntity->addComponent(m_material);
}
