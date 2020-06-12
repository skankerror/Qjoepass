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
  // In case we call this cstr we can set the anim
  propType = siteSwap->getPropType();
  setAnim();
}

AnimSimple::AnimSimple(Juggler *aJuggler,
                       QVector<JugglingRing *> aVRing,
                       SiteSwap *aSiteSwap)
  :juggler(aJuggler),
    vRing(aVRing),
    siteSwap(aSiteSwap),
    siteswapAnimation(new QParallelAnimationGroup())
{
  propType = siteSwap->getPropType();
  setAnim();
}

AnimSimple::AnimSimple(Juggler *aJuggler,
                       QVector<Pirouette *> aVClub,
                       SiteSwap *aSiteSwap)
  :juggler(aJuggler),
    vClub(aVClub),
    siteSwap(aSiteSwap),
    siteswapAnimation(new QParallelAnimationGroup())
{
  propType = siteSwap->getPropType();
  setAnim();
}

void AnimSimple::setSiteSwap(SiteSwap *aSiteSwap)
{
  siteSwap = aSiteSwap;
  propType = siteSwap->getPropType();
}

void AnimSimple::setAnim()
{
  period = siteSwap->getPeriod();
  int numProp = siteSwap->getNumProp();

  if (!(siteSwap->isValid())) // ça marche pas...
  {
    qDebug() << "Siteswap isn't valid";
    return;
  }

  for (int i = 0; i < numProp; i++) // for each prop
  {
    int launchPos = i % period;
    int launch = siteSwap->at(launchPos);
    hand launchHand;
    auto propAnim = new QSequentialAnimationGroup(); // will handle the whole moving
    auto propGlobAnim = new QSequentialAnimationGroup();// needed to add pause at the beginning

    // we had delay for each prop
    if (i)
      propGlobAnim->addPause(DELAY_LAUNCH * i);

    // handle first move
    if (i % 2 == 0)
      launchHand = rightHand;
    else
      launchHand = leftHand;
    auto launchAnim = launchProp(juggler, i, launch, launchHand);
    launchAnim->setLoopCount(1);
    propAnim->addAnimation(launchAnim);

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
      auto followLaunchAnim = launchProp(juggler, i, newLaunch, newLaunchHand);
      followLaunchAnim->setLoopCount(1);
      propAnim->addAnimation(followLaunchAnim);
      // is it the other hand ?
      if (newLaunch % 2 == 1)
        newLaunchHand = changeHand(newLaunchHand);
      newLaunchPos = (newLaunch + newLaunchPos) % period;
    }

    // now we get back to initial site, if we changed hand, let's do the whole thing again
    if (newLaunchHand != launchHand)
    {
      auto backLaunchAnim = launchProp(juggler, i, launch, newLaunchHand);
      backLaunchAnim->setLoopCount(1);
      propAnim->addAnimation(backLaunchAnim);
      newLaunchPos = (launch + launchPos) % period;
      if (launch % 2 == 1)
        newLaunchHand = changeHand(newLaunchHand);
      while (newLaunchPos != launchPos)
      {
        int newLaunch = siteSwap->at(newLaunchPos);
        auto backFollowLaunchAnim = launchProp(juggler, i, newLaunch, newLaunchHand);
        backFollowLaunchAnim->setLoopCount(1);
        propAnim->addAnimation(backFollowLaunchAnim);
        if (newLaunch % 2 == 1)
          newLaunchHand = changeHand(newLaunchHand);
        newLaunchPos = (newLaunch + newLaunchPos) % period;
      }
    }
    // we add to the anim containing starting pause
    propGlobAnim->addAnimation(propAnim);
    propAnim->setLoopCount(-1);
    siteswapAnimation->addAnimation(propGlobAnim); // and we add to the global parallel anim
  }

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

QSequentialAnimationGroup *AnimSimple::launchProp(Juggler *aJuggler, int indexProp, int launch, hand aHand)
{
  auto animGroup = new QSequentialAnimationGroup();
  QVector3D posProp; // pos where it starts
  QVector3D posFinal; // pos where it should finish
  hand receiveHand; // recieve hand to calculate curve after catch

  // odd launches
  if (aHand == leftHand && launch % 2 != 0)
  {
    posProp = aJuggler->getPositionLHint();
    posFinal = aJuggler->getPositionRHext();
    receiveHand = rightHand;
  }
  if (aHand == rightHand && launch % 2 != 0)
  {
    posProp = aJuggler->getPositionRHint();
    posFinal = aJuggler->getPositionLHext();
    receiveHand = leftHand;
  }

  // even launches
  if (aHand == leftHand && launch % 2 == 0)
  {
    posProp = aJuggler->getPositionLHint();
    posFinal = aJuggler->getPositionLHext();
    receiveHand = leftHand;
  }
  if (aHand == rightHand && launch % 2 == 0)
  {
    posProp = aJuggler->getPositionRHint();
    posFinal = aJuggler->getPositionRHext();
    receiveHand = rightHand;
  }
  float timeLaunch = ((float)(launch) * TEMPO) - DWELL_TIME;

  // we calculate velocity launch
  QVector3D velBall = ((posFinal - posProp) - 0.5 *
                       (GRAVITY * timeLaunch * timeLaunch)) / timeLaunch;

  // By counting frames we add 1 due to float to integer approx.
  int frameCount = (int)((timeLaunch / (DELTA_TIME)) + 1);

  // We create our curve
  QVector<QVector3D> vParabolic = MyCurves::curveParabolic(velBall, posProp, frameCount);

  // declare one object for each prop in case we need it in switch routines
  JugglingBall *aBall;
  JugglingRing *aRing;
  Pirouette *aClub;

  // for ring or club rotation we have to create 3 more anims
  QParallelAnimationGroup *animTempGroup = new QParallelAnimationGroup();
  QPropertyAnimation *animRotProp;
  QSequentialAnimationGroup *animTranslationGroup = new QSequentialAnimationGroup();

  // declare them in case we need
  int launchType; // normal, flat, pancake ??
  int rotCount; // number of rotation
  float fRotCount; // needed for helicopter
  float rotY; // to handle different positions between normal, flat, hlicopter

  switch(propType)
  {
  case ball:
    aBall = vBall.at(indexProp);
    // loop creates all our animations for translation
    for (int i = 0; i <= frameCount; i++)
    {
      auto animBall = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
      animBall->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
      animBall->setStartValue(vParabolic.at(i));
      animBall->setEndValue(vParabolic.at(i + 1));
      animBall->setLoopCount(1);
      animGroup->addAnimation(animBall);
    }
    break;
  case ring:
    aRing = vRing.at(indexProp);
    // set rotY to align with juggler
    rotY = juggler->getRotY();

    // switch between types
    launchType = aRing->getLaunchType();
    switch (launchType)
    {
    case normalRing:
      aRing->setRotY(rotY + RING_BASIC_ROTY);

      // loop creates all our animations for translation
      for (int i = 0; i <= frameCount; i++)
      {
        auto animRing = new QPropertyAnimation(aRing, QByteArrayLiteral("position"));
        animRing->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
        animRing->setStartValue(vParabolic.at(i));
        animRing->setEndValue(vParabolic.at(i + 1));
        animRing->setLoopCount(1);
        animGroup->addAnimation(animRing);
      }
      break;
    case panCake:
      aRing->setRotY(rotY);
      aRing->setRotX(RING_PANCAKE_ROTX);
      for (int i = 0; i <= frameCount; i++)
      {
        auto animRing = new QPropertyAnimation(aRing, QByteArrayLiteral("position"));
        animRing->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
        animRing->setStartValue(vParabolic.at(i));
        animRing->setEndValue(vParabolic.at(i + 1));
        animRing->setLoopCount(1);
        animTranslationGroup->addAnimation(animRing);
      }
      rotCount = (int)(launch / 2);
      if (rotCount)
      {
        animRotProp = new QPropertyAnimation(aRing, QByteArrayLiteral("rotX"));
        animRotProp->setDuration(((int)(DELTA_TIME * 600)) * (frameCount + 1) / rotCount);
        animRotProp->setStartValue(360 + RING_PANCAKE_ROTX);
        animRotProp->setEndValue(RING_PANCAKE_ROTX);
        animRotProp->setLoopCount(rotCount);
        animTempGroup->addAnimation(animRotProp);
      }
      animTempGroup->addAnimation(animTranslationGroup);
      animTempGroup->setLoopCount(1);
      animGroup->addAnimation(animTempGroup);
      break;
    }
    break;

  case club:
    aClub = vClub.at(indexProp);
    // set rotY to align with juggler
    rotY = juggler->getRotY();

    // switch between types
    launchType = aClub->getLaunchType();
    switch (launchType)
    {
    case normalClub:
      aClub->setRotX(CLUB_BASIC_ROTX);
      aClub->setRotY(rotY);
      for (int i = 0; i <= frameCount; i++)
      {
        auto animClub = new QPropertyAnimation(aClub, QByteArrayLiteral("position"));
        animClub->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
        animClub->setStartValue(vParabolic.at(i));
        animClub->setEndValue(vParabolic.at(i + 1));
        animClub->setLoopCount(1);
        animTranslationGroup->addAnimation(animClub);
      }
      rotCount = (int)(launch / 2);
      if (rotCount)
      {
        animRotProp = new QPropertyAnimation(aClub, QByteArrayLiteral("rotX"));
        animRotProp->setDuration(((int)(DELTA_TIME * 600)) * (frameCount + 1) / rotCount);
        animRotProp->setStartValue(360 + CLUB_BASIC_ROTX);
        animRotProp->setEndValue(CLUB_BASIC_ROTX);
        animRotProp->setLoopCount(rotCount);
        animTempGroup->addAnimation(animRotProp);
      }
      animTempGroup->addAnimation(animTranslationGroup);
      animTempGroup->setLoopCount(1);
      animGroup->addAnimation(animTempGroup);
      break;

    case flat:
      aClub->setRotX(CLUB_BASIC_ROTX);
      aClub->setRotY(rotY);
      for (int i = 0; i <= frameCount; i++)
      {
        auto animClub = new QPropertyAnimation(aClub, QByteArrayLiteral("position"));
        animClub->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
        animClub->setStartValue(vParabolic.at(i));
        animClub->setEndValue(vParabolic.at(i + 1));
        animClub->setLoopCount(1);
        animGroup->addAnimation(animClub);
      }
      break;

    case helicopter:
      // on oriente la massue suivant la main qui lance
      aClub->setRotX(CLUB_HELICOPTER_ROTX);
      if (aHand == leftHand)
        aClub->setRotY(rotY + 90);
      else
        aClub->setRotY(rotY - 90);
      for (int i = 0; i <= frameCount; i++)
      {
        auto animClub = new QPropertyAnimation(aClub, QByteArrayLiteral("position"));
        animClub->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
        animClub->setStartValue(vParabolic.at(i));
        animClub->setEndValue(vParabolic.at(i + 1));
        animClub->setLoopCount(1);
        animTranslationGroup->addAnimation(animClub);
      }
      fRotCount = (float)((float)launch / 2.0f);
      animRotProp = new QPropertyAnimation(aClub, QByteArrayLiteral("rotY"));
      animRotProp->setDuration(((int)(DELTA_TIME * 600)) * (frameCount + 1));
      if (aHand == leftHand)
      {
        animRotProp->setStartValue(rotY + 90);
        animRotProp->setEndValue(rotY + 90 - (fRotCount * 360));
      }
      else
      {
        animRotProp->setStartValue(rotY - 90);
        animRotProp->setEndValue(rotY - 90 + (fRotCount * 360));
      }
      animRotProp->setLoopCount(1);
      animTempGroup->addAnimation(animRotProp);
      animTempGroup->addAnimation(animTranslationGroup);
      animTempGroup->setLoopCount(1);
      animGroup->addAnimation(animTempGroup);
      break;

    default: break;
    }

  default: break;
  }

  // now we handle curve in the hand
  // surely it's the right place to begin
  posProp = posFinal;

  // find out rotation center
  QVector3D centerCurve;
  if (receiveHand == leftHand)
  {
    //    centerCurve = (posBall + aJuggler->getPositionLHint()) / 2;
    centerCurve = (aJuggler->getPositionLHext() + aJuggler->getPositionLHint()) / 2;
    posProp = aJuggler->getPositionLHext();
  }
  else
  {
    //    centerCurve = (posBall + aJuggler->getPositionRHint()) / 2;
    centerCurve = (aJuggler->getPositionRHext() + aJuggler->getPositionRHint()) / 2;
    posProp = aJuggler->getPositionRHext();
  }

  // determine axis for rotation
  rotY = juggler->getRotY(); // pas bon en cas d'helico...
  QVector3D axisCurve = QVector3D(0, 0, 1);
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(rotY, QVector3D(0, 1, 0));
  axisCurve = rotAxis * axisCurve;

  // determine number of frames
  frameCount = (int)((DWELL_TIME / DELTA_TIME) + 1);

  // determine angles for each delta animation
  float deltaAngles = (float)(180 / frameCount);

  QPropertyAnimation *animProp;

  switch (propType)
  {
  case ball:
    // loop creates all our animations for dwell time
    for (int i = 0; i <= frameCount; i++)
    {
      animProp = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
      animProp->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
      animProp->setStartValue(posProp);
      // handle rotation stuff
      QMatrix4x4 rot;
      rot.setToIdentity();
      rot.translate(centerCurve);
      if (receiveHand == leftHand)
        rot.rotate(-deltaAngles, axisCurve);
      else
        rot.rotate(deltaAngles, axisCurve);
      rot.translate(-centerCurve);
      QVector3D posBall2 = rot * posProp;

      animProp->setEndValue(posBall2);
      animProp->setLoopCount(1);
      animGroup->addAnimation(animProp);
      posProp = posBall2;
    }
    break;
  case ring:
    // loop creates all our animations for dwell time
    for (int i = 0; i <= frameCount; i++)
    {
      animProp = new QPropertyAnimation(aRing, QByteArrayLiteral("position"));
      animProp->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
      animProp->setStartValue(posProp);
      // handle rotation stuff
      QMatrix4x4 rot;
      rot.setToIdentity();
      rot.translate(centerCurve);
      if (receiveHand == leftHand)
        rot.rotate(-deltaAngles, axisCurve);
      else
        rot.rotate(deltaAngles, axisCurve);
      rot.translate(-centerCurve);
      QVector3D posBall2 = rot * posProp;

      animProp->setEndValue(posBall2);
      animProp->setLoopCount(1);
      animGroup->addAnimation(animProp);
      posProp = posBall2;
    }
    break;
  case club:
    // loop creates all our animations for dwell time
    for (int i = 0; i <= frameCount; i++)
    {
      animProp = new QPropertyAnimation(aClub, QByteArrayLiteral("position"));
      animProp->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
      animProp->setStartValue(posProp);
      // handle rotation stuff
      QMatrix4x4 rot;
      rot.setToIdentity();
      rot.translate(centerCurve);
      if (receiveHand == leftHand)
        rot.rotate(-deltaAngles, axisCurve);
      else
        rot.rotate(deltaAngles, axisCurve);
      rot.translate(-centerCurve);
      QVector3D posBall2 = rot * posProp;

      animProp->setEndValue(posBall2);
      animProp->setLoopCount(1);
      animGroup->addAnimation(animProp);
      posProp = posBall2;
    }
    break;
  default: break;
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
