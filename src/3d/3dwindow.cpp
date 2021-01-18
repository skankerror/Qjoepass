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

#include "3dwindow.h"
#include <QDebug>

My3DWindow::My3DWindow(MySettings *t_settings)
  : Qt3DWindow(nullptr/*, Qt3DRender::API::RHI*/),
    m_rootEntity(new QEntity()),
    m_skybox(new QSkyboxEntity()),
    m_pointLight(new QPointLight()),
    m_pirouetteMesh(new QMesh()),
    m_sphereMesh(new QSphereMesh()),
    m_torusMesh(new QTorusMesh()),
    m_settings(t_settings),
    m_anim(new Animation())
{
  // Root entity, root object of the scene
  setRootEntity(m_rootEntity);

  //background
  QColor colorBG = m_settings->value("world/colorbg").value<QColor>();
  defaultFrameGraph()->setClearColor(colorBG);

  setGlobalObject();
  createCam();
  createGround();
  createLighting();
  createSkybox();

/**************************** testing zone ***************************/

  // create 1 juggler for testing purpose
//  createJuggler(0, QVector2D(0, 0), QColor(QRgb(0xFF0000)));

  // create 2 jugllers for passing testing
  createJuggler(90, QVector2D(-7, 0), QColor(QRgb(0xFF0000)));
//  createJuggler(-90, QVector2D(7, 0), QColor(QRgb(0x00FF00)));
}

void My3DWindow::createCam()
{
  m_camera = camera();
  m_camera->lens()->setPerspectiveProjection(CAM_FOV, CAM_RATIO, CAM_NEARPLANE, CAM_FARPLANE);
  m_positionCamera = CAM_INITIAL_POSITION;
  m_camFPController = new QFirstPersonCameraController(m_rootEntity);
  m_camOController = new QOrbitCameraController(m_rootEntity);
  setCameraToOrbit();
}

void My3DWindow::createGround()
{
  QColor colorG = m_settings->value("world/groundColor").value<QColor>();
  m_ground = new Ground(m_rootEntity, colorG);
}

void My3DWindow::setGlobalObject()
{
  // create one pointlight for 3 sources
  m_pointLight->setColor(QColor(QRgb(LIGHT_COLOR)));
  m_pointLight->setIntensity(LIGHT_INTENSITY);

  // For ball creations
  m_sphereMesh->setRings(BALL_RINGS);
  m_sphereMesh->setSlices(BALL_SLICES);
  m_sphereMesh->setRadius(BALL_RADIUS);

  // For ring creations
  m_torusMesh->setRadius(RING_RADIUS);
  m_torusMesh->setMinorRadius(RING_MINOR_RADIUS);
  m_torusMesh->setRings(RING_RING_NUMBER);
  m_torusMesh->setSlices(RING_SLICE_NUMBER);

  // For club creations
  m_pirouetteMesh->setSource(QUrl(CLUB_MESH_SRC));
}

int My3DWindow::getJugglerCount() const
{
  return m_v_juggler.size();
}

void My3DWindow::changeBackground(QColor t_color)
{
  defaultFrameGraph()->setClearColor(t_color);
}

void My3DWindow::changeGroundColor(QColor t_color)
{
  m_ground->setColor(t_color);
}

void My3DWindow::createJuggler(float t_rotY, QVector2D t_position, QColor t_color)
{
  auto juggler = new Juggler(m_rootEntity, t_rotY, t_position, t_color);
  m_v_juggler.append(juggler);
  emit jugglerCountChanged();
}

void My3DWindow::createSkybox()
{
  m_skybox->setBaseName(QStringLiteral(SKYBOX_BASE_NAME));
  m_skybox->setExtension(QStringLiteral(SKYBOX_EXTENSION));

  Qt3DCore::QTransform * skyTransform = new Qt3DCore::QTransform(m_skybox);
  skyTransform->setScale3D(QVector3D( SKYBOX_SCALE, SKYBOX_SCALE, SKYBOX_SCALE));
  m_skybox->addComponent(skyTransform);
  m_skybox->setParent(m_rootEntity);
}

void My3DWindow::createLighting()
{
  QVector3D pos1 = QVector3D(-20, 10, 20);
  auto light = new Light(m_rootEntity, m_pointLight, pos1);
  m_v_light.append(light);

  QVector3D pos2 = QVector3D(20, 10, 20);
  auto light2 = new Light(m_rootEntity, m_pointLight, pos2);
  m_v_light.append(light2);

  QVector3D pos3 = QVector3D(0, 10, -20);
  auto light3 = new Light(m_rootEntity, m_pointLight, pos3);
  m_v_light.append(light3);

  m_envLight = new QEnvironmentLight();

  auto irradiance = new QTextureLoader();
  irradiance->setSource(QUrl(QStringLiteral(IRRADIANCE)));
  irradiance->setWrapMode(QTextureWrapMode());
  irradiance->setGenerateMipMaps(false);
  m_envLight->setIrradiance(irradiance);

  auto specular = new QTextureLoader();
  specular->setSource(QUrl(QStringLiteral(SPECULAR)));
  specular->setWrapMode(QTextureWrapMode());
  specular->setGenerateMipMaps(false);
  m_envLight->setSpecular(specular);

  m_envLight->setParent(m_rootEntity);
}

void My3DWindow::createPirouette(QColor t_color)
{
  auto pirouette = new Pirouette(m_rootEntity, m_pirouetteMesh, t_color);
  m_v_pirouette.append(pirouette);
}

void My3DWindow::createBall(QColor t_color)
{
  auto ball = new JugglingBall(m_rootEntity, m_sphereMesh, t_color);
  m_v_ball.append(ball);
}

void My3DWindow::createRing(QColor t_color)
{
  auto ring = new JugglingRing(m_rootEntity, m_torusMesh, t_color);
  m_v_ring.append(ring);
}

void My3DWindow::createSiteSwap(QVector<SiteswapEvent*> t_v_Event,
                                int t_jugCount,
                                jugglingProp t_propType,
                                int t_launchType,
                                bool t_synchron)
{
  // create siteswap with values
  auto siteswap = new SiteSwap(t_v_Event, t_jugCount, t_propType, t_synchron, this);
  // test if it's valid
  if (!(siteswap->isValid()))
  {
    qDebug() << "siteswap is not valid !";
    return;
  }
  // it's valid, we stop existing anim
  m_anim->stop();
  m_anim->clear();

  // and destroy any prop if there is
  if (m_v_ball.size())
    for (int i = 0; i < m_v_ball.size(); i++)
      m_v_ball.at(i)->setEnabled(false);

  if (m_v_ring.size())
    for (int i = 0; i < m_v_ring.size(); i++)
      m_v_ring.at(i)->setEnabled(false);

  if (m_v_pirouette.size())
    for (int i = 0; i < m_v_pirouette.size(); i++)
      m_v_pirouette.at(i)->setEnabled(false);

  m_v_ball.clear();
  m_v_ball.squeeze();
  m_v_ring.clear();
  m_v_ring.squeeze();
  m_v_pirouette.clear();
  m_v_pirouette.squeeze();

  // now it's surely clean...
  //NOTE: what about jugglers ?

  // create props
  int numProp = siteswap->getNumProp();
  for (int i = 0; i < numProp; i++)
  {
    switch(t_propType)
    {
    case ball:
      createBall(QColor(QRgb(0xA3A600)));
      m_v_ball.at(i)->setLaunchType((launchTypeBall)(t_launchType));
      break;
    case ring:
      createRing(QColor(QRgb(0xA3A600)));
      m_v_ring.at(i)->setLaunchType((launchTypeRing)(t_launchType));
      break;
    case club:
      createPirouette(QColor(QRgb(0xA3A600)));
      m_v_pirouette.at(i)->setLaunchType((launchTypeClub)(t_launchType));
      break;
    default: break;
    }
  }

  siteswap->setLaunchType(t_launchType);

  // pass all the jobs to anim
  m_anim->setVJuggler(m_v_juggler);
  m_anim->setVBall(m_v_ball);
  m_anim->setVRing(m_v_ring);
  m_anim->setVClub(m_v_pirouette);
  m_anim->setSiteSwap(siteswap);
  m_anim->setAnim();
  m_anim->start();
}

void My3DWindow::setCameraToOrbit()
{
  m_camera->setPosition(m_positionCamera);
  m_camera->setUpVector(CAM_UP_VECTOR);
  m_camera->setViewCenter(CAM_ORBIT_VIEW_CENTER);
  m_camOController->setCamera(m_camera);
}

void My3DWindow::setCameraToFirstPers(int t_index)
{
  if (t_index < 0 || t_index >= m_v_juggler.size())
    return;
  QVector3D headPos = m_v_juggler.at(t_index)->getPositionHead();
  m_camera->setPosition(headPos);
  m_camera->setUpVector(CAM_UP_VECTOR);
  QVector3D lookAt = m_v_juggler.at(t_index)->getLookAt();
  m_camera->setViewCenter(lookAt);
  m_camFPController->setCamera(m_camera);
}
