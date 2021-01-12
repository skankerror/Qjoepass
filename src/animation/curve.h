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

#ifndef CURVES_H
#define CURVES_H

#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>
#include "qjoepass.h"


class Curves
{

public:

  Curves();
  ~Curves();

  static QVector<QVector3D> curveParabolic(QVector3D &t_velocity,
                                           QVector3D &t_startPos,
                                           int &t_frameCount);

  static QVector<QVector3D> curveSemiCircular(QVector3D &t_startPos,
                                              QVector3D &t_endPos,
                                              float &t_rotY, // juggler pos
                                              hand &t_hand,
                                              int &t_frameCount);

};

#endif // CURVES_H
