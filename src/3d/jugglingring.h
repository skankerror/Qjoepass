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

#include "jugglingprop.h"

class JugglingRing: public JugglingProp
{

  Q_OBJECT

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

  JugglingRing(QEntity *t_rootEntity,
               QTorusMesh *t_torusMesh,
               QColor &t_color,
               launchTypeRing t_launchType = normalRing);

  float getRotX() const { return m_rotX; };
  float getRotY() const { return m_rotY; };
  float getRotZ() const { return m_rotZ; };
  launchTypeRing getLaunchType() const { return m_launchType; };

private:

  void updateRotX();
  void updateRotY();
  void updateRotZ();

public slots:

  void setRotX(float t_rotX);
  void setRotY(float t_rotY);
  void setRotZ(float t_rotZ);
  void setLaunchType(launchTypeRing t_launchType) { m_launchType = t_launchType; };

signals:

  void rotXChanged(float t_rotX);
  void rotYChanged(float t_rotY);
  void rotZChanged(float t_rotZ);

private:

  launchTypeRing m_launchType;

  float m_rotX = 0;
  float m_rotY = 0;
  float m_rotZ = 0;

};

#endif // JUGGLINGRING_H
