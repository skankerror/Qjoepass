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
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;

class Juggler: public QEntity
{

  Q_OBJECT

  Q_PROPERTY(QVector3D m_position
             READ getPosition
             WRITE setPosition
             NOTIFY positionChanged)

  Q_PROPERTY(QVector3D m_leftHandPosition
             READ getLeftHandPosition
             WRITE setLeftHandPosition
             NOTIFY leftHandPositionChanged)

  Q_PROPERTY(QVector3D m_rightHandPosition
             READ getRightHandPosition
             WRITE setRightHandPosition
             NOTIFY rightHandPositionChanged)

public:

  explicit Juggler(QEntity *t_rootEntity,
                   float &t_rotY,
                   QVector2D &t_position,
                   QColor &t_color);

  QVector3D getPosition() const { return m_position; }
  Qt3DCore::QTransform * getSkeletonTransform() const { return m_skeletonTransform; };
  void setPosition(QVector3D t_position);
  void setSkeletonTransform(Qt3DCore::QTransform *t_transform) { m_skeletonTransform = t_transform; };

  QVector3D getLeftHandPosition() { return m_leftHandPosition; };
  void setLeftHandPosition(QVector3D t_pos);
  QVector3D getRightHandPosition() { return m_rightHandPosition; };
  void setRightHandPosition(QVector3D t_pos);

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

  float getRotY() const { return m_rotY; };

private:

  void createHead();
  void createShoulders();
  void createArms();
  void createElbows();
  void createForearms();
  void createTrunk();
  void createThighs();
  void createKnees();
  void createLegs();

  void updateTransform();

  QMatrix4x4 getRotMatrix();

  // setters for hands pos
  void setPositionLHextPlus(); // for helico, pancakes
  void setPositionLHext();
  void setPositionLHint();
  void setPositionLHmed();
  void setPositionRHextPlus(); // for helico, pancakes
  void setPositionRHext();
  void setPositionRHint();
  void setPositionRHmed();

  void setPosHead();

  // needed to find angle to animate
  QVector3D getAbsoluteLeftShoulderPosition() const;
  QVector3D getAbsoluteLeftElbowPosition() const;
  QVector3D getAbsoluteRightShoulderPosition() const;
  QVector3D getAbsoluteRightElbowPosition() const;


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


private slots:

  void setPositionHands();

signals:

  void skeletonTransformChanged();
  void positionChanged();
  void leftHandPositionChanged();
  void rightHandPositionChanged();
  void handPositionChanged();

private:

  // global members
  QMetalRoughMaterial *m_jugglerMetalRoughMaterial;

  Qt3DCore::QTransform *m_skeletonTransform;

  QVector3D m_position;
  QColor m_color;
  float m_rotY;

  bool m_enabled = true;

  // hands positions
  QVector3D m_leftHandPosition;
  QVector3D m_rightHandPosition;
  QVector3D m_myHandPosition;

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
  QEntity *m_headEntity;
  QSphereMesh *m_head;
  Qt3DCore::QTransform *m_headTransform;

  // shoulders
  QEntity *m_shoulderEntity;
  QCylinderMesh *m_shoulders;
  Qt3DCore::QTransform *m_shouldersTransform;

  QEntity *m_rightShoulderEntity;
  QSphereMesh *m_rightShoulder;
  Qt3DCore::QTransform *m_rightShoulderTransform;

  QEntity *m_leftShoulderEntity;
  QSphereMesh *m_leftShoulder;
  Qt3DCore::QTransform *m_leftShoulderTransform;

  // arms
  QEntity *m_leftArmEntity;
  QCylinderMesh *m_leftArm;
  Qt3DCore::QTransform *m_leftArmTransform;

  QEntity *m_rightArmEntity;
  QCylinderMesh *m_rightArm;
  Qt3DCore::QTransform *m_rightArmTransform;

  // elbows
  QEntity *m_rightElbowEntity;
  QSphereMesh *m_rightElbow;
  Qt3DCore::QTransform *m_rightElbowTransform;

  QEntity *m_leftElbowEntity;
  QSphereMesh *m_leftElbow;
  Qt3DCore::QTransform *m_leftElbowTransform;

  // forearms
  QEntity *m_leftForearmEntity;
  QCylinderMesh *m_leftForearm;
  Qt3DCore::QTransform *m_leftForearmTransform;

  QEntity *m_rightForearmEntity;
  QCylinderMesh *m_rightForearm;
  Qt3DCore::QTransform *m_rightForearmTransform;

  // trunk
  QEntity *m_trunkEntity;
  QCylinderMesh *m_trunk;
  Qt3DCore::QTransform *m_trunkTransform;

  // thighs
  QEntity *m_leftThighEntity;
  QCylinderMesh *m_leftThigh;
  Qt3DCore::QTransform *m_leftThighTransform;

  QEntity *m_rightThighEntity;
  QCylinderMesh *m_rightThigh;
  Qt3DCore::QTransform *m_rightThighTransform;

  // knees
  QEntity *m_rightKneeEntity;
  QSphereMesh *m_rightKnee;
  Qt3DCore::QTransform *m_rightKneeTransform;

  QEntity *m_leftKneeEntity;
  QSphereMesh *m_leftKnee;
  Qt3DCore::QTransform *m_leftKneeTransform;

  // legs
  QEntity *m_rightLegEntity;
  QCylinderMesh *m_rightLeg;
  Qt3DCore::QTransform *m_rightLegTransform;

  QEntity *m_leftLegEntity;
  QCylinderMesh *m_leftLeg;
  Qt3DCore::QTransform *m_leftLegTransform;

};

#endif // JUGGLER_H
