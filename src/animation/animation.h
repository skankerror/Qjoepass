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

#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "juggler.h"
#include "jugglingprop.h"
#include "siteswap.h"
#include "propanim.h"
#include "qjoepass.h"

class Animation : public QParallelAnimationGroup
{

  Q_OBJECT

public:

  explicit Animation(QObject *parent = nullptr);

  void setVJuggler(QVector<Juggler*> t_v_juggler) { m_v_juggler = t_v_juggler; };
  void setVProp(QVector<JugglingProp *> t_v_prop) { m_v_prop = t_v_prop; };
  void setSiteSwap(SiteSwap *t_siteSwap);
  void setAnim();

  void setPropType(propType t_propType) { m_propType = t_propType; };
  void setLaunchType(int t_launchType) { m_launchType = t_launchType; };

  // getters
  propType getPropType() const { return m_propType; };
  int getLaunchType() const { return m_launchType ;};


private:

  QSequentialAnimationGroup* handAnim(Juggler *t_juggler,
                                      int t_indexProp,
                                      int t_launch,
                                      hand t_hand);

private:

  QVector<Juggler *> m_v_juggler;
  QVector<JugglingProp *> m_v_prop;
  int m_launchType;
  SiteSwap *m_siteSwap;
  propType m_propType;
  int m_period;

};

#endif // ANIMATION_H
