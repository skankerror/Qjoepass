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

#ifndef JUGGLINGPROP_H
#define JUGGLINGPROP_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include "qjoepass.h"

class JugglingProp : public Qt3DCore::QEntity
{

  Q_OBJECT

  Q_PROPERTY(QVector3D m_position
             READ getPosition
             WRITE setPosition
             NOTIFY positionChanged)

  Q_PROPERTY(float m_rotX
             READ getRotX
             WRITE setRotX
             NOTIFY rotXChanged)

  Q_PROPERTY(float m_rotY
             READ getRotY
             WRITE setRotY
             NOTIFY rotYChanged)

  Q_PROPERTY(float m_rotZ
             READ getRotZ
             WRITE setRotZ
             NOTIFY rotZChanged)

public:

  JugglingProp(QEntity *t_rootEntity,
               QColor &t_color);

  QVector3D getPosition() const { return m_position; };
  float getRotX() const { return m_rotX; };
  float getRotY() const { return m_rotY; };
  float getRotZ() const { return m_rotZ; };

public slots:

  void setPosition(QVector3D t_position);
  void setRotX(float t_rotX);
  void setRotY(float t_rotY);
  void setRotZ(float t_rotZ);

signals:

  void positionChanged(QVector3D t_position);
  void rotXChanged(float t_rotX);
  void rotYChanged(float t_rotY);
  void rotZChanged(float t_rotZ);

protected:

  void updateTransform();
  void updateRotX();
  void updateRotY();
  void updateRotZ();

protected:

  Qt3DExtras::QMetalRoughMaterial *m_propMetalRoughMaterial;
  Qt3DCore::QTransform *m_propTransform;
  QVector3D m_position;
  QColor m_color;
  bool m_enabled = true;
  float m_rotX/* = 0*/;
  float m_rotY/* = 0*/;
  float m_rotZ/* = 0*/;
};

#endif // JUGGLINGPROP_H
