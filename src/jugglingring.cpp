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


JugglingRing::JugglingRing(QEntity *aRootEntity,
                           QTorusMesh *torusMesh,
                           QColor aColor,
                           launchTypeRing aLaunchType)

  : ringMetalRoughMaterial(new QMetalRoughMaterial()),
    ringTransform(new Qt3DCore::QTransform()),
    color(aColor),
    launchType(aLaunchType)

{
  ringTransform->setScale3D(QVector3D(RING_SCALE_X, RING_SCALE_Y, RING_SCALE_Z));

  ringMetalRoughMaterial->setBaseColor(color);
  ringMetalRoughMaterial->setMetalness(PROP_METALNESS);
  ringMetalRoughMaterial->setRoughness(PROP_ROUGHNESS);


  QEntity::setParent(aRootEntity);
  addComponent(torusMesh);
  addComponent(ringTransform);
  addComponent(ringMetalRoughMaterial);
  setEnabled(enabled);

}

void JugglingRing::setPosition(QVector3D position)
{
  if (m_position == position)
    return;

  m_position = position;
  emit positionChanged(position);
  updateTranslation();
}

void JugglingRing::setRotX(float aRot)
{
  if (rotX == aRot)
    return;

  rotX = aRot;
  emit rotXChanged(rotX);
  updateRotX();
}

void JugglingRing::setRotY(float aRot)
{
  if (rotY == aRot)
    return;

  rotY = aRot;
  emit rotYChanged(rotY);
  updateRotY();
}

void JugglingRing::setRotZ(float aRot)
{
  if (rotZ == aRot)
    return;

  rotZ = aRot;
  emit rotZChanged(rotZ);
  updateRotZ();
}

void JugglingRing::updateTranslation()
{
  ringTransform->setTranslation(m_position);
}

void JugglingRing::updateRotX()
{
  ringTransform->setRotationX(rotX);
}

void JugglingRing::updateRotY()
{
  ringTransform->setRotationY(rotY);
}

void JugglingRing::updateRotZ()
{
  ringTransform->setRotationZ(rotZ);
}
