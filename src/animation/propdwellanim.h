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

#ifndef PROPDWELLANIM_H
#define PROPDWELLANIM_H

#include <QParallelAnimationGroup>
#include "jugglingprop.h"
#include "qjoepass.h"

class PropDwellAnim : public QParallelAnimationGroup
{

  Q_OBJECT

public:

  // cstr for self ball
  PropDwellAnim(JugglingProp *t_prop,
                QVector3D t_initialPos,
                QVector3D t_finalPos,
                int dwellDuration,
                int t_launch,
                QObject * parent = nullptr);

  // cstr for passing ball
  PropDwellAnim(JugglingProp *t_prop,
                QVector3D t_initialPos,
                QVector3D t_medPos,
                QVector3D t_finalPos,
                int dwellDuration,
                int t_launch,
                QObject * parent = nullptr);

  // cstr for self rotating props
  PropDwellAnim(JugglingProp *t_prop,
                QVector3D t_initialPos,
                QVector3D t_finalPos,
                int dwellDuration,
                int t_launch,
                float t_initialRotX,
                float t_finalRotX,
                float t_initialRotY,
                float t_finalRotY,
                QObject *parent = nullptr);

  // cstr for passing rotating props
  PropDwellAnim(JugglingProp *t_prop,
                QVector3D t_initialPos,
                QVector3D t_medPos,
                QVector3D t_finalPos,
                int dwellDuration,
                int t_launch,
                float t_initialRotX,
                float t_finalRotX,
                float t_initialRotY,
                float t_finalRotY,
                QObject *parent = nullptr);


private:

  QSequentialAnimationGroup *passingTranslationAnim();
  QSequentialAnimationGroup *selfPassingTranslationAnim();
  QSequentialAnimationGroup *clubRotXAnim();
  QSequentialAnimationGroup *clubRotYAnim();

private:



};

#endif // PROPDWELLANIM_H
