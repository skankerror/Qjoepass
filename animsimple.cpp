#include "animsimple.h"

AnimSimple::AnimSimple(Juggler *aJuggler,
                       QVector<JugglingBall *> aVBall,
                       QVector<int> aSiteswap)
  :juggler(aJuggler),
    vBall(aVBall),
    siteswap(aSiteswap)
{
  auto ball = vBall.at(0);
  auto launch1Animation = launchBall(juggler, ball, 3, leftHand);
  launch1Animation->setLoopCount(-1);
  launch1Animation->start();

}

QSequentialAnimationGroup *AnimSimple::launchBall(Juggler *aJuggler,
                                                  JugglingBall *aBall,
                                                  int launch,
                                                  hand aHand)
{
  auto animGroup = new QSequentialAnimationGroup();
  QVector3D posBall;
  QVector3D posFinal;
  if (aHand == leftHand)
  {
    posBall = aJuggler->getPositionRHint();
    posFinal = aJuggler->getPositionLHext();
  }
  else
  {
    posBall = aJuggler->getPositionLHint();
    posFinal = aJuggler->getPositionRHext();
  }
  float timeLaunch = (float(launch) / TEMPO) - DWELL_TIME;
  QVector3D velBall = ((posFinal - posBall) - 0.5 *
                       (gravity * timeLaunch * timeLaunch)) / timeLaunch;
  int frameCount = (int)(timeLaunch / DELTA_TIME);
  for (int i = 0; i < frameCount; i++)
  {
    auto animBall = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
    animBall->setDuration((int)(DELTA_TIME * 600)); // devrait être à 1000... à voir
    animBall->setStartValue(posBall);
    QVector3D posBall2 = posBall + (DELTA_TIME * velBall);
    animBall->setEndValue(posBall2);
    animBall->setLoopCount(1);
    animGroup->addAnimation(animBall);
    posBall = posBall2;
    velBall = velBall + (DELTA_TIME * gravity);
  }
  return animGroup;
}
