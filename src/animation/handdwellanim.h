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

#ifndef HANDDWELLANIM_H
#define HANDDWELLANIM_H

#include <QSequentialAnimationGroup>
#include "juggler.h"
#include "jugglingprop.h"
#include "qjoepass.h"

class HandDwellAnim : public QParallelAnimationGroup
{

  Q_OBJECT

public:

  // cstr for self launch
  HandDwellAnim(Juggler *t_juggler,
                hand t_hand,
                JugglingProp *t_prop,
                QVector3D t_initialPos,
                QVector3D t_finalPos,
                int t_launch,
                QObject * parent = nullptr);

  // cstr for passing launch
  HandDwellAnim(Juggler *t_juggler,
                hand t_hand,
                JugglingProp *t_prop,
                QVector3D t_initialPos,
                QVector3D t_medPos,
                QVector3D t_finalPos,
                int t_launch,
                QObject * parent = nullptr);

private:

  void passingAnim();
  void selfAnim();

private:

  Juggler *m_juggler;
  hand m_hand;
  JugglingProp *m_prop;
  QVector3D m_initialPos;
  QVector3D m_medPos;
  QVector3D m_finalPos;
  int m_launch;
  QByteArray m_propertyName;

};

#endif // HANDDWELLANIM_H
