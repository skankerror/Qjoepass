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
#include <Qt3DAnimation> // for skeleton test
#include "settings.h"
#include "juggler.h"
#include "light.h"
#include "ground.h"
#include "pirouette.h"
#include "jugglingball.h"
#include "jugglingring.h"
#include "siteswap.h"
#include "animation.h"
#include "skeleton.h"

using namespace Qt3DCore;
using namespace Qt3DRender;
using namespace Qt3DExtras;
using namespace Qt3DAnimation; // for skeleton test


class My3DWindow: public Qt3DWindow
{
  Q_OBJECT
public:
  My3DWindow(MySettings *aSettings);

  int getJugglerCount() const;

private:
  void createCam();
  void createGround();
  void setGlobalObject();

signals:
  void jugglerCountChanged();

public slots:
  void changeBackground(QColor aColor);
  void changeGroundColor(QColor aColor);
  void createJuggler(float aRoty, QVector2D aPosition, QColor aColor);
  void createSkybox();
  void createLighting();
  void createPirouette(QColor aColor);
  void createBall(QColor aColor);
  void createRing(QColor aColor);
  void createSiteSwap(QVector<SiteswapEvent*> aVecEvent,
                      int aJugCount,
                      jugglingProp aPropType = ball,
                      int launchType = normalBall,
                      bool someSynchron = false);
  void setCameraToOrbit();
  void setCameraToFirstPers(int index);

private:
  QEntity *rootEntity;

  // Camera
  QCamera *m_camera;
  QFirstPersonCameraController *camFPController;
  QOrbitCameraController *camOController;
  QVector3D positionCamera;

  // Skybox
  QSkyboxEntity *skybox;

  // Ground
  Ground *ground;

  // light
  QEnvironmentLight *envLight;
  QPointLight *pointLight;
  QVector<Light *> vLight;

  // juggler
  QVector<Juggler *> vJuggler;

  // skeleton test
//  Skeleton *mySkeleton;

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

  Animation *anim;
};

#endif // MY3DWINDOW_H
