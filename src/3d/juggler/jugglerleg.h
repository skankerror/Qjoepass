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

#ifndef JUGGLERLEG_H
#define JUGGLERLEG_H

#include <Qt3DCore>
#include <Qt3DExtras>
//#include "qjoepass.h"
#include "jugglerpart.h"


class JugglerLeg : public JugglerPart
{

  Q_OBJECT

public:

  JugglerLeg(QEntity *t_rootEntity,
             Qt3DExtras::QMetalRoughMaterial *t_jugglerMetalRoughMaterial,
             QColor &t_color,
             hand t_side);

  void setHaunchRotationX(float t_angle);
  void setHaunchRotationY(float t_angle);
  void setKneeRotationX(float t_angle);

private:

  hand m_side;

  // haunch
  QEntity *m_haunchEntity;
  Qt3DCore::QTransform *m_haunchTransform;

  // thigh
  QEntity *m_thighEntity;

  // knee
  QEntity *m_kneeEntity;
  Qt3DCore::QTransform *m_kneeTransform;

  // tibia
  QEntity *m_tibiaEntity;

};

#endif // JUGGLERLEG_H
