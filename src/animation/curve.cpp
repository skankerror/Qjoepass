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
#include <QtMath>

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
                                             int &t_frameCount)
{

  QVector<QVector3D> retVector;
  QVector3D pos = t_startPos;
  retVector.append(pos);
  float deltaAngles = (float)(180 / t_frameCount);
  QVector3D centerCurve = (t_startPos + t_endPos) / 2;
  float rotY = - qRadiansToDegrees(qAtan2(t_startPos.z() - t_endPos.z(),
                                            t_startPos.x() - t_endPos.x()));
  QVector3D axisCurve = QVector3D(0, 0, 1);
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(rotY, QVector3D(0, 1, 0));
  axisCurve = rotAxis * axisCurve;

  for (int i = 0; i <= t_frameCount; i++)
  {
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.translate(centerCurve);
    rot.rotate(-deltaAngles, axisCurve);
    rot.translate(-centerCurve);
    QVector3D nextPos = rot * pos;
    retVector.append(nextPos);
    pos = nextPos;
  }
  return retVector;
}

QVector<QVector3D> Curves::curveTwoQuarterCircular(QVector3D &t_startPos,
                                                   QVector3D &t_midPos,
                                                   QVector3D &t_endPos,
                                                   int &t_frameCount)
{
  QVector<QVector3D> retVector;
  QVector3D pos = t_startPos;
  retVector.append(pos);
  float deltaAngles = (float)(180 / t_frameCount);
  QVector3D centerCurve1(t_midPos.x(), t_startPos.y(), t_midPos.z());
  QVector3D axisCurve1 = QVector3D(0, 0, 1);
  float curve1RotY = - qRadiansToDegrees(qAtan2(t_startPos.z() - t_midPos.z(),
                                       t_startPos.x() - t_midPos.x()));
  QMatrix4x4 rotAxis1;
  rotAxis1.setToIdentity();
  rotAxis1.rotate(curve1RotY, QVector3D(0, 1, 0));
  axisCurve1 = rotAxis1 * axisCurve1;
  // faire la boucle
  // TODO: vérifier le temps, on risque d'avoir une frame de dif ?
  for (int i = 0; i < t_frameCount/2; i++)
  {
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.translate(centerCurve1);
    rot.rotate(-deltaAngles, axisCurve1);
    rot.translate(-centerCurve1);
    QVector3D nextPos = rot * pos;
    retVector.append(nextPos);
    pos = nextPos;
  }

  pos = t_midPos;
  QVector3D centerCurve2(t_midPos.x(), t_endPos.y(), t_midPos.z());
  QVector3D axisCurve2 = QVector3D(0, 0, 1);
  float curve2RotY = - qRadiansToDegrees(qAtan2(t_midPos.z() - t_endPos.z(),
                                              t_midPos.x() - t_endPos.z()));
  QMatrix4x4 rotAxis2;
  rotAxis2.setToIdentity();
  rotAxis2.rotate(curve2RotY, QVector3D(0, 1, 0));
  axisCurve2 = rotAxis2 * axisCurve2;
  // faire la boucle
  for (int i = t_frameCount/2; i < t_frameCount; i++)
  {
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.translate(centerCurve2);
    rot.rotate(-deltaAngles, axisCurve2);
    rot.translate(-centerCurve2);
    QVector3D nextPos = rot * pos;
    retVector.append(nextPos);
    pos = nextPos;
  }
  qDebug() << "in curve factory" << "frameCount" << t_frameCount << "size of return" << retVector.size();
  return retVector;
}
