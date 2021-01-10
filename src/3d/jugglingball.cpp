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

#include "jugglingball.h"

JugglingBall::JugglingBall(QEntity *t_rootEntity,
                           QSphereMesh *t_sphereMesh,
                           QColor &t_color,
                           launchTypeBall t_launchType)

  : m_ballMetalRoughMaterial(new QMetalRoughMaterial()),
    m_sphereTransform(new Qt3DCore::QTransform()),
    m_color(t_color),
    m_launchType(t_launchType)
{
  m_sphereTransform->setScale(BALL_SCALE);

  m_ballMetalRoughMaterial->setBaseColor(m_color);
  m_ballMetalRoughMaterial->setMetalness(PROP_METALNESS);
  m_ballMetalRoughMaterial->setRoughness(PROP_ROUGHNESS);

  QEntity::setParent(t_rootEntity);
  addComponent(t_sphereMesh);
  addComponent(m_ballMetalRoughMaterial);
  addComponent(m_sphereTransform);
}

void JugglingBall::setPosition(QVector3D t_position)
{
  if (m_position == t_position)
    return;

  m_position = t_position;
  emit positionChanged(t_position);
  updateTransForm();
}

void JugglingBall::updateTransForm()
{
  m_sphereTransform->setTranslation(m_position);
}
