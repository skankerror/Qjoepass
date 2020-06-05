#ifndef ANIMSIMPLE_H
#define ANIMSIMPLE_H

#include <QObject>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "juggler.h"
#include "jugglingball.h"

class AnimSimple: public QObject
{
  Q_OBJECT
public:
  AnimSimple(Juggler *aJuggler,
             QVector<JugglingBall *> aVBall,
             QVector<int> aSiteswap);

public slots:
  void startAnimation();

private:
  QSequentialAnimationGroup* launchBall(Juggler *aJuggler,
                                        JugglingBall *aBall,
                                        int launch,
                                        hand aHand);

  hand changeHand(hand aHand);

private:
  Juggler *juggler;
  QVector<JugglingBall *> vBall;
  QVector<int> siteswap;
  int period;

  QParallelAnimationGroup *siteswapAnimation; // anim global
  QVector<QSequentialAnimationGroup *> vPropAnim; // vecteur pour anim pour chaque balle
};

#endif // ANIMSIMPLE_H
