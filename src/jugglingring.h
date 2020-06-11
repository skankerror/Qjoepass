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

#ifndef JUGGLINGRING_H
#define JUGGLINGRING_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;

class JugglingRing: public QEntity
{
  Q_OBJECT
  Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
  Q_PROPERTY(float rotX READ getRotX WRITE setRotX NOTIFY rotXChanged)
  Q_PROPERTY(float rotY READ getRotY WRITE setRotY NOTIFY rotYChanged)
  Q_PROPERTY(float rotZ READ getRotZ WRITE setRotZ NOTIFY rotZChanged)

public:
  JugglingRing(QEntity *aRootEntity,
               QTorusMesh *torusMesh,
               QColor aColor,
               launchTypeRing aLaunchType = normalRing);

  QVector3D position() const {return m_position;};
  float getRotX() const {return rotX;};
  float getRotY() const {return rotY;};
  float getRotZ() const {return rotZ;};
  launchTypeRing getLaunchType() const {return launchType;};


public slots:
  void setPosition(QVector3D position);
  void setRotX(float aRot);
  void setRotY(float aRot);
  void setRotZ(float aRot);
  void setLaunchType(launchTypeRing aLaunchType) {launchType = aLaunchType;};

signals:
  void positionChanged(QVector3D position);
  void rotXChanged(float aRot);
  void rotYChanged(float aRot);
  void rotZChanged(float aRot);

private:
  void updateTranslation();
  void updateRotX();
  void updateRotY();
  void updateRotZ();

private:
  QMetalRoughMaterial *ringMetalRoughMaterial;
  Qt3DCore::QTransform *ringTransform;
  QVector3D m_position;
  QColor color;
  bool enabled = true;
  launchTypeRing launchType;

  float rotX = 0;
  float rotY = 0;
  float rotZ = 0;

};

#endif // JUGGLINGRING_H
