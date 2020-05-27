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
