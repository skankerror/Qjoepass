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

#include "jugglerhead.h"

JugglerHead::JugglerHead(QEntity *t_rootEntity,
                         Qt3DExtras::QMetalRoughMaterial *t_jugglerMetalRoughMaterial,
                         QColor &t_color)
  : JugglerPart(t_rootEntity, t_jugglerMetalRoughMaterial, t_color),
    m_lastVertebrateEntity(new QEntity(this)),
    m_lastVertebrateTransform(new Qt3DCore::QTransform()),
    m_neckEntity(new QEntity(m_lastVertebrateEntity)), // connect neck to vertebrate
    m_skullEntity(new QEntity(m_neckEntity)), // connect skull to neck
    m_leftEyeEntity(new QEntity(m_skullEntity)), // connect eyes to skull
    m_rightEyeEntity(new QEntity(m_skullEntity)) // ***
{
  // juggler will create head with m_claviclesEntity as rootEntity
  QEntity::setParent(t_rootEntity);

  // translate
  m_globalTransform->setTranslation(HEAD_TRANSLATION);

  // rotate
  m_globalTransform->setRotation(QQuaternion::fromEulerAngles(HEAD_ROTATION));

  // add transform to QEntity
  addComponent(m_globalTransform);

  // make last vertebrate
  auto sphere = new Qt3DExtras::QSphereMesh();
  sphere->setRadius(ARTICULATION_RADIUS);
  sphere->setRings(ARTICULATION_RINGS);
  sphere->setSlices(ARTICULATION_SLICES);
  m_lastVertebrateTransform->setTranslation(LAST_VERTEBRATE_TRANSLATION);
  m_lastVertebrateEntity->addComponent(sphere);
  m_lastVertebrateEntity->addComponent(m_lastVertebrateTransform);
  m_lastVertebrateEntity->addComponent(m_material);

  // make neck
  auto cylinder = new Qt3DExtras::QCylinderMesh();
  cylinder->setRadius(MEMBERS_RADIUS);
  cylinder->setRings(MEMBERS_RINGS);
  cylinder->setSlices(MEMBERS_SLICES);
  cylinder->setLength(NECK_LENGHT);
  auto neckTransform = new Qt3DCore::QTransform();
  neckTransform->setTranslation(NECK_TRANSLATION);
  neckTransform->setRotation(QQuaternion::fromEulerAngles(NECK_ROTATION));
  m_neckEntity->addComponent(cylinder);
  m_neckEntity->addComponent(neckTransform);
  m_neckEntity->addComponent(m_material);

  // make skull
  auto skull = new Qt3DExtras::QSphereMesh();
  skull->setRadius(SKULL_RADIUS);
  skull->setRings(SKULL_RINGS);
  skull->setSlices(SKULL_SLICES);
  auto skullTransform = new Qt3DCore::QTransform();
  skullTransform->setTranslation(SKULL_TRANSLATION);
  m_skullEntity->addComponent(skull);
  m_skullEntity->addComponent(skullTransform);
  m_skullEntity->addComponent(m_material);

  // make eyes
  //left
  auto leftEye = new Qt3DExtras::QSphereMesh();
  leftEye->setRadius(EYE_RADIUS);
  leftEye->setRings(EYE_RINGS);
  leftEye->setSlices(EYE_SLICES);
  auto leftEyeTransform = new Qt3DCore::QTransform();
  leftEyeTransform->setTranslation(LEFT_EYE_TRANSLATION);
  m_leftEyeEntity->addComponent(leftEye);
  m_leftEyeEntity->addComponent(leftEyeTransform);
  m_leftEyeEntity->addComponent(m_material);
  //right
  auto rightEye = new Qt3DExtras::QSphereMesh();
  rightEye->setRadius(EYE_RADIUS);
  rightEye->setRings(EYE_RINGS);
  rightEye->setSlices(EYE_SLICES);
  auto rightEyeTransform = new Qt3DCore::QTransform();
  rightEyeTransform->setTranslation(RIGHT_EYE_TRANSLATION);
  m_rightEyeEntity->addComponent(rightEye);
  m_rightEyeEntity->addComponent(rightEyeTransform);
  m_rightEyeEntity->addComponent(m_material);
}

void JugglerHead::setHeadRotationX(float t_angle)
{
  m_lastVertebrateTransform->setRotationX(t_angle);
}

void JugglerHead::setHeadRotationY(float t_angle)
{
  m_lastVertebrateTransform->setRotationY(t_angle);
}
