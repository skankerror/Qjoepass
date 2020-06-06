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
