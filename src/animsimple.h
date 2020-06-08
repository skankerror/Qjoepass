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


#ifndef ANIMSIMPLE_H
#define ANIMSIMPLE_H

#include <QObject>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "juggler.h"
#include "jugglingball.h"
#include "siteswap.h"

class AnimSimple: public QObject
{
  Q_OBJECT
public:
  AnimSimple();
  AnimSimple(Juggler *aJuggler,
             QVector<JugglingBall *> aVBall,
             SiteSwap *aSiteSwap);

  void setJuggler(Juggler *aJuggler){ juggler = aJuggler;};
  void setVBall(QVector<JugglingBall *> aVBall) { vBall = aVBall;};
  void setSiteSwap(SiteSwap *aSiteSwap) {siteSwap = aSiteSwap;};
  void setAnim();

public slots:
  void startAnimation();
  void stopAnimation();

private:
  QSequentialAnimationGroup* launchBall(Juggler *aJuggler,
                                        JugglingBall *aBall,
                                        int launch,
                                        hand aHand);

  hand changeHand(hand aHand);

private:
  Juggler *juggler;
  QVector<JugglingBall *> vBall;
  SiteSwap *siteSwap;
  int period;

  QParallelAnimationGroup *siteswapAnimation; // anim global
};

#endif // ANIMSIMPLE_H