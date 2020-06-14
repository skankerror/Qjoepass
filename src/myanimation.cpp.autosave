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

#include "myanimation.h"
#include "mycurves.h"

MyAnimation::MyAnimation(QObject *parent)
  : QObject(parent),
    siteswapAnimation(new QParallelAnimationGroup())
{}

void MyAnimation::setSiteSwap(SiteSwap *aSiteSwap)
{
  siteSwap = aSiteSwap;
  propType = siteSwap->getPropType();
}

void MyAnimation::setAnim()
{
  if (!(siteSwap->isValid())) // ça marche pas...
  {
    qDebug() << "Siteswap isn't valid";
    return;
  }

  period = siteSwap->getPeriod();
  QBitArray state = siteSwap->getState();
  int propNum = 0;

  for (int i = 0; i < state.size(); i++) // for each bit in state
  {
    if (state.testBit(i)) // if it's a site launch
    {
      int launchPos = i % period; // i may be beyond period
      int launch = siteSwap->at(launchPos);
      hand launchHand;
      auto propMoveAnim = new QSequentialAnimationGroup(); // will handle the whole moving
      auto propGlobAnim = new QSequentialAnimationGroup();// needed to add pause at the beginning

      // we had delay for each prop
      // we set a delay even for first launch so that we can anim the hand before launch
      float delay = (HAND_PERIOD / 2) * i;
      if (launch == 1) // if launch is 1 time is shorter
        propGlobAnim->addPause((DWELL_TIME_LAUNCH1 + delay) * S_TO_MS);
      else
        propGlobAnim->addPause((DWELL_TIME + delay) * S_TO_MS);

      // let's go
      (i % 2 == 0) ? launchHand = rightHand : launchHand = leftHand; // i is odd or even

      auto launchAnim = parabolicAnim(juggler, propNum, launch, launchHand);
      launchAnim->setLoopCount(1);
      propMoveAnim->addAnimation(launchAnim);

      // search for new site
      int newLaunchPos = (launch + launchPos) % period;

      // setting the new hand
      // if launch is odd we change hand
      hand newLaunchHand;
      (launch % 2 == 1) ? newLaunchHand = changeHand(launchHand) : newLaunchHand = launchHand;
      // get new launch to know if it's a 1
      int newLaunch = siteSwap->at(newLaunchPos);

      // we set dwell animation
      auto dwellAnimation = dwellAnim(juggler, propNum, newLaunch, newLaunchHand);
      dwellAnimation->setLoopCount(1);
      propMoveAnim->addAnimation(dwellAnimation);

      // if we're not on the same site, let's keep on animate
      while (newLaunchPos != launchPos)
      {
        auto followLaunchAnim = parabolicAnim(juggler, propNum, newLaunch, newLaunchHand);
        followLaunchAnim->setLoopCount(1);
        propMoveAnim->addAnimation(followLaunchAnim);

        // is it the other hand ?
        if (newLaunch % 2 == 1)
          newLaunchHand = changeHand(newLaunchHand);
        newLaunchPos = (newLaunch + newLaunchPos) % period;
        newLaunch = siteSwap->at(newLaunchPos);
        // set another dwell animation
        dwellAnimation = dwellAnim(juggler, propNum, newLaunch, newLaunchHand);
        dwellAnimation->setLoopCount(1);
        propMoveAnim->addAnimation(dwellAnimation);
      }

      // now we get back to initial site, if we changed hand, let's do the whole thing again
      if (newLaunchHand != launchHand)
      {
        auto backLaunchAnim = parabolicAnim(juggler, propNum, launch, newLaunchHand);
        backLaunchAnim->setLoopCount(1);
        propMoveAnim->addAnimation(backLaunchAnim);

        newLaunchPos = (launch + launchPos) % period;
        if (launch % 2 == 1)
          newLaunchHand = changeHand(newLaunchHand);
        newLaunch = siteSwap->at(newLaunchPos);

        dwellAnimation = dwellAnim(juggler, propNum, newLaunch, newLaunchHand);
        dwellAnimation->setLoopCount(1);
        propMoveAnim->addAnimation(dwellAnimation);

        while (newLaunchPos != launchPos)
        {
          auto backFollowLaunchAnim = parabolicAnim(juggler, propNum, newLaunch, newLaunchHand);
          backFollowLaunchAnim->setLoopCount(1);
          propMoveAnim->addAnimation(backFollowLaunchAnim);

          if (newLaunch % 2 == 1)
            newLaunchHand = changeHand(newLaunchHand);
          newLaunchPos = (newLaunch + newLaunchPos) % period;
          newLaunch = siteSwap->at(newLaunchPos);

          dwellAnimation = dwellAnim(juggler, propNum, newLaunch, newLaunchHand);
          dwellAnimation->setLoopCount(1);
          propMoveAnim->addAnimation(dwellAnimation);
        }
      }
      // we add to the anim containing starting pause
      propGlobAnim->addAnimation(propMoveAnim);
      propMoveAnim->setLoopCount(-1);
      siteswapAnimation->addAnimation(propGlobAnim); // and we add to the global parallel anim
      propNum++;
    }
  }
}

void MyAnimation::startAnimation()
{
  siteswapAnimation->start();
}

void MyAnimation::stopAnimation()
{
  siteswapAnimation->stop();
  siteswapAnimation->clear();
}

QSequentialAnimationGroup *MyAnimation::parabolicAnim(Juggler *aJuggler,
                                                      int indexProp,
                                                      int launch,
                                                      hand aHand)
{
  auto animGroup = new QSequentialAnimationGroup();
  QVector3D posProp; // pos where it starts
  QVector3D posFinal; // pos where it should finish

  // odd launches
  if (aHand == leftHand && launch % 2 != 0)
  {
    posProp = aJuggler->getPositionLHint();
    posFinal = aJuggler->getPositionRHext();
  }
  if (aHand == rightHand && launch % 2 != 0)
  {
    posProp = aJuggler->getPositionRHint();
    posFinal = aJuggler->getPositionLHext();
  }

  // even launches
  if (aHand == leftHand && launch % 2 == 0)
  {
    posProp = aJuggler->getPositionLHint();
    posFinal = aJuggler->getPositionLHext();
  }
  if (aHand == rightHand && launch % 2 == 0)
  {
    posProp = aJuggler->getPositionRHint();
    posFinal = aJuggler->getPositionRHext();
  }

  // Shannon theorem
  float arcTime;
  if (launch == 1) // For launch 1 Shannon doesn't work
  {
    arcTime = LAUNCH1_TIME;
//    qDebug() << "arcTime" << arcTime;
  }
  else // thanks Claude Shannon
    arcTime = ((HAND_PERIOD) / 2) * (launch - (2 * DWELL_RATIO));

  // we calculate velocity launch
  QVector3D velBall = ((posFinal - posProp) - 0.5 *
                       (GRAVITY * arcTime * arcTime)) / arcTime;

  // By counting frames we add 1 due to float to integer approx.
  int frameCount = (int)((arcTime / (DELTA_TIME)) /*+ 1*/);
//  if (launch == 1) qDebug() << "frameCount" << frameCount;

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
  float rotY; // to handle different positions between normal, flat, helicopter

  switch(propType)
  {
  case ball:
    aBall = vBall.at(indexProp);
    // loop creates all our animations for translation
    for (int i = 0; i < frameCount; i++)
    {
      auto animBall = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
      animBall->setDuration((int)(DELTA_TIME * S_TO_MS));
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
      for (int i = 0; i < frameCount; i++)
      {
        auto animRing = new QPropertyAnimation(aRing, QByteArrayLiteral("position"));
        animRing->setDuration((int)(DELTA_TIME * S_TO_MS));
        animRing->setStartValue(vParabolic.at(i));
        animRing->setEndValue(vParabolic.at(i + 1));
        animRing->setLoopCount(1);
        animGroup->addAnimation(animRing);
      }
      break;
    case panCake:
      aRing->setRotY(rotY);
      aRing->setRotX(RING_PANCAKE_ROTX);
      for (int i = 0; i < frameCount; i++)
      {
        auto animRing = new QPropertyAnimation(aRing, QByteArrayLiteral("position"));
        animRing->setDuration((int)(DELTA_TIME * S_TO_MS));
        animRing->setStartValue(vParabolic.at(i));
        animRing->setEndValue(vParabolic.at(i + 1));
        animRing->setLoopCount(1);
        animTranslationGroup->addAnimation(animRing);
      }
      rotCount = (int)(launch / 2);
      if (rotCount)
      {
        animRotProp = new QPropertyAnimation(aRing, QByteArrayLiteral("rotX"));
        animRotProp->setDuration(((int)(DELTA_TIME * S_TO_MS)) * (frameCount + 1) / rotCount);
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
      for (int i = 0; i < frameCount; i++)
      {
        auto animClub = new QPropertyAnimation(aClub, QByteArrayLiteral("position"));
        animClub->setDuration((int)(DELTA_TIME * S_TO_MS));
        animClub->setStartValue(vParabolic.at(i));
        animClub->setEndValue(vParabolic.at(i + 1));
        animClub->setLoopCount(1);
        animTranslationGroup->addAnimation(animClub);
      }
      rotCount = (int)(launch / 2);
      if (rotCount)
      {
        animRotProp = new QPropertyAnimation(aClub, QByteArrayLiteral("rotX"));
        animRotProp->setDuration(((int)(DELTA_TIME * S_TO_MS)) * (frameCount + 1) / rotCount);
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
      for (int i = 0; i < frameCount; i++)
      {
        auto animClub = new QPropertyAnimation(aClub, QByteArrayLiteral("position"));
        animClub->setDuration((int)(DELTA_TIME * S_TO_MS));
        animClub->setStartValue(vParabolic.at(i));
        animClub->setEndValue(vParabolic.at(i + 1));
        animClub->setLoopCount(1);
        animGroup->addAnimation(animClub);
      }
      break;

    case helicopter:
      // we set rotX depending launching hand
      aClub->setRotX(CLUB_HELICOPTER_ROTX);
      if (aHand == leftHand)
        aClub->setRotY(rotY + 90);
      else
        aClub->setRotY(rotY - 90);
      for (int i = 0; i < frameCount; i++)
      {
        auto animClub = new QPropertyAnimation(aClub, QByteArrayLiteral("position"));
        animClub->setDuration((int)(DELTA_TIME * S_TO_MS));
        animClub->setStartValue(vParabolic.at(i));
        animClub->setEndValue(vParabolic.at(i + 1));
        animClub->setLoopCount(1);
        animTranslationGroup->addAnimation(animClub);
      }
      fRotCount = (float)((float)launch / 2.0f);
      animRotProp = new QPropertyAnimation(aClub, QByteArrayLiteral("rotY"));
      animRotProp->setDuration(((int)(DELTA_TIME * S_TO_MS)) * (frameCount + 1));
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
  return animGroup;
}

QSequentialAnimationGroup *MyAnimation::dwellAnim(Juggler *aJuggler,
                                                  int indexProp,
                                                  int nextLaunch,
                                                  hand aHand)
{
  // set recieve pos
  QVector3D pos;
  // set next launch pos
  QVector3D pos2;
  // set center curve
  QVector3D centerCurve;
  auto returnAnim = new QSequentialAnimationGroup();

  if (aHand == leftHand)
  {
    pos = aJuggler->getPositionLHext();
    pos2 = aJuggler->getPositionLHint();
  }
  else
  {
    pos = aJuggler->getPositionRHext();
    pos2 = aJuggler->getPositionRHint();
  }
  centerCurve = (pos + pos2) / 2;

  // declare before switch cos I don't want to be insulted
  QPropertyAnimation *dwellAnimation;
  JugglingBall *aBall;
  JugglingRing *aRing;
  Pirouette *aClub;

  if (nextLaunch == 1) // that's special for launch 1
  {
    switch (propType)
    {
    case ball:
      aBall = vBall.at(indexProp);
      dwellAnimation = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
      dwellAnimation->setDuration((int)(DWELL_TIME_LAUNCH1 * S_TO_MS));
      dwellAnimation->setStartValue(pos);
      dwellAnimation->setEndValue(pos2);
      // mettre une easing curve ?
      dwellAnimation->setLoopCount(1);
      returnAnim->addAnimation(dwellAnimation);
      break;
    case ring:
      aRing = vRing.at(indexProp);
      dwellAnimation = new QPropertyAnimation(aRing, QByteArrayLiteral("position"));
      dwellAnimation->setDuration((int)(DWELL_TIME_LAUNCH1 * S_TO_MS));
      dwellAnimation->setStartValue(pos);
      dwellAnimation->setEndValue(pos2);
      // mettre une easing curve ?
      dwellAnimation->setLoopCount(1);
      returnAnim->addAnimation(dwellAnimation);
      break;
    case club:
      aClub = vClub.at(indexProp);
      dwellAnimation = new QPropertyAnimation(aClub, QByteArrayLiteral("position"));
      dwellAnimation->setDuration((int)(DWELL_TIME_LAUNCH1 * S_TO_MS));
      dwellAnimation->setStartValue(pos);
      dwellAnimation->setEndValue(pos2);
      // mettre une easing curve ?
      dwellAnimation->setLoopCount(1);
      returnAnim->addAnimation(dwellAnimation);
      break;
    default:
      break;
    }
    return returnAnim;
  }

  // determine axis for rotation
  float rotY = juggler->getRotY();
  QVector3D axisCurve = QVector3D(0, 0, 1);
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(rotY, QVector3D(0, 1, 0));
  axisCurve = rotAxis * axisCurve;

  // determine number of frames
  int frameCount = (int)((DWELL_TIME / DELTA_TIME) + 1);
  // determine angles for each delta animation
  float deltaAngles = (float)(180 / frameCount);
  switch (propType)
  {
  case ball:
    aBall = vBall.at(indexProp);
    // loop creates all our animations for dwell time
    for (int i = 0; i <= frameCount; i++)
    {
      dwellAnimation = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
      dwellAnimation->setDuration((int)(DELTA_TIME * S_TO_MS));
      dwellAnimation->setStartValue(pos);
      // handle rotation stuff
      QMatrix4x4 rot;
      rot.setToIdentity();
      rot.translate(centerCurve);
      if (aHand == leftHand)
        rot.rotate(-deltaAngles, axisCurve);
      else
        rot.rotate(deltaAngles, axisCurve);
      rot.translate(-centerCurve);
      QVector3D posBall2 = rot * pos;

      dwellAnimation->setEndValue(posBall2);
      dwellAnimation->setLoopCount(1);
      returnAnim->addAnimation(dwellAnimation);
      pos = posBall2;
    }
    break;
  case ring:
    aRing = vRing.at(indexProp);
    // loop creates all our animations for dwell time
    for (int i = 0; i <= frameCount; i++)
    {
      dwellAnimation = new QPropertyAnimation(aRing, QByteArrayLiteral("position"));
      dwellAnimation->setDuration((int)(DELTA_TIME * S_TO_MS));
      dwellAnimation->setStartValue(pos);
      // handle rotation stuff
      QMatrix4x4 rot;
      rot.setToIdentity();
      rot.translate(centerCurve);
      if (aHand == leftHand)
        rot.rotate(-deltaAngles, axisCurve);
      else
        rot.rotate(deltaAngles, axisCurve);
      rot.translate(-centerCurve);
      QVector3D posBall2 = rot * pos;

      dwellAnimation->setEndValue(posBall2);
      dwellAnimation->setLoopCount(1);
      returnAnim->addAnimation(dwellAnimation);
      pos = posBall2;
    }
    break;
  case club:
    aClub = vClub.at(indexProp);
    // loop creates all our animations for dwell time
    for (int i = 0; i <= frameCount; i++)
    {
      dwellAnimation = new QPropertyAnimation(aClub, QByteArrayLiteral("position"));
      dwellAnimation->setDuration((int)(DELTA_TIME * S_TO_MS));
      dwellAnimation->setStartValue(pos);
      // handle rotation stuff
      QMatrix4x4 rot;
      rot.setToIdentity();
      rot.translate(centerCurve);
      if (aHand == leftHand)
        rot.rotate(-deltaAngles, axisCurve);
      else
        rot.rotate(deltaAngles, axisCurve);
      rot.translate(-centerCurve);
      QVector3D posBall2 = rot * pos;

      dwellAnimation->setEndValue(posBall2);
      dwellAnimation->setLoopCount(1);
      returnAnim->addAnimation(dwellAnimation);
      pos = posBall2;
    }
    break;
  default: break;
  }
  return returnAnim;
}

hand MyAnimation::changeHand(hand aHand)
{
  return (aHand == leftHand) ? rightHand : leftHand;
}
