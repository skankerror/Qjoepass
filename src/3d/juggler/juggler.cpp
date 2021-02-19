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

#include "juggler.h"
#include <QDebug>

Juggler::Juggler(QEntity *t_rootEntity,
                 float &t_rotY,
                 QVector2D &t_position,
                 QColor &t_color)

  : m_jugglerMetalRoughMaterial(new QMetalRoughMaterial()),
    m_skeletonTransform(new Qt3DCore::QTransform()),
    m_color(t_color),
    m_rotY(t_rotY)
{
  // set global material apllied on each entities
  m_jugglerMetalRoughMaterial->setBaseColor(m_color);
  m_jugglerMetalRoughMaterial->setMetalness(JUGGLER_METALNESS);
  m_jugglerMetalRoughMaterial->setRoughness(JUGGLER_ROUGHNESS);

  // create all the parts
  createTrunk();
  createArms();
  createLegs();
  createHead();

  // translate our juggler
  m_position = QVector3D(t_position.x(), JUGGLER_TRANSLATION_Y, t_position.y());
  m_skeletonTransform->setTranslation(m_position);
  m_skeletonTransform->setScale(JUGGLER_SCALE);

  // rot our juggler
  m_skeletonTransform->setRotationY(m_rotY);

  // hello world here I am
  QEntity::setParent(t_rootEntity);
  addComponent(m_skeletonTransform);

  // we update hands positions and head
  setBodyPositions();

  // connect to always have correct send and receive props positions
  connect(this, SIGNAL(positionChanged()), this, SLOT(setBodyPositions()));

  /*********************** testing zone *********************************/

//  m_head->setHeadRotationX(15);
//  m_head->setHeadRotationY(25);
//  m_leftLeg->setHaunchRotationX(-20);
//  m_leftLeg->setKneeRotationX(20);
//  m_trunk->setPelvisRotationX(15);
//  m_trunk->setClaviculesRotationY(10);
}

void Juggler::createTrunk()
{
  m_trunk = new JugglerTrunk(this,
                             m_jugglerMetalRoughMaterial,
                             m_color);
}

void Juggler::createArms()
{
  m_leftArm = new JugglerArm(m_trunk->getClaviclesEntity(),
                             m_jugglerMetalRoughMaterial,
                             m_color,
                             hand(leftHand));

  m_rightArm = new JugglerArm(m_trunk->getClaviclesEntity(),
                              m_jugglerMetalRoughMaterial,
                              m_color,
                              hand(rightHand));
}

void Juggler::createLegs()
{
  m_leftLeg = new JugglerLeg(m_trunk->getPelvisEntity(),
                             m_jugglerMetalRoughMaterial,
                             m_color,
                             hand(leftHand));

  m_leftLeg = new JugglerLeg(m_trunk->getPelvisEntity(),
                             m_jugglerMetalRoughMaterial,
                             m_color,
                             hand(rightHand));
}

void Juggler::createHead()
{
  m_head = new JugglerHead(m_trunk->getClaviclesEntity(),
                           m_jugglerMetalRoughMaterial,
                           m_color);
}

void Juggler::setLeftHandPosition(QVector3D t_pos)
{
  // get relative coordonate of our position
  QVector3D relativePos = worldVecToJugglerVec(t_pos);

  // send to our arm
  m_leftArm->setHandPosition(relativePos);
  emit leftHandPositionChanged(t_pos);
}

void Juggler::setRightHandPosition(QVector3D t_pos)
{
  // get relative coordonate of our position
  QVector3D relativePos = worldVecToJugglerVec(t_pos);

  // send to our arm
  m_rightArm->setHandPosition(relativePos);
  emit rightHandPositionChanged(t_pos);
}

void Juggler::setPosition(QVector3D t_position)
{
  if (m_position == t_position)
    return;

  m_position = t_position;
  m_skeletonTransform->setTranslation(m_position);
  emit positionChanged();
}

QMatrix4x4 Juggler::getRotMatrix() const
{
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.translate(m_position);
  rot.rotate(m_rotY, QVector3D(0, 1, 0));
  rot.translate(-m_position);
  return rot;
}

QVector3D Juggler::worldVecToJugglerVec(const QVector3D t_pos) const
{
  // we translate
  QVector3D relativePos(t_pos.x() - m_position.x(), t_pos.y(), t_pos.z() - m_position.z());

  // we rotate
  QMatrix4x4 rot;
  rot.setToIdentity();
  rot.rotate(-m_rotY, QVector3D(0, 1, 0));
  relativePos = rot * relativePos;

  // we return
  return relativePos;
}

QVector3D Juggler::jugglerVecToWorldVec(const QVector3D t_pos) const
{
  // we translate
  QVector3D retPos = m_position + t_pos;
  // we rot
  QMatrix4x4 rot = getRotMatrix();
  retPos = rot * retPos;
  // we return
  return retPos;
}

void Juggler::setBodyPositions()
{
  // set all values to know where to and from the props will move
  m_posLHextPlus = jugglerVecToWorldVec(QVector3D(HAND_OFFSET_X + HAND_OFFSET_EXT_PLUS,
                                                  HAND_OFFSET_Y,
                                                  HAND_OFFSET_Z));

  m_posLHext = jugglerVecToWorldVec(QVector3D(HAND_OFFSET_X + HAND_OFFSET_EXT,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  m_posLHmed = jugglerVecToWorldVec(QVector3D(HAND_OFFSET_X,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  m_posLHint = jugglerVecToWorldVec(QVector3D(HAND_OFFSET_X - HAND_OFFSET_INT,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  m_posRHextPlus = jugglerVecToWorldVec(QVector3D(-HAND_OFFSET_X - HAND_OFFSET_EXT_PLUS,
                                                  HAND_OFFSET_Y,
                                                  HAND_OFFSET_Z));

  m_posRHext = jugglerVecToWorldVec(QVector3D(-HAND_OFFSET_X - HAND_OFFSET_EXT,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  m_posRHmed = jugglerVecToWorldVec(QVector3D(-HAND_OFFSET_X,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  m_posRHint = jugglerVecToWorldVec(QVector3D(-HAND_OFFSET_X + HAND_OFFSET_INT,
                                              HAND_OFFSET_Y,
                                              HAND_OFFSET_Z));

  // set head position for juggler camera
  m_posHead = QVector3D(m_position.x(),
                        HEAD_POS_Y,
                        m_position.z());

  QVector3D temp = m_posHead + LOOK_AT_VECTOR;
  QMatrix4x4 rot = getRotMatrix();
  temp = rot * temp;
  m_headLookAt = temp;
}

