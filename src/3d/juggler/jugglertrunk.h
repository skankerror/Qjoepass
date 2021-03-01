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

#ifndef JUGGLERTRUNK_H
#define JUGGLERTRUNK_H

#include <Qt3DCore>
#include <Qt3DExtras>
//#include "qjoepass.h"
#include "jugglerpart.h"


class JugglerTrunk : public JugglerPart
{

  Q_OBJECT

public:

  JugglerTrunk(QEntity *t_rootEntity,
               Qt3DExtras::QMetalRoughMaterial *t_jugglerMetalRoughMaterial,
               QColor &t_color);


  QEntity *getClaviclesEntity() const { return m_claviclesEntity; }
  QEntity *getPelvisEntity() const { return m_pelvisEntity; }

  void setPelvisRotationX(float t_angle) { m_pelvisTransform->setRotationX(t_angle); }
  void setPelvisRotationY(float t_angle) { m_pelvisTransform->setRotationY(t_angle); }
  void setClaviculesRotationY(float t_angle) { m_claviclesTransform->setRotationX(t_angle); }

private:

  // pelvis
  QEntity *m_pelvisEntity;
  Qt3DCore::QTransform *m_pelvisTransform;

  // spine
  QEntity *m_spineEntity;

  // clavicles
  QEntity *m_claviclesEntity;
  Qt3DCore::QTransform *m_claviclesTransform;
};

#endif // JUGGLERTRUNK_H
