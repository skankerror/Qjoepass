#include "my3dwindow.h"

My3DWindow::My3DWindow(MySettings *aSettings)
  :settings(aSettings)
{
  //background
  QColor colorBG = settings->value("world/colorbg").value<QColor>();
  defaultFrameGraph()->setClearColor(colorBG);

  // Root entity, root object of the scene
  rootEntity = new Qt3DCore::QEntity();
  setRootEntity(rootEntity);

  createCam();

  createGround();

  // create 3 lights for a macCandless style lighting !
  createLight(QVector3D(-20, 10, 20), QColor(QRgb(0xFFEFE1)), 2);
  createLight(QVector3D(20, 10, 20), QColor(QRgb(0xD0E1FF)), 1);
  createLight(QVector3D(0, 10, -20), QColor(QRgb(0xFFFFFF)), 2);

  // create 2 jugglers for testing purpose
  createJuggler(90, QVector2D(-7, 0), QColor(QRgb(0x204C9B)));
  createJuggler(-90, QVector2D(7, 0), QColor(QRgb(0x10561B)));
  // create 1 pirouette for testing purpose
  createPirouette(QColor(QRgb(0xA3A600)));
  vPirouette[0]->setPosition(QVector3D(0, -4, 0));
  // create 1 ball for testing purpose
  createBall(QColor(QRgb(0xA3A600)));
  // create 1 ring for testing purpose
  createRing(QColor(QRgb(0xA3A600)));

}

void My3DWindow::createCam()
{
  m_camera = camera();
  m_camera->lens()->setPerspectiveProjection(45.0f, 4.0f/3.0f, 0.1f, 1000.0f);
  m_camera->setPosition(QVector3D(0, 0, 20.0f));
  m_camera->setUpVector(QVector3D(0, 1, 0));
  m_camera->setViewCenter(QVector3D(0, 0, 0));
  // For camera controls
  camFPController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
  camOController = new Qt3DExtras::QOrbitCameraController(rootEntity);
  camOController->setCamera(m_camera);
}

void My3DWindow::createGround()
{
  QColor color(QColor(QRgb(0x301510))); // Ground color
  ground = new Ground(rootEntity, color);
}

void My3DWindow::changeBackground(QColor aColor)
{
  defaultFrameGraph()->setClearColor(aColor);
}

void My3DWindow::createJuggler(float aRoty, QVector2D aPosition, QColor aColor)
{
  auto juggler = new Juggler(rootEntity, aRoty, aPosition, aColor);
  vJuggler.append(juggler);
}

void My3DWindow::createLight(QVector3D aPosition, QColor aColor, float aIntensity)
{
  auto light = new Light(rootEntity, aPosition, aColor, aIntensity);
  vLight.append(light);
}

void My3DWindow::createPirouette(QColor aColor)
{
  Pirouette *pirouette = new Pirouette(rootEntity, aColor);
  vPirouette.append(pirouette);
}

void My3DWindow::createBall(QColor aColor)
{
  JugglingBall *ball = new JugglingBall(rootEntity, aColor);
  vBall.append(ball);
}

void My3DWindow::createRing(QColor aColor)
{
  auto ring = new JugglingRing(rootEntity, aColor);
  vRing.append(ring);
}
