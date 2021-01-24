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

#ifndef JUGGLINGPROP_H
#define JUGGLINGPROP_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;

class JugglingProp : public QEntity
{

  Q_OBJECT

  Q_PROPERTY(QVector3D m_position
             READ getPosition
             WRITE setPosition
             NOTIFY positionChanged)

public:

  JugglingProp(QEntity *t_rootEntity,
               QColor &t_color);

  QVector3D getPosition() const { return m_position; };

public slots:

  void setPosition(QVector3D t_position);

signals:

  void positionChanged(QVector3D t_position);

protected:

  void updateTransform();

protected:

  QMetalRoughMaterial *m_propMetalRoughMaterial;
  Qt3DCore::QTransform *m_propTransform;
  QVector3D m_position;
  QColor m_color;
  bool m_enabled = true;
};

#endif // JUGGLINGPROP_H
