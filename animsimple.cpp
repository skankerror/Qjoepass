#include "animsimple.h"
#include <qmath.h>

AnimSimple::AnimSimple(Juggler *aJuggler,
                       QVector<JugglingBall *> aVBall,
                       QVector<int> aSiteswap)
  :juggler(aJuggler),
    vBall(aVBall),
    siteswap(aSiteswap)
{
  period = siteswap.size();

  auto ball = vBall.at(0);
  auto launch1Animation = launchBall(juggler, ball, 3, rightHand);
  launch1Animation->setLoopCount(1);
  auto launch2Animation = launchBall(juggler, ball, 3, leftHand);
  launch2Animation->setLoopCount(1);
  auto ball1Animation = new QSequentialAnimationGroup();
  ball1Animation->addAnimation(launch1Animation);
  ball1Animation->addAnimation(launch2Animation);
  ball1Animation->setLoopCount(-1);
  qDebug() << ball1Animation->duration();

  auto ball2 = vBall.at(1);
  auto launch3Animation = launchBall(juggler, ball2, 3, leftHand);
  launch3Animation->setLoopCount(1);
  auto launch4Animation = launchBall(juggler, ball2, 3, rightHand);
  launch4Animation->setLoopCount(1);
  auto ball2Animation = new QSequentialAnimationGroup();
  ball2Animation->addAnimation(launch3Animation);
  ball2Animation->addAnimation(launch4Animation);
  ball2Animation->setLoopCount(-1);
  auto ball2SeqAnim = new QSequentialAnimationGroup();
  ball2SeqAnim->addPause(DELAY_LAUNCH);
  ball2SeqAnim->addAnimation(ball2Animation);


  auto ball3 = vBall.at(2);
  auto launch5Animation = launchBall(juggler, ball3, 3, rightHand);
  launch5Animation->setLoopCount(1);
  auto launch6Animation = launchBall(juggler, ball3, 3, leftHand);
  launch6Animation->setLoopCount(1);
  auto ball3Animation = new QSequentialAnimationGroup();
  ball3Animation->addAnimation(launch5Animation);
  ball3Animation->addAnimation(launch6Animation);
  ball3Animation->setLoopCount(-1);
  auto ball3SeqAnim = new QSequentialAnimationGroup();
  ball3SeqAnim->addPause(DELAY_LAUNCH * 2);
  ball3SeqAnim->addAnimation(ball3Animation);

  auto siteswapAnimation = new QParallelAnimationGroup();
  siteswapAnimation->addAnimation(ball1Animation);
  siteswapAnimation->addAnimation(ball2SeqAnim);
  siteswapAnimation->addAnimation(ball3SeqAnim);
  siteswapAnimation->setLoopCount(-1);
  siteswapAnimation->start();

}

QSequentialAnimationGroup *AnimSimple::launchBall(Juggler *aJuggler,
                                                  JugglingBall *aBall,
                                                  int launch,
                                                  hand aHand)
{
  auto animGroup = new QSequentialAnimationGroup();
  QVector3D posBall; // pos where it starts
  QVector3D posFinal; // pos where it should finish
  hand receiveHand; // recieve hand to calculate curve after catch
  // todo handle 0 launch

  // odd launches
  if (aHand == leftHand && launch % 2 != 0)
  {
    posBall = aJuggler->getPositionLHint();
    posFinal = aJuggler->getPositionRHext();
    receiveHand = rightHand;
  }
  if (aHand == rightHand && launch % 2 != 0)
  {
    posBall = aJuggler->getPositionRHint();
    posFinal = aJuggler->getPositionLHext();
    receiveHand = leftHand;
  }

  // even launches
  if (aHand == leftHand && launch % 2 == 0)
  {
    posBall = aJuggler->getPositionLHint();
    posFinal = aJuggler->getPositionLHext();
    receiveHand = leftHand;
  }
  if (aHand == rightHand && launch % 2 == 0)
  {
    posBall = aJuggler->getPositionRHint();
    posFinal = aJuggler->getPositionRHext();
    receiveHand = rightHand;
  }
//  qDebug() << posBall;

  float timeLaunch = ((float)(launch) * TEMPO) - DWELL_TIME;
//  qDebug() << timeLaunch;

  // we calculate velocity launch
  QVector3D velBall = ((posFinal - posBall) - 0.5 *
                       (GRAVITY * timeLaunch * timeLaunch)) / timeLaunch;

  // By counting frames we add 1 due to float to integer approx.
  int frameCount = (int)((timeLaunch / (DELTA_TIME)) + 1);
//  qDebug() << frameCount;

  // loop creates all our animations for launch
  for (int i = 0; i <= frameCount; i++)
  {
    auto animBall = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
    animBall->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
    animBall->setStartValue(posBall);
    QVector3D posBall2 = posBall + (DELTA_TIME * velBall);
    animBall->setEndValue(posBall2);
    animBall->setLoopCount(1);
    animGroup->addAnimation(animBall);
    posBall = posBall2;
//    qDebug() << posBall;
    velBall = velBall + (DELTA_TIME * GRAVITY);
  }


  // now we handle curve in the hand
  // surely it's the right place to begin
  posBall = posFinal;

  // find out rotation center
  QVector3D centerCurve;
  if (receiveHand == leftHand)
  {
    centerCurve = (posBall + aJuggler->getPositionLHint()) / 2;
//    qDebug() << posBall;
//    qDebug() << centerCurve;
//    qDebug() << aJuggler->getPositionLHint();
  }
  else
  {
    centerCurve = (posBall + aJuggler->getPositionRHint()) / 2;
//    qDebug() << posBall;
//    qDebug() << centerCurve;
//    qDebug() << aJuggler->getPositionRHint();
  }

  // determine axis for rotation
  float rotY = juggler->getRotY();
  QVector3D axisCurve = QVector3D(sinf(rotY), 0, cosf(rotY));

  // determine number of frames
  frameCount = (int)((DWELL_TIME / DELTA_TIME) + 1);

  // determine angles for each delta animation
  float deltaAngles = (float)(180 / frameCount);

  // loop creates all our animations for dwell time
  for (int i = 0; i <= frameCount; i++)
  {
    auto animBall = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
    animBall->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
    animBall->setStartValue(posBall);
    // handle rotation stuff
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.translate(centerCurve);
    if (receiveHand == leftHand)
      rot.rotate(-deltaAngles, axisCurve);
    else
      rot.rotate(deltaAngles, axisCurve);
    rot.translate(-centerCurve);
    QVector3D posBall2 = rot * posBall;

    animBall->setEndValue(posBall2);
    animBall->setLoopCount(1);
    animGroup->addAnimation(animBall);
    posBall = posBall2;
//    qDebug() << posBall;
  }

  return animGroup;
}
