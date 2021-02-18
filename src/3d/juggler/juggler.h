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

#ifndef JUGGLER_H
#define JUGGLER_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "jugglerarm.h"
#include "jugglerleg.h"
#include "jugglerhead.h"
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;

class Juggler: public QEntity
{

  Q_OBJECT

  // in case we animate juggler position
  Q_PROPERTY(QVector3D m_position
             READ getPosition
             WRITE setPosition
             NOTIFY positionChanged)

  // needed for anim
  Q_PROPERTY(QVector3D m_leftHandPosition
             READ getLeftHandPosition
             WRITE setLeftHandPosition
             NOTIFY leftHandPositionChanged)

  // needed for anim
  Q_PROPERTY(QVector3D m_rightHandPosition
             READ getRightHandPosition
             WRITE setRightHandPosition
             NOTIFY rightHandPositionChanged)


public:

  explicit Juggler(QEntity *t_rootEntity,
                   float &t_rotY,
                   QVector2D &t_position,
                   QColor &t_color);

  // property getters
  QVector3D getPosition() const { return m_position; }
  QVector3D getLeftHandPosition() { return m_leftHandPosition; };
  QVector3D getRightHandPosition() { return m_rightHandPosition; };

  // getters for hands positions ext to catch, int to launch, med for siteswap 2
  QVector3D getPositionLHextPlus() const { return m_posLHextPlus; }; // helico, pancakes
  QVector3D getPositionLHext() const { return m_posLHext; };
  QVector3D getPositionLHint() const { return m_posLHint; };
  QVector3D getPositionLHmed() const { return m_posLHmed; };
  QVector3D getPositionRHextPlus() const { return m_posRHextPlus; }; // helico, pancakes
  QVector3D getPositionRHext() const { return m_posRHext; };
  QVector3D getPositionRHint() const { return m_posRHint; };
  QVector3D getPositionRHmed() const { return m_posRHmed; };
  // getters for camera position
  QVector3D getPositionHead() const { return m_posHead; };
  QVector3D getLookAt() const { return m_headLookAt; };

  // needed for rotate props
  float getRotY() const { return m_rotY; };

  // needed to pass vec between juggler and world
  QVector3D worldVecToJugglerVec(const QVector3D t_pos) const;
  QVector3D jugglerVecToWorldVec(const QVector3D t_pos) const;

private:

  // cstr private methods
  void createHead();
  void createBody();
  void createArms();
  void createLegs();
  void makeMember(QCylinderMesh *t_member,
                  Qt3DCore::QTransform *t_memberTransform,
                  QEntity *t_memberEntity,
                  QVector3D t_rot,
                  QVector3D t_trans,
                  float t_length);
  void makeArticulation(QSphereMesh *t_phere,
                        Qt3DCore::QTransform *t_sphereTransform,
                        QEntity *t_sphereEntity,
                        QVector3D t_trans);

  // used by several methods
  QMatrix4x4 getRotMatrix() const;

signals:

  // signals properties
  void positionChanged(); // connected to setBodyPositions()
  // connect to prop during dwell
  void leftHandPositionChanged(QVector3D);
  void rightHandPositionChanged(QVector3D);

public slots:

  // property setters
  void setPosition(QVector3D t_position);
  void setLeftHandPosition(QVector3D t_pos);
  void setRightHandPosition(QVector3D t_pos);

private slots:

  // update all juggling positions and head position for camera
  void setBodyPositions();

private:

  // global members
  QMetalRoughMaterial *m_jugglerMetalRoughMaterial;
  Qt3DCore::QTransform *m_skeletonTransform;
  QVector3D m_position;
  QColor m_color;
  float m_rotY;
  bool m_enabled = true;

  // hands positions
  QVector3D m_leftHandPosition; // for property
  QVector3D m_rightHandPosition; // for property

  QVector3D m_posLHextPlus; // helico pancake
  QVector3D m_posLHext;
  QVector3D m_posLHint;
  QVector3D m_posLHmed;
  QVector3D m_posRHextPlus; // helico pancake
  QVector3D m_posRHext;
  QVector3D m_posRHint;
  QVector3D m_posRHmed;

  // head position
  QVector3D m_posHead;
  QVector3D m_headLookAt;

  // head
//  QEntity *m_headEntity;
//  Qt3DCore::QTransform *m_headTransform;

  // clavicles
  QEntity *m_claviclesEntity;

  // Arms
  JugglerArm *m_leftArm;
  JugglerArm *m_rightArm;

  // trunk
  QEntity *m_trunkEntity;

  // pelvis
  QEntity *m_pelvisEntity;
  Qt3DCore::QTransform *m_pelvisTransform;

  // legs
  JugglerLeg *m_leftLeg;
  JugglerLeg *m_rightLeg;

  // head
  JugglerHead *m_head;

};

#endif // JUGGLER_H
