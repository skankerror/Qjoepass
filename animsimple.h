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

private:
  QSequentialAnimationGroup* launchBall(Juggler *aJuggler,
                                        JugglingBall *aBall,
                                        int launch,
                                        hand aHand);

private:
  Juggler *juggler;
  QVector<JugglingBall *> vBall;
  QVector<int> siteswap;
  int period;
};

#endif // ANIMSIMPLE_H