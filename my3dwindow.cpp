#include "my3dwindow.h"

My3DWindow::My3DWindow(MySettings *aSettings)
  :settings(aSettings)
{
  //background
  QColor colorBG = settings->value("world/colorbg").value<QColor>();
  defaultFrameGraph()->setClearColor(colorBG);

  // Root entity
  rootEntity = new Qt3DCore::QEntity();

  createCam();

  createLight(QVector3D(-20, 10, 20), QColor(QRgb(0xFFEFE1)), 2);
  createLight(QVector3D(20, 10, 20), QColor(QRgb(0xD0E1FF)), 1);
  createLight(QVector3D(0, 10, -20), QColor(QRgb(0xFFFFFF)), 2);

  // Set root object of the scene
  setRootEntity(rootEntity);

  QColor color(QColor(QRgb(0x301510))); // Ground color
  ground = new Ground(rootEntity, color);

  createJuggler(90, QVector2D(-7, 0), QColor(QRgb(0x204C9B)));
  createJuggler(-90, QVector2D(7, 0), QColor(QRgb(0x10561B)));
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
