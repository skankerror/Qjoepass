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
