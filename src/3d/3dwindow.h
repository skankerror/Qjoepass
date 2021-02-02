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

#ifndef MY3DWINDOW_H
#define MY3DWINDOW_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>
#include "settings.h"
#include "juggler.h"
#include "light.h"
#include "ground.h"
#include "jugglingball.h"
#include "jugglingring.h"
#include "jugglingclub.h"
#include "siteswap.h"
#include "animation.h"

using namespace Qt3DCore;
using namespace Qt3DRender;
using namespace Qt3DExtras;

class My3DWindow: public Qt3DWindow
{

  Q_OBJECT

public:

  My3DWindow(MySettings *t_settings);

  int getJugglerCount() const;

private:

  void createCam();
  void createGround();
  void createSkybox();
  void createLighting();
  void setGlobalObject();
  void createJuggler(float t_rotY,
                     QVector2D t_position,
                     QColor t_color);
  void createClub(QColor t_color);
  void createBall(QColor t_color);
  void createRing(QColor t_color);
  void deleteJugglers();
  void createJugglers(int t_jugglerCount);
  void deleteProps();
  void createProps(int t_propCount,
                   propType t_propType);
  void setAnimation(propType t_propType,
                    int t_launchType);

signals:

  void jugglerCountChanged();

public slots:

  void changeBackground(QColor t_color);
  void changeGroundColor(QColor t_color);
  void createSiteSwap(QVector<siteswapEvent *> t_v_event,
                      int t_jugCount,
                      propType t_propType = ball,
                      int t_launchType = normalBall,
                      bool t_synchron = false);
  void setCameraToOrbit();
  void setCameraToFirstPers(int t_index);

private:

  // root entity of 3D scene
  QEntity *m_rootEntity;

  // Camera
  QCamera *m_camera;
  QFirstPersonCameraController *m_camFPController;
  QOrbitCameraController *m_camOController;
  QVector3D m_positionCamera;

  // Skybox
  QSkyboxEntity *m_skybox;

  // Ground
  Ground *m_ground;

  // light
  QEnvironmentLight *m_envLight;
  QPointLight *m_pointLight;
  QVector<Light *> m_v_light;

  // juggler
  QVector<Juggler *> m_v_juggler;

  // prop
  QVector<JugglingProp *> m_v_prop;

    // club
    QMesh *m_pirouetteMesh;

    // ball
    QSphereMesh *m_sphereMesh;

    // ring
    QTorusMesh *m_torusMesh;

  // global app settings
  MySettings *m_settings;

  // siteswap
  SiteSwap *m_siteswap;

  // animation
  Animation *m_anim;
};

#endif // MY3DWINDOW_H
