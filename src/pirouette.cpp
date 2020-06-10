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

Pirouette::Pirouette(QEntity *aRootEntity,
                     QMesh *aPirouetteMesh,
                     QColor aColor)
  : clubMetalRoughMaterial(new QMetalRoughMaterial()),
    pirouetteTransform(new Qt3DCore::QTransform()),
    color(aColor)
{
  pirouetteTransform->setScale(CLUB_SCALE);
  pirouetteTransform->setRotationX(100);

  clubMetalRoughMaterial->setBaseColor(color);
  clubMetalRoughMaterial->setMetalness(PROP_METALNESS);
  clubMetalRoughMaterial->setRoughness(PROP_ROUGHNESS);


  QEntity::setParent(aRootEntity);
  addComponent(pirouetteTransform);
  addComponent(aPirouetteMesh);
  addComponent(clubMetalRoughMaterial);
  setEnabled(enabled);
}

void Pirouette::setPosition(QVector3D aPosition)
{
  if (m_position == aPosition + initialPosition)
    return;

  m_position = aPosition + initialPosition;
  emit positionChanged(m_position);
  updateTransForm();
}

void Pirouette::updateTransForm()
{
  pirouetteTransform->setTranslation(m_position);
}
