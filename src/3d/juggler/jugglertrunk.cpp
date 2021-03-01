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

#include "jugglertrunk.h"

JugglerTrunk::JugglerTrunk(QEntity *t_rootEntity,
                           Qt3DExtras::QMetalRoughMaterial *t_jugglerMetalRoughMaterial,
                           QColor &t_color)
  : JugglerPart(t_rootEntity, t_jugglerMetalRoughMaterial, t_color),
    m_pelvisEntity(new QEntity(this)),
    m_pelvisTransform(new Qt3DCore::QTransform()),
    m_spineEntity(new QEntity(m_pelvisEntity)), // connect spine to pelvis
    m_claviclesEntity(new QEntity(m_spineEntity)), // connect clavicles to spine
    m_claviclesTransform(new Qt3DCore::QTransform())
{
  // juggler will create legs with m_pelvisEntity as rootEntity
  QEntity::setParent(t_rootEntity);

  // translate
  m_globalTransform->setTranslation(TRUNK_TRANSLATION);

  // rotation
  m_globalTransform->setRotation(QQuaternion::fromEulerAngles(TRUNK_ROTATION));

  // add transform to QEntity
  addComponent(m_globalTransform);

  // make pelvis
  makeArticulation(new Qt3DExtras::QSphereMesh(),
                   m_pelvisTransform,
                   m_pelvisEntity,
                   PELVIS_TRANSLATION);

  // make spine
  makeMember(new Qt3DExtras::QCylinderMesh(),
             new Qt3DCore::QTransform(),
             m_spineEntity,
             SPINE_ROTATION,
             SPINE_TRANSLATION,
             SPINE_LENGHT);

  // make clavicles
  makeMember(new Qt3DExtras::QCylinderMesh(),
             m_claviclesTransform,
             m_claviclesEntity,
             CLAVICLES_ROTATION,
             CLAVICLES_TRANSLATION,
             CLAVICLES_LENGHT);

}
