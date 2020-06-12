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
                     QColor &aColor,
                     launchTypeClub aLaunchType)

  : clubMetalRoughMaterial(new QMetalRoughMaterial()),
    pirouetteTransform(new Qt3DCore::QTransform()),
    color(aColor),
    launchType(aLaunchType)
{
  pirouetteTransform->setScale(CLUB_SCALE);
//  pirouetteTransform->setRotationX(CLUB_BASIC_ROTX);

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
  if (m_position == aPosition)
    return;

  m_position = aPosition;
  emit positionChanged(m_position);
  updateTranslation();
}

void Pirouette::setRotX(float aRot)
{
  if (rotX == aRot)
    return;

  rotX = aRot;
  emit rotXChanged(rotX);
  updateRotX();
}

void Pirouette::setRotY(float aRot)
{
  if (rotY == aRot)
    return;

  rotY = aRot;
  emit rotYChanged(rotY);
  updateRotY();
}

void Pirouette::setRotZ(float aRot)
{
  if (rotZ == aRot)
    return;

  rotZ = aRot;
  emit rotZChanged(rotZ);
  updateRotZ();
}

void Pirouette::updateTranslation()
{
  pirouetteTransform->setTranslation(m_position);
}

void Pirouette::updateRotX()
{
  pirouetteTransform->setRotationX(rotX);
}

void Pirouette::updateRotY()
{
  pirouetteTransform->setRotationY(rotY);
}

void Pirouette::updateRotZ()
{
  pirouetteTransform->setRotationZ(rotZ);
}
