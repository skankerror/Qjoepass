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
#include <QtMath>
#include <QDebug>

My3DWindow::My3DWindow(MySettings *t_settings)
  : Qt3DWindow(nullptr/*, Qt3DRender::API::RHI*/),
    m_rootEntity(new QEntity()),
    m_skybox(new QSkyboxEntity()),
    m_pointLight(new QPointLight()),
    m_settings(t_settings),
    m_siteswap(nullptr),
    m_anim(new Animation())
{
  // Root entity, root object of the scene
  setRootEntity(m_rootEntity);

  //background
  QColor colorBG = m_settings->value("world/colorbg").value<QColor>();
  defaultFrameGraph()->setClearColor(colorBG);

  createCam();
  createGround();
  createLighting();
  createSkybox();

/**************************** testing zone ***************************/

}

void My3DWindow::createCam()
{
  m_camera = camera();
  m_camera->lens()->setPerspectiveProjection(CAM_FOV,
                                             CAM_RATIO,
                                             CAM_NEARPLANE,
                                             CAM_FARPLANE);
  m_positionCamera = CAM_INITIAL_POSITION;
  m_camFPController = new QFirstPersonCameraController(m_rootEntity);
  m_camOController = new QOrbitCameraController(m_rootEntity);
  setCameraToOrbit();
}

void My3DWindow::createGround()
{
  QColor colorG = m_settings->value("world/groundColor").value<QColor>();
  m_ground = new Ground(m_rootEntity,
                        colorG);
}

void My3DWindow::deleteJugglers()
{
  if (m_v_juggler.size())
    for (int i = 0; i < m_v_juggler.size(); i++)
      m_v_juggler.at(i)->setEnabled(false);

  m_v_juggler.clear();
  m_v_juggler.squeeze();
}

void My3DWindow::createJugglers(int t_jugglerCount)
{
  if (!t_jugglerCount)
    return;

  deleteJugglers();

  for (int i = 0; i < t_jugglerCount; i++)
  {
    float angle = 360.0f / t_jugglerCount;
    float rotY = i * angle;
    auto pos = QVector2D(- qSin(qDegreesToRadians(rotY)) * DEFAULT_JUG_POS_CIRC_RADIUS,
                         - qCos(qDegreesToRadians(rotY)) * DEFAULT_JUG_POS_CIRC_RADIUS);
    createJuggler(rotY, pos, QColor(QRgb(0xFF0000)));
  }
}

void My3DWindow::deleteProps()
{
  if (m_v_prop.size())
    for (int i = 0; i < m_v_prop.size(); i++)
      m_v_prop.at(i)->setEnabled(false);

  m_v_prop.clear();
  m_v_prop.squeeze();
}

void My3DWindow::createProps(int t_propCount,
                             propType t_propType)
{
  if (!t_propCount)
    return;

  deleteProps();

  for (int i = 0; i < t_propCount; i++)
  {
    switch(t_propType)
    {
    case ball:
      createBall(QColor(QRgb(0xA3A600)));
      break;
    case ring:
      createRing(QColor(QRgb(0xA3A600)));
      break;
    case club:
      createClub(QColor(QRgb(0xA3A600)));
      break;
    default: break;
    }
  }

}

void My3DWindow::setAnimation(propType t_propType,
                              int t_launchType)
{
  // stop and clear existing anim
  m_anim->stop();
  m_anim->clear();

  // pass all the jobs to anim
  m_anim->setVJuggler(m_v_juggler);
  m_anim->setVProp(m_v_prop);
  m_anim->setPropType(t_propType);
  m_anim->setLaunchType(t_launchType);
  m_anim->setSiteSwap(m_siteswap);
  m_anim->setAnim();
  m_anim->start();
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

void My3DWindow::createJuggler(float t_rotY,
                               QVector2D t_position,
                               QColor t_color)
{
  auto juggler = new Juggler(m_rootEntity, t_rotY, t_position, t_color);
  m_v_juggler.append(juggler);

  // give data to juggler position widget
  QVector<QVector3D> vecToSend;
  for (int i = 0; i < m_v_juggler.size(); i++)
  {
    auto juggler = m_v_juggler.at(i);

    // vec(jugX, jugZ, jugRotY)
    auto vec = QVector3D(juggler->getPosition().x(),
                         juggler->getPosition().z(),
                         juggler->getRotY());
    vecToSend.append(vec);
  }
  emit jugglerCountChanged(vecToSend);
}

void My3DWindow::createSkybox()
{
  m_skybox->setBaseName(QStringLiteral(SKYBOX_BASE_NAME));
  m_skybox->setExtension(QStringLiteral(SKYBOX_EXTENSION));

  auto *skyTransform = new Qt3DCore::QTransform(m_skybox);
  skyTransform->setScale3D(QVector3D(SKYBOX_SCALE,
                                     SKYBOX_SCALE,
                                     SKYBOX_SCALE));
  m_skybox->addComponent(skyTransform);
  m_skybox->setParent(m_rootEntity);
}

void My3DWindow::createLighting()
{
  // create one pointlight for 3 sources
  m_pointLight->setColor(QColor(QRgb(LIGHT_COLOR)));
  m_pointLight->setIntensity(LIGHT_INTENSITY);

  auto light = new Light(m_rootEntity,
                         m_pointLight,
                         LIGHT1_POS);
  m_v_light.append(light);

  auto light2 = new Light(m_rootEntity,
                          m_pointLight,
                          LIGHT2_POS);
  m_v_light.append(light2);

  auto light3 = new Light(m_rootEntity,
                          m_pointLight,
                          LIGHT3_POS);
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

void My3DWindow::createClub(QColor t_color)
{
  auto club = new JugglingClub(m_rootEntity,
                               t_color);
  m_v_prop.append(club);

}

void My3DWindow::createBall(QColor t_color)
{
  auto ball = new JugglingBall(m_rootEntity,
                               t_color);
  m_v_prop.append(ball);
}

void My3DWindow::createRing(QColor t_color)
{
  auto ring = new JugglingRing(m_rootEntity,
                               t_color);
  m_v_prop.append(ring);
}

void My3DWindow::createSiteSwap(QVector<siteswapEvent *> t_v_event,
                                int t_jugCount,
                                propType t_propType,
                                int t_launchType,
                                bool t_synchron)
{
  // delete possibly existing siteswap
  if (m_siteswap)
    m_siteswap->deleteLater();

  // create siteswap with values
  m_siteswap = new SiteSwap(t_v_event,
                            t_jugCount,
                            t_synchron,
                            this);
  // test if it's valid
  if (!(m_siteswap->isValid()))
  {
    qDebug() << "siteswap is not valid !";
    return;
  }

  // create jugglers
  createJugglers(t_jugCount);

  // create props
  int propCount = m_siteswap->getPropCount();
  createProps(propCount,
              t_propType);

  // set animation
  setAnimation(t_propType,
               t_launchType);
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
