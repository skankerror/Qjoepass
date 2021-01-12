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

#include "curve.h"

Curves::Curves()
{}

Curves::~Curves()
{}

QVector<QVector3D> Curves::curveParabolic(QVector3D &t_velocity,
                                            QVector3D &t_startPos,
                                            int &t_frameCount)
{
  QVector<QVector3D> retVector;
  retVector.append(t_startPos);
  for (int i = 0; i <= t_frameCount; i++)
  {
    t_startPos += DELTA_TIME * t_velocity;
    retVector.append(t_startPos);
    t_velocity += DELTA_TIME * GRAVITY;
  }
  return retVector;
}

QVector<QVector3D> Curves::curveSemiCircular(QVector3D &t_startPos,
                                             QVector3D &t_endPos,
                                             float &t_rotY,
                                             hand &t_hand,
                                             int &t_frameCount)
{
  QVector<QVector3D> retVector;
  QVector3D pos = t_startPos;
  retVector.append(pos);
  float deltaAngles = (float)(180 / t_frameCount);
  QVector3D centerCurve = (t_startPos + t_endPos) / 2;
  QVector3D axisCurve = QVector3D(0, 0, 1);
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(t_rotY, QVector3D(0, 1, 0));
  axisCurve = rotAxis * axisCurve;

  for (int i = 0; i <= t_frameCount; i++)
  {
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.translate(centerCurve);
    if (t_hand == leftHand)
      rot.rotate(-deltaAngles, axisCurve);
    else
      rot.rotate(deltaAngles, axisCurve);
    rot.translate(-centerCurve);
    QVector3D nextPos = rot * pos;
    retVector.append(nextPos);
    pos = nextPos;
  }
  return retVector;
}
