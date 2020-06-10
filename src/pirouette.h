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

#ifndef PIROUETTE_H
#define PIROUETTE_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;

class Pirouette: public QEntity
{
  Q_OBJECT
  Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
  Q_PROPERTY(float rotX READ getRotX WRITE setRotX NOTIFY rotXChanged)
  Q_PROPERTY(float rotY READ getRotY WRITE setRotY NOTIFY rotYChanged)
  Q_PROPERTY(float rotZ READ getRotZ WRITE setRotZ NOTIFY rotZChanged)

public:
  Pirouette(QEntity *aRootEntity,
            QMesh *aPirouetteMesh,
            QColor &aColor);

  QVector3D position() const {return m_position;};
  float getRotX() const {return rotX;};
  float getRotY() const {return rotY;};
  float getRotZ() const {return rotZ;};

public slots:
  void setPosition(QVector3D aPosition);
  void setRotX(float aRot);
  void setRotY(float aRot);
  void setRotZ(float aRot);

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
  QMetalRoughMaterial *clubMetalRoughMaterial;

  Qt3DCore::QTransform *pirouetteTransform;

  QColor color;
  QVector3D m_position;

  bool enabled = true;

  float rotX = 0;
  float rotY = 0;
  float rotZ = 0;

};

#endif // PIROUETTE_H
