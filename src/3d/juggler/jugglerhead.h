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

#ifndef JUGGLERHEAD_H
#define JUGGLERHEAD_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include "qjoepass.h"

class JugglerHead : public Qt3DCore::QEntity
{

  Q_OBJECT

public:

  JugglerHead(QEntity *t_rootEntity,
              Qt3DExtras::QMetalRoughMaterial *t_jugglerMetalRoughMaterial,
              QColor &t_color);

  void setHeadRotationX(float t_angle);
  void setHeadRotationY(float t_angle);

private:

  Qt3DExtras::QMetalRoughMaterial *m_headMaterial;
  QColor m_color;
  bool m_enabled = true;

  Qt3DCore::QTransform *m_globalHeadTransform;

  // last vertebrate
  QEntity *m_lastVertebrateEntity;
  Qt3DCore::QTransform *m_lastVertebrateTransform;

  // neck
  QEntity *m_neckEntity;

  // skull
  QEntity *m_skullEntity;

  // eyes
  QEntity *m_leftEyeEntity;
  QEntity *m_rightEyeEntity;

};

#endif // JUGGLERHEAD_H
