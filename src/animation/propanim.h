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

#ifndef PROPANIM_H
#define PROPANIM_H

#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <juggler.h>
#include "jugglingball.h"
#include "jugglingring.h"
#include "jugglingclub.h"
#include "siteswap.h"
#include "qjoepass.h"
#include "curve.h"

// classe pour créer tout le trajet d'un objet

class PropAnim : public QSequentialAnimationGroup
{

  Q_OBJECT

public:

  explicit PropAnim(QVector<Juggler *> t_v_juggler,
                    JugglingProp *t_prop,
                    int t_propId = 0,
                    propType t_propType = ball,
                    int t_launchType = 0,
                    QObject *parent = nullptr);

private:

  QParallelAnimationGroup *parabolicAnim(int t_jugglerIdLaunch,
                                         hand t_handLaunch,
                                         int t_jugglerIdRecieve,
                                         hand t_handRecieve,
                                         int t_launch);

  QParallelAnimationGroup *dwellAnim(int t_jugglerIdLaunch,
                                       hand t_handLaunch,
                                       int t_jugglerIdRecieve,
                                       hand t_handRecieve,
                                       int t_launch);

private:

  int m_propId; // the id of the prop
  int m_decayTime; // depending on propId, locate on the timeline
  QVector<Juggler *> m_v_juggler;

  propType m_propType;
  JugglingProp *m_prop;
  launchTypeBall m_launchTypeBall;
  launchTypeRing m_launchTypeRing;
  launchTypeClub m_launchTypeClub;
  // bool to know if we need to enlarge juggling
  bool m_isExtPlusCatch;
};

#endif // PROPANIM_H
