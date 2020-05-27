#ifndef MY3DWINDOW_H
#define MY3DWINDOW_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DInput>
#include <Qt3DLogic>
#include <Qt3DExtras>
#include <Qt3DAnimation>
//#include <Kuesa>
#include "settings.h"

#define SKELETON_MODEL "/home/ray/dev/Qjoepass/mesh/RiggedFigure.gltf"
#define JUGGLER_MODEL "mesh/RiggedFigure.gltf"

class My3DWindow: public Qt3DExtras::Qt3DWindow
{
  Q_OBJECT
public:
  My3DWindow(MySettings *aSettings);

public slots:
  void changeBackground(QColor aColor);

private:
  Qt3DCore::QEntity *rootEntity;

  Qt3DRender::QCamera *m_camera;
  Qt3DExtras::QFirstPersonCameraController *camFPController;
  Qt3DExtras::QOrbitCameraController *camOController;

  Qt3DCore::QEntity *lightEntity;
  Qt3DRender::QPointLight *light;
  Qt3DCore::QTransform *lightTransform;

  Qt3DCore::QEntity *lightEntity2;
  Qt3DRender::QPointLight *light2;
  Qt3DCore::QTransform *lightTransform2;

  Qt3DCore::QEntity *lightEntity3;
  Qt3DRender::QPointLight *light3;
  Qt3DCore::QTransform *lightTransform3;

  Qt3DExtras::QPlaneMesh *planeMesh;
  Qt3DCore::QTransform *planeTransform;
  Qt3DExtras::QPhongMaterial *planeMaterial;
  Qt3DCore::QEntity *planeEntity;

  Qt3DCore::QEntity *skeletonEntity;
  Qt3DCore::QSkeletonLoader *skeleton;
  Qt3DRender::QMesh *skeletonMesh;
  Qt3DExtras::QPhongMaterial *skeletonMaterial;
  Qt3DCore::QTransform *skeletonTransform;
  Qt3DCore::QArmature *skeletonArmature;

  Qt3DCore::QEntity *skeletonEntity2;
  Qt3DCore::QSkeletonLoader *skeleton2;
  Qt3DRender::QMesh *skeletonMesh2;
  Qt3DExtras::QPhongMaterial *skeletonMaterial2;
  Qt3DCore::QTransform *skeletonTransform2;
  Qt3DCore::QArmature *skeletonArmature2;

  MySettings *settings;

  bool enabled = true;
};

#endif // MY3DWINDOW_H
