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

#ifndef LIGHT_H
#define LIGHT_H

#include <Qt3DCore>
#include <Qt3DRender>
#include "qjoepass.h"

class Light: public Qt3DCore::QEntity
{

  Q_OBJECT

public:

  explicit Light(QEntity *t_rootEntity,
                 Qt3DRender::QPointLight *t_light,
                 QVector3D t_position);

private:

  Qt3DCore::QTransform *m_lightTransform;
  QEntity *m_rootEntity;
  Qt3DRender::QPointLight *m_light;
  QVector3D m_position;

  bool m_enabled = true;
};

#endif // LIGHT_H
