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

#include "animsimple.h"
#include "mycurves.h"

AnimSimple::AnimSimple():
  siteswapAnimation(new QParallelAnimationGroup())
{}

AnimSimple::AnimSimple(Juggler *aJuggler,
                       QVector<JugglingBall *> aVBall,
                       SiteSwap *aSiteSwap)
  :juggler(aJuggler),
    vBall(aVBall),
    siteSwap(aSiteSwap),
    siteswapAnimation(new QParallelAnimationGroup())
{
  // In case we call this cstr we cat set the anim
  setAnim();
}

void AnimSimple::setAnim()
{
  period = siteSwap->getPeriod();
  int numProp = vBall.size();

  if (!(siteSwap->isValid()) || numProp != siteSwap->getNumProp())
  {
    qDebug() << "Siteswap isn't valid or internal problem about number of props";
  }

  for (int i = 0; i < numProp; i++) // for each prop
  {
    int launchPos = i % period;
    int launch = siteSwap->at(launchPos);
    hand launchHand;
    auto ball = vBall.at(i);
    auto ballAnim = new QSequentialAnimationGroup(); // will handle the whole moving
    auto ballGlobAnim = new QSequentialAnimationGroup();// needed to add pause at the beginning

    // we had delay for each prop
    if (i)
      ballGlobAnim->addPause(DELAY_LAUNCH * i);

    // handle first move
    if (i % 2 == 0)
      launchHand = rightHand;
    else
      launchHand = leftHand;
    auto launchAnim = launchBall(juggler, ball, launch, launchHand);
    launchAnim->setLoopCount(1);
    ballAnim->addAnimation(launchAnim);

    // search for new site
    int newLaunchPos = (launch + launchPos) % period;
    // setting the new hand
    // if launch is odd we change hand
    hand newLaunchHand;
    if (launch % 2 == 1)
      newLaunchHand = changeHand(launchHand);
    else
      newLaunchHand = launchHand;
    // if we're not on the same site, let's keep on animate
    while (newLaunchPos != launchPos)
    {
      int newLaunch = siteSwap->at(newLaunchPos);
      auto followLaunchAnim = launchBall(juggler, ball, newLaunch, newLaunchHand);
      followLaunchAnim->setLoopCount(1);
      ballAnim->addAnimation(followLaunchAnim);
      // is it the other hand ?
      if (newLaunch % 2 == 1)
        newLaunchHand = changeHand(newLaunchHand);
      newLaunchPos = (newLaunch + newLaunchPos) % period;
    }

    // now we get back to initial site, if we changed hand, let's do the whole thing again
    if (newLaunchHand != launchHand)
    {
      auto backLaunchAnim = launchBall(juggler, ball, launch, newLaunchHand);
      backLaunchAnim->setLoopCount(1);
      ballAnim->addAnimation(backLaunchAnim);
      newLaunchPos = (launch + launchPos) % period;
      if (launch % 2 == 1)
        newLaunchHand = changeHand(newLaunchHand);
      while (newLaunchPos != launchPos)
      {
        int newLaunch = siteSwap->at(newLaunchPos);
        auto backFollowLaunchAnim = launchBall(juggler, ball, newLaunch, newLaunchHand);
        backFollowLaunchAnim->setLoopCount(1);
        ballAnim->addAnimation(backFollowLaunchAnim);
        if (newLaunch % 2 == 1)
          newLaunchHand = changeHand(newLaunchHand);
        newLaunchPos = (newLaunch + newLaunchPos) % period;
      }
    }
    // we add to the anim containing starting pause
    ballGlobAnim->addAnimation(ballAnim);
    ballAnim->setLoopCount(-1);
    siteswapAnimation->addAnimation(ballGlobAnim); // and we add to the global parallel anim
  }

//  // test anim hand en mode moulin lol
//  auto animLeftHand = new QPropertyAnimation(juggler, QByteArrayLiteral("leftForearmPosition"));
////  animLeftHand->setDuration(DELAY_LAUNCH * 2 + 6);
//  animLeftHand->setDuration((int)(TEMPO * 1000.0f));
//  animLeftHand->setStartValue(-180);
//  animLeftHand->setEndValue(180);
//  animLeftHand->setLoopCount(-1);
//  siteswapAnimation->addAnimation(animLeftHand);

//  auto animRightHand = new QPropertyAnimation(juggler, QByteArrayLiteral("rightForearmPosition"));
////  animRightHand->setDuration(DELAY_LAUNCH * 2 + 6);
//  animRightHand->setDuration((int)(TEMPO * 1000.0f));
//  animRightHand->setStartValue(360);
//  animRightHand->setEndValue(0);
//  animRightHand->setLoopCount(-1);
//  siteswapAnimation->addAnimation(animRightHand);

}

void AnimSimple::startAnimation()
{
  siteswapAnimation->start();
}

void AnimSimple::stopAnimation()
{
  siteswapAnimation->stop();
  siteswapAnimation->clear();
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
  float timeLaunch = ((float)(launch) * TEMPO) - DWELL_TIME;

  // we calculate velocity launch
  QVector3D velBall = ((posFinal - posBall) - 0.5 *
                       (GRAVITY * timeLaunch * timeLaunch)) / timeLaunch;

  // By counting frames we add 1 due to float to integer approx.
  int frameCount = (int)((timeLaunch / (DELTA_TIME)) + 1);

  // We create our curve
  QVector<QVector3D> vParabolic = MyCurves::curveParabolic(velBall, posBall, frameCount);

  // loop creates all our animations for launch
  for (int i = 0; i <= frameCount; i++)
  {
    auto animBall = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
    animBall->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
    animBall->setStartValue(vParabolic.at(i));
    animBall->setEndValue(vParabolic.at(i + 1));
    animBall->setLoopCount(1);
    animGroup->addAnimation(animBall);
  }

  // now we handle curve in the hand
  // surely it's the right place to begin
  posBall = posFinal;

  // find out rotation center
  QVector3D centerCurve;
  if (receiveHand == leftHand)
  {
    centerCurve = (posBall + aJuggler->getPositionLHint()) / 2;
  }
  else
  {
    centerCurve = (posBall + aJuggler->getPositionRHint()) / 2;
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
  }
  return animGroup;
}

hand AnimSimple::changeHand(hand aHand)
{
  if (aHand == leftHand)
    return rightHand;
  else
    return leftHand;
}
