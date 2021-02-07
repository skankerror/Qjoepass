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

#ifndef PROPPARABOLICANIM_H
#define PROPPARABOLICANIM_H

#include <QParallelAnimationGroup>
#include <jugglingprop.h>


class PropParabolicAnim : public QParallelAnimationGroup
{

  Q_OBJECT

public:

  // cstr for ball
  PropParabolicAnim(JugglingProp *t_prop,
                    QVector3D t_initialPos,
                    QVector3D t_finalPos,
                    int t_launch,
                    QObject *parent = nullptr);

  // cstr for club normal and flat
  PropParabolicAnim(JugglingProp *t_prop,
                    QVector3D t_initialPos,
                    QVector3D t_finalPos,
                    int t_launch,
                    float t_initialRotX,
                    float t_finalRotX,
                    QObject *parent = nullptr);

private:

  QSequentialAnimationGroup *translationAnim();
  QPropertyAnimation *rotXAnim();
  QSequentialAnimationGroup *rotYAnim(); // for helicopter

  float getArcTime() const;

private:

  JugglingProp *m_prop;
  QVector3D m_initialPos;
  QVector3D m_finalPos;
  int m_launch;
  float m_initialRotX;
  float m_finalRotX;
  float m_initialRotY;
  float m_finalRotY;

  float m_arcTime;
  int m_duration;

};

#endif // PROPPARABOLICANIM_H
