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

#ifndef PIROUETTE_H
#define PIROUETTE_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
//using namespace Qt3DAnimation;
using namespace Qt3DRender;

class Pirouette: public QEntity
{
  Q_OBJECT
public:
  Pirouette(QEntity *aRootEntity,
            QMesh *aPirouetteMesh,
            QEffect *aEffect,
            QColor aColor);

  void setPosition(QVector3D aPosition);

private:
  QMaterial *pirouetteMaterial;
  QParameter *diffuseColorParameter;
  QParameter *shininessParameter;
  Qt3DCore::QTransform *pirouetteTransform;

  QColor color;

  QVector3D position;

  bool enabled = true;
};

#endif // PIROUETTE_H
