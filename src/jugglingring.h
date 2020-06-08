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

#ifndef JUGGLINGRING_H
#define JUGGLINGRING_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;
using namespace Qt3DAnimation;



class JugglingRing: public QEntity
{
  Q_OBJECT
public:
  JugglingRing(QEntity *aRootEntity,
               QTorusMesh *torusMesh,
//               QEffect *aEffect,
               QColor aColor);

private:
//  QMaterial *ringMaterial;
//  QParameter *diffuseColorParameter;
//  QParameter *shininessParameter;
  QMetalRoughMaterial *ringMetalRoughMaterial;
  Qt3DCore::QTransform *ringTransform;

  QColor color;

  bool enabled = true;

};

#endif // JUGGLINGRING_H
