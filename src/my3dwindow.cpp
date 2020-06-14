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

#include "my3dwindow.h"
#include <QDebug>

My3DWindow::My3DWindow(MySettings *aSettings)
  : rootEntity(new QEntity()),
    skybox(new QSkyboxEntity()),
    pointLight(new QPointLight()),
    pirouetteMesh(new QMesh()),
    sphereMesh(new QSphereMesh()),
    torusMesh(new QTorusMesh()),
    settings(aSettings),
    anim(new MyAnimation())
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
  createSkybox();

/**************************** testing zone ***************************/

  // create 1 juggler for testing purpose
  createJuggler(45, QVector2D(-2, -2), QColor(QRgb(0xFF0000)));
}

void My3DWindow::createCam()
{
  m_camera = camera();
  m_camera->lens()->setPerspectiveProjection(CAM_FOV, CAM_RATIO, CAM_NEARPLANE, CAM_FARPLANE);
  positionCamera = CAM_INITIAL_POSITION;
  camFPController = new QFirstPersonCameraController(rootEntity);
  camOController = new QOrbitCameraController(rootEntity);
  setCameraToOrbit();
}

void My3DWindow::createGround()
{
  QColor colorG = settings->value("world/groundColor").value<QColor>();
  ground = new Ground(rootEntity, colorG);
}

void My3DWindow::setGlobalObject()
{
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

int My3DWindow::getJugglerCount() const
{
  return vJuggler.size();
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
  auto juggler = new Juggler(rootEntity, aRoty, aPosition, aColor);
  vJuggler.append(juggler);
  emit jugglerCountChanged();
}

void My3DWindow::createSkybox()
{
  skybox->setBaseName(QStringLiteral(SKYBOX_BASE_NAME));
  skybox->setExtension(QStringLiteral(SKYBOX_EXTENSION));

  Qt3DCore::QTransform * skyTransform = new Qt3DCore::QTransform(skybox);
  skyTransform->setScale3D(QVector3D( SKYBOX_SCALE, SKYBOX_SCALE, SKYBOX_SCALE));
  skybox->addComponent(skyTransform);
  skybox->setParent(rootEntity);
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

  envLight = new QEnvironmentLight();

  auto irradiance = new QTextureLoader();
  irradiance->setSource(QUrl(QStringLiteral(IRRADIANCE)));
  irradiance->setWrapMode(QTextureWrapMode());
  irradiance->setGenerateMipMaps(false);
  envLight->setIrradiance(irradiance);

  auto specular = new QTextureLoader();
  specular->setSource(QUrl(QStringLiteral(SPECULAR)));
  specular->setWrapMode(QTextureWrapMode());
  specular->setGenerateMipMaps(false);
  envLight->setSpecular(specular);

  envLight->setParent(rootEntity);
}

void My3DWindow::createPirouette(QColor aColor)
{
  auto pirouette = new Pirouette(rootEntity, pirouetteMesh, aColor);
  vPirouette.append(pirouette);
}

void My3DWindow::createBall(QColor aColor)
{
  auto ball = new JugglingBall(rootEntity, sphereMesh, aColor);
  vBall.append(ball);
}

void My3DWindow::createRing(QColor aColor)
{
  auto ring = new JugglingRing(rootEntity, torusMesh, aColor);
  vRing.append(ring);
}

void My3DWindow::createSiteSwap(QVector<int> aVecInt, jugglingProp aPropType, int launchType, bool someSynchron)
{
  SiteSwap *siteSwap = new SiteSwap(aVecInt, aPropType, someSynchron, this);
  if (!(siteSwap->isValid()))
  {
    qDebug() << "siteswap is not valid !";
    return;
  }
  anim->stop();
  anim->clear();

  if (vBall.size())
    for (int i = 0; i < vBall.size(); i++)
      vBall.at(i)->setEnabled(false);

  if (vRing.size())
    for (int i = 0; i < vRing.size(); i++)
      vRing.at(i)->setEnabled(false);

  if (vPirouette.size())
    for (int i = 0; i < vPirouette.size(); i++)
      vPirouette.at(i)->setEnabled(false);

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
    case ball:
      createBall(QColor(QRgb(0xA3A600)));
      vBall.at(i)->setLaunchType((launchTypeBall)(launchType));
      break;
    case ring:
      createRing(QColor(QRgb(0xA3A600)));
      vRing.at(i)->setLaunchType((launchTypeRing)(launchType));
      break;
    case club:
      createPirouette(QColor(QRgb(0xA3A600)));
      vPirouette.at(i)->setLaunchType((launchTypeClub)(launchType));
      break;
    default: break;
    }
  }

  siteSwap->setLaunchType(launchType); // it simplifies our animation

  anim->setJuggler(vJuggler.at(0));
  anim->setVBall(vBall);
  anim->setVRing(vRing);
  anim->setVClub(vPirouette);
  anim->setSiteSwap(siteSwap);
  anim->setAnim();
  anim->start();
}

void My3DWindow::setCameraToOrbit()
{
  m_camera->setPosition(positionCamera);
  m_camera->setUpVector(CAM_UP_VECTOR);
  m_camera->setViewCenter(CAM_ORBIT_VIEW_CENTER);
  camOController->setCamera(m_camera);
}

void My3DWindow::setCameraToFirstPers(int index)
{
  if (index < 0 || index >= vJuggler.size())
    return;
  QVector3D headPos = vJuggler.at(index)->getPositionHead();
  m_camera->setPosition(headPos);
  m_camera->setUpVector(CAM_UP_VECTOR);
  QVector3D lookAt = vJuggler.at(index)->getLookAt();
  m_camera->setViewCenter(lookAt);
  camFPController->setCamera(m_camera);
}
