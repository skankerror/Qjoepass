#include "my3dwindow.h"
#include <QPropertyAnimation>
#include <QDebug>

My3DWindow::My3DWindow(MySettings *aSettings)
  :rootEntity(new QEntity()),
    pointLight(new QPointLight()),
//    skeletonMesh(new QMesh()),
    pirouetteMesh(new QMesh()),
    sphereMesh(new QSphereMesh()),
    torusMesh(new QTorusMesh()),
    settings(aSettings),
    anim(new AnimSimple())
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

/**************************** testing zone ***************************/

  // create 1 juggler for testing purpose
  createJuggler(0, QVector2D(0, 0), QColor(QRgb(0x10561B)));

}

void My3DWindow::createCam()
{
  //  m_camera = new Qt3DRender::QCamera();
  m_camera = camera();
  m_camera->lens()->setPerspectiveProjection(60.0f, 4.0f/3.0f, 0.1f, 1000.0f);
  m_camera->setPosition(QVector3D(0, 10, 15));
  m_camera->setUpVector(QVector3D(0, 1, 0));
  m_camera->setViewCenter(QVector3D(0, 0, 0));
  // For camera controls
  camFPController = new QFirstPersonCameraController(rootEntity);
  camOController = new QOrbitCameraController(rootEntity);
  camOController->setCamera(m_camera);
}

void My3DWindow::createGround()
{
  QColor colorG = settings->value("world/groundColor").value<QColor>();
  ground = new Ground(rootEntity, effect, colorG);
}

void My3DWindow::setGlobalObject()
{
  // global material
  material = new QDiffuseSpecularMaterial(rootEntity);
  effect = material->effect();

  // create one pointlight for 3 sources
  pointLight->setColor(QColor(QRgb(LIGHT_COLOR)));
  pointLight->setIntensity(LIGHT_INTENSITY);

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

void My3DWindow::changeGroundColor(QColor aColor)
{
  ground->setColor(aColor);
}

void My3DWindow::createJuggler(float aRoty, QVector2D aPosition, QColor aColor)
{
  auto juggler = new Juggler(rootEntity, effect, aRoty, aPosition, aColor);
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

void My3DWindow::createSiteSwap(QVector<int> aVecInt, jugglingProp aPropType, bool someSynchron)
{
  SiteSwap *siteSwap = new SiteSwap(aVecInt, someSynchron, this);
  if (!(siteSwap->isValid()))
  {
    qDebug() << "siteswap is not valid !";
    return;
  }
  anim->stopAnimation();

  if (vBall.size())
  {
    for (int i = 0; i < vBall.size(); i++)
    {
      vBall.at(i)->setEnabled(false);
    }
  }
  vBall.clear();
  vBall.squeeze();
  vRing.clear();
  vRing.squeeze();
  vPirouette.clear();
  vPirouette.squeeze();

  int numProp = siteSwap->getNumProp();
  for (int i = 0; i < numProp; i++)
  {
    switch(aPropType)
    {
    case ball: createBall(QColor(QRgb(0xA3A600))); break;
    case ring: createRing(QColor(QRgb(0xA3A600))); break;
    case club: createPirouette(QColor(QRgb(0xA3A600))); break;
    default: break;
    }
  }
  switch(aPropType)
  {
  case ball:
    anim->setJuggler(vJuggler.at(0));
    anim->setVBall(vBall);
    anim->setSiteSwap(siteSwap);
    anim->setAnim();
    anim->startAnimation();
    break;
  case ring:
    break;
  case club:
    break;
  default: break;
  }
}

