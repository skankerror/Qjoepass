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

#ifndef GROUND_H
#define GROUND_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;

class Ground: public QEntity
{

  Q_OBJECT

public:

  explicit Ground(QEntity *t_rootEntity,
                  QColor &t_color);

  QColor getColor() const {return color;};
  void setColor(QColor t_color);

private:

  QPlaneMesh *planeMesh;
  Qt3DCore::QTransform *planeTransform;

  QColor color;
  bool enabled = true;

  QMetalRoughMaterial *groundMetalRoughnessMaterial;
  QTextureLoader *groundBaseColor;
  QTextureLoader *groundMetalness;
  QTextureLoader *groundRoughness;
  QTextureLoader *groundNormal;
  QTextureLoader *groundAmbientOcclusion;
};

#endif // GROUND_H