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
#include "animationfactory.h"

using namespace Qt3DCore;
using namespace Qt3DRender;
using namespace Qt3DExtras;
//using namespace Qt3DInput;
using namespace Qt3DAnimation;


class My3DWindow: public Qt3DWindow
{
  Q_OBJECT
public:
  My3DWindow(MySettings *aSettings);

private:
  void createCam();
  void createGround();

public slots:
  void changeBackground(QColor aColor);
  void createJuggler(float aRoty, QVector2D aPosition, QColor aColor);
  void createLight(QVector3D aPosition, QColor aColor, float aIntensity);
  void createPirouette(QColor aColor);
  void createBall(QColor aColor);
  void createRing(QColor aColor);
  void upateBallAnim(QAnimationClipData clipData);

private:
  QEntity *rootEntity;
//  QRenderSettings *renderSettings;
//  QForwardRenderer *forwardRenderer;
//  Qt3DInput::QInputSettings *inputSettings;

  QCamera *m_camera;
  QFirstPersonCameraController *camFPController;
  QOrbitCameraController *camOController;

  Ground *ground;
  QVector<Light *> vLight;
  QVector<Juggler *> vJuggler;
  QVector<Pirouette *> vPirouette;
  QVector<JugglingBall *> vBall;
  QVector<JugglingRing *> vRing;

  MySettings *settings;

  AnimationFactory *animation;
};

#endif // MY3DWINDOW_H
