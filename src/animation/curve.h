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

  // for translation in the air
  static QVector<QVector3D> curveParabolic(const QVector3D &t_velocity,
                                           const QVector3D &t_startPos,
                                           int &t_frameCount);

  // for self dwell
  static QVector<QVector3D> curveSemiCircular(const QVector3D &t_startPos,
                                              const QVector3D &t_endPos,
                                              int &t_frameCount);

  // for passing dwell
  static QVector<QVector3D> curveTwoQuarterCircular(const QVector3D &t_startPos,
                                                    const QVector3D &t_midPos,
                                                    const QVector3D &t_endPos,
                                                    int &t_frameCount);

};

#endif // CURVES_H
