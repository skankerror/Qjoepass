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

#include "camera.h"


Camera::Camera(
    //    Qt3DRender::QCamera *aQCamera,
    Qt3DCore::QEntity *aRootEntity,
    QVector4D aPerspProj,
    QVector3D aPosition,
    QVector3D aSetup,
    QVector3D aViewCenter,
    bool isFirstPersonCam):
  Qt3DRender::QCamera(),
  rootEntity(aRootEntity),
  perspProj(aPerspProj),
  position(aPosition),
  setUp(aSetup),
  viewCenter(aViewCenter),
  isFirstPersonCamera(isFirstPersonCam)
{
  lens()->setPerspectiveProjection(perspProj.x(),
                                   perspProj.y(),
                                   perspProj.z(),
                                   perspProj.w());
  setPosition(position);
  setUpVector(setUp);
  setViewCenter(viewCenter);
  if(isFirstPersonCamera)
  {
    camFPController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camFPController->setCamera(this);
  }
  else
  {
    camOController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camOController->setCamera(this);
  }
}
