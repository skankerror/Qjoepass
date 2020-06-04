#ifndef MY3DWINDOW_H
#define MY3DWINDOW_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DInput>
#include <Qt3DLogic>
#include <Qt3DExtras>
#include <Qt3DAnimation>
#include "settings.h"
#include "juggler.h"
#include "light.h"
#include "ground.h"
#include "pirouette.h"
#include "jugglingball.h"
#include "jugglingring.h"
#include "animsimple.h"

using namespace Qt3DCore;
using namespace Qt3DRender;
using namespace Qt3DExtras;
using namespace Qt3DAnimation;


class My3DWindow: public Qt3DWindow
{
  Q_OBJECT
public:
  My3DWindow(MySettings *aSettings);

private:
  void createCam();
  void createGround();
  void setGlobalObject();

public slots:
  void changeBackground(QColor aColor);
  void changeGroundColor(QColor aColor);
  void createJuggler(float aRoty, QVector2D aPosition, QColor aColor);
  void createLighting();
  void createPirouette(QColor aColor);
  void createBall(QColor aColor);
  void createRing(QColor aColor);

private:
  QEntity *rootEntity;

  // Camera
  QCamera *m_camera;
  QFirstPersonCameraController *camFPController;
  QOrbitCameraController *camOController;

  // Global Material, we create one for the whole scene
  // and pass effect to each 3d object
  QDiffuseSpecularMaterial *material;
  QEffect *effect;

  // Ground
  Ground *ground;

  // light
  QPointLight *pointLight;
  QVector<Light *> vLight;

  // juggler
  QMesh *skeletonMesh;
  QVector<Juggler *> vJuggler;

  // club
  QMesh *pirouetteMesh;
  QVector<Pirouette *> vPirouette;

  // ball
  QSphereMesh *sphereMesh;
  QVector<JugglingBall *> vBall;

  // ring
  QTorusMesh *torusMesh;
  QVector<JugglingRing *> vRing;

  MySettings *settings;

};

#endif // MY3DWINDOW_H
