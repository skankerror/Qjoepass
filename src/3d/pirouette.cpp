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

#include "pirouette.h"

Pirouette::Pirouette(QEntity *t_rootEntity,
                     QMesh *t_pirouetteMesh,
                     QColor &t_color,
                     launchTypeClub t_launchType)

  : m_clubMetalRoughMaterial(new QMetalRoughMaterial()),
    m_pirouetteTransform(new Qt3DCore::QTransform()),
    m_color(t_color),
    m_launchType(t_launchType)
{
  m_pirouetteTransform->setScale(CLUB_SCALE);

  m_clubMetalRoughMaterial->setBaseColor(m_color);
  m_clubMetalRoughMaterial->setMetalness(PROP_METALNESS);
  m_clubMetalRoughMaterial->setRoughness(PROP_ROUGHNESS);


  QEntity::setParent(t_rootEntity);
  addComponent(m_pirouetteTransform);
  addComponent(t_pirouetteMesh);
  addComponent(m_clubMetalRoughMaterial);
  setEnabled(m_enabled);
}

void Pirouette::setPosition(QVector3D t_position)
{
  if (m_position == t_position)
    return;

  m_position = t_position;
  emit positionChanged(m_position);
  updateTranslation();
}

void Pirouette::setRotX(float t_rotX)
{
  if (m_rotX == t_rotX)
    return;

  m_rotX = t_rotX;
  emit rotXChanged(m_rotX);
  updateRotX();
}

void Pirouette::setRotY(float t_rotY)
{
  if (m_rotY == t_rotY)
    return;

  m_rotY = t_rotY;
  emit rotYChanged(m_rotY);
  updateRotY();
}

void Pirouette::setRotZ(float t_rotZ)
{
  if (m_rotZ == t_rotZ)
    return;

  m_rotZ = t_rotZ;
  emit rotZChanged(m_rotZ);
  updateRotZ();
}

void Pirouette::updateTranslation()
{
  m_pirouetteTransform->setTranslation(m_position);
}

void Pirouette::updateRotX()
{
  m_pirouetteTransform->setRotationX(m_rotX);
}

void Pirouette::updateRotY()
{
  m_pirouetteTransform->setRotationY(m_rotY);
}

void Pirouette::updateRotZ()
{
  m_pirouetteTransform->setRotationZ(m_rotZ);
}
