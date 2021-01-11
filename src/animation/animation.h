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

#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "juggler.h"
#include "jugglingball.h"
#include "jugglingring.h"
#include "pirouette.h"
#include "siteswap.h"

class Animation : public QParallelAnimationGroup
{

  Q_OBJECT

public:

  explicit Animation(QObject *parent = nullptr);

  void setVJuggler(QVector<Juggler*> t_v_juggler) { m_v_juggler = t_v_juggler; };
  void setVBall(QVector<JugglingBall *> t_v_ball) { m_v_ball = t_v_ball; };
  void setVRing(QVector<JugglingRing *> t_v_ring) { m_v_ring = t_v_ring; };
  void setVClub(QVector<Pirouette *> t_v_club) { m_v_club = t_v_club; };
  void setSiteSwap(SiteSwap *t_siteSwap);
  void setAnim();

private:

  QSequentialAnimationGroup* parabolicAnim(Juggler *t_juggler,
                                           int t_indexProp,
                                           int t_launch,
                                           hand t_hand);

  QSequentialAnimationGroup* dwellAnim(Juggler *t_juggler,
                                       int t_indexProp,
                                       int t_nextLaunch,
                                       hand t_hand);

  QSequentialAnimationGroup* handAnim(Juggler *t_juggler,
                                      int t_indexProp,
                                      int t_launch,
                                      hand t_hand);

private:

  QVector<Juggler *> m_v_juggler;
  QVector<JugglingBall *> m_v_ball;
  QVector<JugglingRing *> m_v_ring;
  QVector<Pirouette *> m_v_club;
  SiteSwap *m_siteSwap;
  jugglingProp m_propType;
  int m_period;

};

#endif // ANIMATION_H
