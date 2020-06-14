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

#ifndef MYANIMATION_H
#define MYANIMATION_H

#include <QObject>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "juggler.h"
#include "jugglingball.h"
#include "jugglingring.h"
#include "pirouette.h"
#include "siteswap.h"

class MyAnimation : public QObject
{
  Q_OBJECT
public:
  explicit MyAnimation(QObject *parent = nullptr);

  void setJuggler(Juggler *aJuggler){ juggler = aJuggler;};
  void setVBall(QVector<JugglingBall *> aVBall) { vBall = aVBall;};
  void setVRing(QVector<JugglingRing *> aVRing) { vRing = aVRing;};
  void setVClub(QVector<Pirouette *> aVClub) {vClub = aVClub;};
  void setSiteSwap(SiteSwap *aSiteSwap);;
  void setAnim();

signals:

public slots:
  void startAnimation();
  void stopAnimation();

private:
  QSequentialAnimationGroup* parabolicAnim(Juggler *aJuggler,
                                           int indexProp,
                                           int launch,
                                           hand aHand);

  QSequentialAnimationGroup* dwellAnim(Juggler *aJuggler,
                                       int indexProp,
                                       int nextLaunch,
                                       hand aHand);

  hand changeHand(hand aHand);

private:
  Juggler *juggler;
  QVector<JugglingBall *> vBall;
  QVector<JugglingRing *> vRing;
  QVector<Pirouette *> vClub;
  SiteSwap *siteSwap;
  jugglingProp propType;
  int period;

  QParallelAnimationGroup *siteswapAnimation; // anim global


};

#endif // MYANIMATION_H
