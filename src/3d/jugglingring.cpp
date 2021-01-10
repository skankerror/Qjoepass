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

#include "jugglingring.h"


JugglingRing::JugglingRing(QEntity *t_rootEntity,
                           QTorusMesh *t_torusMesh,
                           QColor t_color,
                           launchTypeRing t_launchType)

  : m_ringMetalRoughMaterial(new QMetalRoughMaterial()),
    m_ringTransform(new Qt3DCore::QTransform()),
    m_color(t_color),
    m_launchType(t_launchType)

{
  m_ringTransform->setScale3D(QVector3D(RING_SCALE_X, RING_SCALE_Y, RING_SCALE_Z));

  m_ringMetalRoughMaterial->setBaseColor(m_color);
  m_ringMetalRoughMaterial->setMetalness(PROP_METALNESS);
  m_ringMetalRoughMaterial->setRoughness(PROP_ROUGHNESS);


  QEntity::setParent(t_rootEntity);
  addComponent(t_torusMesh);
  addComponent(m_ringTransform);
  addComponent(m_ringMetalRoughMaterial);
  setEnabled(m_enabled);
}

void JugglingRing::setPosition(QVector3D t_position)
{
  if (m_position == t_position)
    return;

  m_position = t_position;
  emit positionChanged(t_position);
  updateTranslation();
}

void JugglingRing::setRotX(float t_rotX)
{
  if (m_rotX == t_rotX)
    return;

  m_rotX = t_rotX;
  emit rotXChanged(m_rotX);
  updateRotX();
}

void JugglingRing::setRotY(float t_rotY)
{
  if (m_rotY == t_rotY)
    return;

  m_rotY = t_rotY;
  emit rotYChanged(m_rotY);
  updateRotY();
}

void JugglingRing::setRotZ(float t_rotZ)
{
  if (m_rotZ == t_rotZ)
    return;

  m_rotZ = t_rotZ;
  emit rotZChanged(m_rotZ);
  updateRotZ();
}

void JugglingRing::updateTranslation()
{
  m_ringTransform->setTranslation(m_position);
}

void JugglingRing::updateRotX()
{
  m_ringTransform->setRotationX(m_rotX);
}

void JugglingRing::updateRotY()
{
  m_ringTransform->setRotationY(m_rotY);
}

void JugglingRing::updateRotZ()
{
  m_ringTransform->setRotationZ(m_rotZ);
}
