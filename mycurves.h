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

#ifndef MYCURVES_H
#define MYCURVES_H

#include <QVector>
#include <QVector3D>

class MyCurves
{
public:
  MyCurves();

  static QVector<QVector3D> curveParabolic(QVector3D &velocity,
                                           QVector3D &startPos,
                                           int &frameCount);

};

#endif // MYCURVES_H
