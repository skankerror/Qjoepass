#include "my3dwindow.h"

My3DWindow::My3DWindow(MySettings *aSettings)
  :rootEntity(new QEntity()),
    pointLight(new QPointLight()),
    skeleton(new QSkeletonLoader()),
    skeletonMesh(new QMesh()),
    pirouetteMesh(new QMesh()),
    sphereMesh(new QSphereMesh()),
    torusMesh(new QTorusMesh()),
    settings(aSettings)
{
  // Root entity, root object of the scene
  setRootEntity(rootEntity);

  //background
  QColor colorBG = settings->value("world/colorbg").value<QColor>();
  defaultFrameGraph()->setClearColor(colorBG);

  setGlobalObject();
  createCam();
  createGround();
  createLighting();

  // create 2 jugglers for testing purpose
  createJuggler(90, QVector2D(-7, 0), QColor(QRgb(0x204C9B)));
  createJuggler(-90, QVector2D(7, 0), QColor(QRgb(0x10561B)));
  // create 1 pirouette for testing purpose
  createPirouette(QColor(QRgb(0xA3A600)));
  vPirouette.at(0)->setPosition(QVector3D(0, -4, 0));
  // create 1 ball for testing purpose
  createBall(QColor(QRgb(0xA3A600)));
  // create 1 ring for testing purpose
  createRing(QColor(QRgb(0xA3A600)));

}

void My3DWindow::createCam()
{
  //  m_camera = new Qt3DRender::QCamera();
  m_camera = camera();
  m_camera->lens()->setPerspectiveProjection(45.0f, 4.0f/3.0f, 0.1f, 1000.0f);
  m_camera->setPosition(QVector3D(0, 10, 25));
  m_camera->setUpVector(QVector3D(0, 1, 0));
  m_camera->setViewCenter(QVector3D(0, 0, 0));
  // For camera controls
  camFPController = new QFirstPersonCameraController(rootEntity);
  camOController = new QOrbitCameraController(rootEntity);
  camOController->setCamera(m_camera);
}

void My3DWindow::createGround()
{
  QColor color(QColor(QRgb(0x301510))); // Ground color
  ground = new Ground(rootEntity, effect, color);
}

void My3DWindow::setGlobalObject()
{
  // global material
  material = new QDiffuseSpecularMaterial(rootEntity);
  effect = material->effect();

  // create one pointlight for 3 sources
  pointLight->setColor(QColor(QRgb(LIGHT_COLOR)));
  pointLight->setIntensity(LIGHT_INTENSITY);

  // For jugglers creations
  skeleton->setSource(QUrl(SKELETON_SRC));
  skeletonMesh->setSource(QUrl(SKELETON_MESH_SRC));

  // For ball creations
  sphereMesh->setRings(BALL_RINGS);
  sphereMesh->setSlices(BALL_SLICES);
  sphereMesh->setRadius(BALL_RADIUS);

  // For ring creations
  torusMesh->setRadius(RING_RADIUS);
  torusMesh->setMinorRadius(RING_MINOR_RADIUS);
  torusMesh->setRings(RING_RING_NUMBER);
  torusMesh->setSlices(RING_SLICE_NUMBER);

  // For club creations
  pirouetteMesh->setSource(QUrl(CLUB_MESH_SRC));
}

void My3DWindow::changeBackground(QColor aColor)
{
  defaultFrameGraph()->setClearColor(aColor);
}

void My3DWindow::createJuggler(float aRoty, QVector2D aPosition, QColor aColor)
{
  auto juggler = new Juggler(rootEntity, skeleton, skeletonMesh, effect, aRoty, aPosition, aColor);
  vJuggler.append(juggler);
}

void My3DWindow::createLighting()
{
  QVector3D pos1 = QVector3D(-20, 10, 20);
  auto light = new Light(rootEntity, pointLight, pos1);
  vLight.append(light);

  QVector3D pos2 = QVector3D(20, 10, 20);
  auto light2 = new Light(rootEntity, pointLight, pos2);
  vLight.append(light2);

  QVector3D pos3 = QVector3D(0, 10, -20);
  auto light3 = new Light(rootEntity, pointLight, pos3);
  vLight.append(light3);
}

void My3DWindow::createPirouette(QColor aColor)
{
  auto pirouette = new Pirouette(rootEntity, pirouetteMesh, effect, aColor);
  vPirouette.append(pirouette);
}

void My3DWindow::createBall(QColor aColor)
{
  auto ball = new JugglingBall(rootEntity, sphereMesh, effect, aColor);
  vBall.append(ball);
}

void My3DWindow::createRing(QColor aColor)
{
  auto ring = new JugglingRing(rootEntity, torusMesh, effect, aColor);
  vRing.append(ring);
}

