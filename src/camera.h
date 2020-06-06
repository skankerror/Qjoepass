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

#ifndef CAMERA_H
#define CAMERA_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

// marche pas, il faudra voir si on met un objet renderer

class Camera : public Qt3DRender::QCamera
{
  Q_OBJECT
public:
  Camera(/*Qt3DRender::QCamera *aQCamera,*/
         Qt3DCore::QEntity *aRootEntity,
         QVector4D aPerspProj = QVector4D(45.0f, 4.0f/3.0f, 0.1f, 1000.0f),
         QVector3D aPosition = QVector3D(0, 0, 20),
         QVector3D aSetup = QVector3D(0, 1, 0),
         QVector3D aViewCenter = QVector3D(0, 0, 0),
         bool isFirstPersonCam = true);

private:
  Qt3DCore::QEntity *rootEntity;

  QVector4D perspProj; // 45.0f, 4.0f/3.0f, 0.1f, 1000.0f
  QVector3D position; //0, 0, 20.0f
  QVector3D setUp;//0, 1, 0
  QVector3D viewCenter;//0, 0, 0
  Qt3DExtras::QFirstPersonCameraController *camFPController;
  Qt3DExtras::QOrbitCameraController *camOController;

  bool isFirstPersonCamera;
};

#endif // CAMERA_H
