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

#include "animation.h"
#include "curve.h"

Animation::Animation(QObject *parent)
  : QParallelAnimationGroup(parent)
{}

void Animation::setSiteSwap(SiteSwap *aSiteSwap)
{
  siteSwap = aSiteSwap;
  propType = siteSwap->getPropType();
}

void Animation::setAnim()
{
  if (!(siteSwap->isValid()))
  {
    qDebug() << "Siteswap isn't valid";
    return;
  }
  
  period = siteSwap->getPeriod();
  QBitArray state = siteSwap->getState();
  int propNum = 0;
  
  auto handPauseAnim = new QSequentialAnimationGroup();
  QParallelAnimationGroup *animTempGroup = new QParallelAnimationGroup();
  
  auto rightHandAnimation = handAnim(vJuggler.at(0), propNum, 1, rightHand);
  rightHandAnimation->setLoopCount(-1);
  animTempGroup->addAnimation(rightHandAnimation);
  
  handPauseAnim->addPause((HAND_PERIOD / 2) * S_TO_MS);
  auto leftHandAnimation = handAnim(vJuggler.at(0), propNum, 1, leftHand);
  handPauseAnim->addAnimation(leftHandAnimation);
  leftHandAnimation->setLoopCount(-1);
  animTempGroup->addAnimation(handPauseAnim);
  
  // pas beau
//  auto rightHandAnimation2 = handAnim(vJuggler.at(1), propNum, 1, rightHand);
//  rightHandAnimation2->setLoopCount(-1);
//  animTempGroup->addAnimation(rightHandAnimation2);
  
//  handPauseAnim->addPause((HAND_PERIOD / 2) * S_TO_MS);
//  auto leftHandAnimation2 = handAnim(vJuggler.at(1), propNum, 1, leftHand);
//  handPauseAnim->addAnimation(leftHandAnimation2);
//  leftHandAnimation2->setLoopCount(-1);
//  animTempGroup->addAnimation(handPauseAnim);
  
  
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
      // single juggler
      int jugId = 0;
      
      QVector<AnimEvent*> v_animEvent = siteSwap->getAnimEvents(launchPos, launchHand, jugId);
      
      auto juggler = vJuggler.at(0); // pas beau
      
      for (int j = 0; j < v_animEvent.size(); j++)
      {
        auto animEvent = v_animEvent.at(j);
        auto launchAnim = parabolicAnim(juggler, propNum, animEvent->getLaunch(), animEvent->getHandLaunch());
        launchAnim->setLoopCount(1);
        propMoveAnim->addAnimation(launchAnim);
        // dwell
        auto dwellAnimation = dwellAnim(juggler, propNum, animEvent->getNewLaunch(), animEvent->getHandRecieve());
        dwellAnimation->setLoopCount(1);
        propMoveAnim->addAnimation(dwellAnimation);
      }
      // we add to the anim containing starting pause
      propGlobAnim->addAnimation(propMoveAnim);
      propMoveAnim->setLoopCount(-1);
      animTempGroup->addAnimation(propGlobAnim); // and we add to the global parallel anim
      
      if (siteSwap->getJugglerCount() > 1) // pas beau
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
        // single juggler
        int jugId = 0;
        
        QVector<AnimEvent*> v_animEvent = siteSwap->getAnimEvents(launchPos, launchHand, jugId);
        
        juggler = vJuggler.at(1); // pas beau
        for (int j = 0; j < v_animEvent.size(); j++)
        {
          // juggler peut changer dans le merdier
          
          auto animEvent = v_animEvent.at(j);
          auto launchAnim = parabolicAnim(vJuggler.at(animEvent->getJugLaunchId()), propNum, animEvent->getLaunch(), animEvent->getHandLaunch());
          launchAnim->setLoopCount(1);
          propMoveAnim->addAnimation(launchAnim);
          // dwell
          auto dwellAnimation = dwellAnim(vJuggler.at(animEvent->getJugLaunchId()), propNum, animEvent->getNewLaunch(), animEvent->getHandRecieve());
          dwellAnimation->setLoopCount(1);
          propMoveAnim->addAnimation(dwellAnimation);
        }
        // we add to the anim containing starting pause
        propGlobAnim->addAnimation(propMoveAnim);
        propMoveAnim->setLoopCount(-1);
        animTempGroup->addAnimation(propGlobAnim); // and we add to the global parallel anim
        
      }
      propNum++;
    }
  }
  addAnimation(animTempGroup);
}

//QPropertyAnimation handPropAnim(Juggler *aJuggler, hand aHand, bool motion)
//{
//  QByteArray  hand;
//  if (aHand == leftHand) hand =  QByteArrayLiteral("leftHandPosition");
//  else hand =  QByteArrayLiteral("RightHandPosition");
//  auto  animHandDwell = new QPropertyAnimation(aJuggler, hand);
//  return animHandDwell;
//}

QSequentialAnimationGroup *Animation::handAnim(Juggler *aJuggler,
                                               int indexProp,
                                               int launch,
                                               hand aHand)
{
  //define animations for one hand
  QPropertyAnimation *dwellAnimation;
  QPropertyAnimation *emptyHandAnimation;
  // set receive pos
  QVector3D pos;
  // set next frame pos
  QVector3D pos2;
  // bool to know if we need to enlarge our juggling
  int emptyHandTime = (HAND_PERIOD - DWELL_TIME)* S_TO_MS;

  int frameCount = (int)((DWELL_TIME / DELTA_TIME));
  float dwellTime;

//  qDebug() << "frameCount : " << frameCount;
//  qDebug() << "empty Hand Time : " << emptyHandTime;

//  if (launch == 1) // that's special for launch 1
//  {
//    dwellTime = (int)(DWELL_TIME_LAUNCH1 * S_TO_MS);
//  }
  dwellTime = (int)(DELTA_TIME * S_TO_MS);
  // determine axis for rotation
  float rotY = aJuggler->getRotY();

  QVector3D axisCurve = QVector3D(0, 0, 1);
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(rotY, QVector3D(0, 1, 0));
  axisCurve = rotAxis * axisCurve;

  auto animGroup = new QSequentialAnimationGroup();

  float deltaAngles;

  if (aHand == leftHand)
  {
    deltaAngles = (float)(180 / frameCount);
    pos = aJuggler->getPositionLHext();
    pos2 = aJuggler->getPositionLHint();
    emptyHandAnimation = new QPropertyAnimation(aJuggler, QByteArrayLiteral("leftHandPosition"));
  }
  else
  {
    deltaAngles = (float)(-180 / frameCount);
    emptyHandAnimation = new QPropertyAnimation(aJuggler, QByteArrayLiteral("rightHandPosition"));
    pos = aJuggler->getPositionRHext();
    pos2 = aJuggler->getPositionRHint();
  }

  QVector3D centerCurve = (pos + pos2) / 2;
  QVector3D posBall1 = pos;

  for (int i = 0; i < frameCount; i++)
  {
    if (aHand == leftHand) dwellAnimation = new QPropertyAnimation(aJuggler, QByteArrayLiteral("leftHandPosition"));
    else dwellAnimation = new QPropertyAnimation(aJuggler, QByteArrayLiteral("rightHandPosition"));
    dwellAnimation->setDuration(dwellTime);
    dwellAnimation->setStartValue(posBall1);
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.translate(centerCurve);
    rot.rotate(deltaAngles, axisCurve);
    rot.translate(-centerCurve);
    QVector3D posBall2 = rot * posBall1;
    dwellAnimation->setEndValue(posBall2);
    dwellAnimation->setLoopCount(1);
    animGroup->addAnimation(dwellAnimation);
//    qDebug() << "hand : " << aHand;
//    qDebug() << "handPos : " << aJuggler->getPositionLHmed();
//    qDebug() << "handPos : " << aJuggler->getPositionRHmed();
    posBall1 = posBall2;
  }

  emptyHandAnimation->setDuration(emptyHandTime);
  emptyHandAnimation->setStartValue(pos2);
  emptyHandAnimation->setEndValue(pos);
  emptyHandAnimation->setLoopCount(1);
  animGroup->addAnimation(emptyHandAnimation);
  // time adjustments
  float duration = DELTA_TIME * frameCount;
  float decay = DWELL_TIME - duration;
  int intDecay = (int)(decay * S_TO_MS);
  if (intDecay)
    animGroup->addPause(intDecay);
  return animGroup;
}

QSequentialAnimationGroup *Animation::parabolicAnim(Juggler *aJuggler, // mettre 2 jugglers
                                                    int indexProp,
                                                    int launch,
                                                    hand aHand)
{
  auto animGroup = new QSequentialAnimationGroup();
  QVector3D posProp; // pos where it starts
  QVector3D posFinal; // pos where it should finish
  
  // bool to know if we need to enlarge our juggling
  bool isExtPlusCatch = (propType == ring  && siteSwap->getLaunchType() == panCake) ||
      (propType == club && siteSwap->getLaunchType() == helicopter);
  
  // odd launches
  if (aHand == leftHand && launch % 2 != 0)
  {
    posProp = aJuggler->getPositionLHint();
    if (isExtPlusCatch)
      posFinal = aJuggler->getPositionRHextPlus();
    else
      posFinal = aJuggler->getPositionRHext();
  }
  if (aHand == rightHand && launch % 2 != 0)
  {
    posProp = aJuggler->getPositionRHint();
    if (isExtPlusCatch)
      posFinal = aJuggler->getPositionLHextPlus();
    else
      posFinal = aJuggler->getPositionLHext();
  }
  
  // even launches
  if (aHand == leftHand && launch % 2 == 0)
  {
    posProp = aJuggler->getPositionLHint();
    if (isExtPlusCatch)
      posFinal = aJuggler->getPositionLHextPlus();
    else
      posFinal = aJuggler->getPositionLHext();
  }
  if (aHand == rightHand && launch % 2 == 0)
  {
    posProp = aJuggler->getPositionRHint();
    if (isExtPlusCatch)
      posFinal = aJuggler->getPositionRHextPlus();
    else
      posFinal = aJuggler->getPositionRHext();
  }
  
  // Shannon theorem
  float arcTime;
  if (launch == 1) // For launch 1 Shannon doesn't work
  {
    arcTime = LAUNCH1_TIME;
  }
  else // thanks Claude Shannon
    arcTime = ((HAND_PERIOD) / 2) * (launch - (2 * DWELL_RATIO));
  
  // we calculate velocity launch
  QVector3D velBall = ((posFinal - posProp) - 0.5 *
                       (GRAVITY * arcTime * arcTime)) / arcTime;
  
  int frameCount = (int)((arcTime / (DELTA_TIME)));
  
  // We create our curve
  QVector<QVector3D> vParabolic = Curves::curveParabolic(velBall, posProp, frameCount);
  
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
  
  qDebug() << "prop launchTime : " << (int)(frameCount * (int)(DELTA_TIME * S_TO_MS));
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
    rotY = aJuggler->getRotY();
    
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
        animRotProp->setDuration(((int)(DELTA_TIME * S_TO_MS)) * (frameCount) / rotCount);
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
    rotY = aJuggler->getRotY();
    
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
        animRotProp->setDuration(((int)(DELTA_TIME * S_TO_MS)) * (frameCount) / rotCount);
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
      // we set rotY depending launching hand
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
      animRotProp->setDuration(((int)(DELTA_TIME * S_TO_MS)) * (frameCount));
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
  // Time adjustment if necessary
  float decay = arcTime - (DELTA_TIME * frameCount);
  int intDecay = decay * S_TO_MS;
  if (intDecay)
    animGroup->addPause(intDecay);
  return animGroup;
}

QSequentialAnimationGroup *Animation::dwellAnim(Juggler *aJuggler,
                                                int indexProp,
                                                int nextLaunch,
                                                hand aHand)
{
  // set receive pos
  QVector3D pos;
  // set next launch pos
  QVector3D pos2;
  // set center curve
  QVector3D centerCurve;
  // bool to know if we need to enlarge our juggling
  bool isExtPlusCatch = (propType == ring && siteSwap->getLaunchType() == panCake) ||
      (propType == club && siteSwap->getLaunchType() == helicopter);
  
  auto returnAnim = new QSequentialAnimationGroup();
  
  if (aHand == leftHand)
  {
    if (isExtPlusCatch)
      pos = aJuggler->getPositionLHextPlus();
    else
      pos = aJuggler->getPositionLHext();
    pos2 = aJuggler->getPositionLHint();
  }
  else
  {
    if (isExtPlusCatch)
      pos = aJuggler->getPositionRHextPlus();
    else
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
    default: case ball:
      aBall = vBall.at(indexProp);
      dwellAnimation = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
      break;
    case ring:
      aRing = vRing.at(indexProp);
      dwellAnimation = new QPropertyAnimation(aRing, QByteArrayLiteral("position"));
      break;
    case club:
      aClub = vClub.at(indexProp);
      dwellAnimation = new QPropertyAnimation(aClub, QByteArrayLiteral("position"));
      break;
    }
    dwellAnimation->setDuration((int)(DWELL_TIME_LAUNCH1 * S_TO_MS));
    dwellAnimation->setStartValue(pos);
    dwellAnimation->setEndValue(pos2);
    // mettre une easing curve ?
    dwellAnimation->setLoopCount(1);
    returnAnim->addAnimation(dwellAnimation);
    
    return returnAnim;
  }
  
  // determine axis for rotation
  float rotY = aJuggler->getRotY();
  QVector3D axisCurve = QVector3D(0, 0, 1);
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(rotY, QVector3D(0, 1, 0));
  axisCurve = rotAxis * axisCurve;
  
  // determine number of frames
  int frameCount = (int)((DWELL_TIME / DELTA_TIME));
  // determine angles for each delta animation
  float deltaAngles = (float)(180 / frameCount);
  switch (propType)
  {
  default: case ball:
    aBall = vBall.at(indexProp);
    break;
  case ring:
    aRing = vRing.at(indexProp);
    break;
  case club:
    aClub = vClub.at(indexProp);
    break;
  }
  // loop creates all our animations for dwell time
  for (int i = 0; i < frameCount; i++)
  {
    switch (propType)
    {
    default: case ball:
      dwellAnimation = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
      break;
    case ring:
      dwellAnimation = new QPropertyAnimation(aRing, QByteArrayLiteral("position"));
      break;
    case club:
      dwellAnimation = new QPropertyAnimation(aClub, QByteArrayLiteral("position"));
      break;
    }
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
  
  // time adjustments
  float duration = DELTA_TIME * frameCount;
  float decay = DWELL_TIME - duration;
  int intDecay = (int)(decay * S_TO_MS);
  if (intDecay)
    returnAnim->addPause(intDecay);
  return returnAnim;
}
