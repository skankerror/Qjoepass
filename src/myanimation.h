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
