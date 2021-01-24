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

#include "jugglingprop.h"

JugglingProp::JugglingProp(QEntity *t_rootEntity,
                           QColor &t_color)

  : m_propMetalRoughMaterial(new QMetalRoughMaterial()),
    m_propTransform(new Qt3DCore::QTransform()),
    m_color(t_color)
{
  m_propMetalRoughMaterial->setBaseColor(m_color);
  m_propMetalRoughMaterial->setMetalness(PROP_METALNESS);
  m_propMetalRoughMaterial->setRoughness(PROP_ROUGHNESS);

  QEntity::setParent(t_rootEntity);
  addComponent(m_propMetalRoughMaterial);
  addComponent(m_propTransform);
  setEnabled(m_enabled);
}

void JugglingProp::setPosition(QVector3D t_position)
{
  if (m_position == t_position)
    return;

  m_position = t_position;
  emit positionChanged(t_position);
  updateTransform();
}

void JugglingProp::updateTransform()
{
  m_propTransform->setTranslation(m_position);
}

void JugglingProp::setRotX(float t_rotX)
{
  if (m_rotX == t_rotX)
    return;

  m_rotX = t_rotX;
  emit rotXChanged(m_rotX);
  updateRotX();
}

void JugglingProp::setRotY(float t_rotY)
{
  if (m_rotY == t_rotY)
    return;

  m_rotY = t_rotY;
  emit rotYChanged(m_rotY);
  updateRotY();
}

void JugglingProp::setRotZ(float t_rotZ)
{
  if (m_rotZ == t_rotZ)
    return;

  m_rotZ = t_rotZ;
  emit rotZChanged(m_rotZ);
  updateRotZ();
}

void JugglingProp::updateRotX()
{
  m_propTransform->setRotationX(m_rotX);
}

void JugglingProp::updateRotY()
{
  m_propTransform->setRotationY(m_rotY);
}

void JugglingProp::updateRotZ()
{
  m_propTransform->setRotationZ(m_rotZ);
}
