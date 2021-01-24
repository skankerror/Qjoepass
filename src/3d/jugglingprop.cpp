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
