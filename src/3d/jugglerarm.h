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

#ifndef JUGGLERARM_H
#define JUGGLERARM_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;

class JugglerArm : public QEntity
{

  Q_OBJECT

//  Q_PROPERTY(QVector3D m_handPosition
//             READ getHandPosition
//             WRITE setHandPosition
//             NOTIFY handPositionChanged)

  // CACA BOUDIN

public:

  JugglerArm(QEntity *t_rootEntity,
             QMetalRoughMaterial *t_jugglerMetalRoughMaterial,
             QColor &t_color,
             hand t_side);

  void setShoulderRotationX(float t_angle);
  void setShoulderRotationY(float t_angle);

  void setElbowRotationX(float t_angle);

//  QVector3D getHandPosition() const { return m_handPosition; };
//  void setHandPosition(const QVector3D &t_handPosition) { m_handPosition = t_handPosition; };

private:

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


//signals:

//  void handPositionChanged();

private:

  QMetalRoughMaterial *m_armMaterial;
  QColor m_color;
  bool m_enabled = true;
  hand m_side;
//  QVector3D m_handPosition;

  Qt3DCore::QTransform *m_globalArmTransform;

  // shoulder
  QEntity *m_shoulderEntity;
  QSphereMesh *m_shoulder;
  Qt3DCore::QTransform *m_shoulderTransform;

  // arm
  QEntity *m_armEntity;
  QCylinderMesh *m_arm;
  Qt3DCore::QTransform *m_armTransform;

  // elbow
  QEntity *m_elbowEntity;
  QSphereMesh *m_elbow;
  Qt3DCore::QTransform *m_elbowTransform;

  // forearm
  QEntity *m_forearmEntity;
  QCylinderMesh *m_forearm;
  Qt3DCore::QTransform *m_forearmTransform;

};

#endif // JUGGLERARM_H
