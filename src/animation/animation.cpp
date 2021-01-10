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

void Animation::setSiteSwap(SiteSwap *t_siteSwap)
{
  m_siteSwap = t_siteSwap;
  m_propType = m_siteSwap->getPropType();
}

void Animation::setAnim()
{
  if (!(m_siteSwap->isValid()))
  {
    qDebug() << "Siteswap isn't valid";
    return;
  }

  m_period = m_siteSwap->getPeriod();
  QBitArray state = m_siteSwap->getState();
  int propNum = 0;

  auto handPauseAnim = new QSequentialAnimationGroup();
  auto animTempGroup = new QParallelAnimationGroup();

  auto rightHandAnimation = handAnim(m_v_juggler.at(0), propNum, 1, rightHand);
  rightHandAnimation->setLoopCount(-1);
  animTempGroup->addAnimation(rightHandAnimation);

  handPauseAnim->addPause((HAND_PERIOD / 2) * S_TO_MS);
  auto leftHandAnimation = handAnim(m_v_juggler.at(0), propNum, 1, leftHand);
  handPauseAnim->addAnimation(leftHandAnimation);
  leftHandAnimation->setLoopCount(-1);
  animTempGroup->addAnimation(handPauseAnim);

  // pas beau
  auto rightHandAnimation2 = handAnim(m_v_juggler.at(1), propNum, 1, rightHand);
  rightHandAnimation2->setLoopCount(-1);
  animTempGroup->addAnimation(rightHandAnimation2);

  handPauseAnim->addPause((HAND_PERIOD / 2) * S_TO_MS);
  auto leftHandAnimation2 = handAnim(m_v_juggler.at(1), propNum, 1, leftHand);
  handPauseAnim->addAnimation(leftHandAnimation2);
  leftHandAnimation2->setLoopCount(-1);
  animTempGroup->addAnimation(handPauseAnim);


  for (int i = 0; i < state.size(); i++) // for each bit in state
  {
    if (state.testBit(i)) // if it's a site launch
    {
      int launchPos = i % m_period; // i may be beyond period
      int launch = m_siteSwap->at(launchPos);
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

      QVector<AnimEvent*> v_animEvent = m_siteSwap->getAnimEvents(launchPos, launchHand, jugId);

      auto juggler = m_v_juggler.at(0); // pas beau

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

      if (m_siteSwap->getJugglerCount() > 1) // pas beau
      {
        int launchPos = i % m_period; // i may be beyond period
        int launch = m_siteSwap->at(launchPos);
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

        QVector<AnimEvent*> v_animEvent = m_siteSwap->getAnimEvents(launchPos, launchHand, jugId);

        juggler = m_v_juggler.at(1); // WARNING: ugly !
        for (int j = 0; j < v_animEvent.size(); j++)
        {
          // WARNING: juggler may change

          auto animEvent = v_animEvent.at(j);
          auto launchAnim = parabolicAnim(m_v_juggler.at(animEvent->getJugLaunchId()), propNum, animEvent->getLaunch(), animEvent->getHandLaunch());
          launchAnim->setLoopCount(1);
          propMoveAnim->addAnimation(launchAnim);
          // dwell
          auto dwellAnimation = dwellAnim(m_v_juggler.at(animEvent->getJugLaunchId()), propNum, animEvent->getNewLaunch(), animEvent->getHandRecieve());
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

QSequentialAnimationGroup *Animation::handAnim(Juggler *t_juggler,
                                                 int t_indexProp,
                                                 int t_launch,
                                                 hand t_hand)
{
  // start and end rotation <angle,velocity,hand>
//    QVector3D pos = QVector3D(270,15,0);
//    QVector3D pos2 = QVector3D(90,15,0);
  Q_UNUSED(t_indexProp)
  Q_UNUSED(t_launch)

  QVector2D pos = QVector2D(60,15);
  QVector2D pos2 = QVector2D(-60,15);
  auto animGroup = new QSequentialAnimationGroup();

  auto animLeftHandDwell = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_leftHandPosition"));
  auto animRightHandDwell = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_rightHandPosition"));
  auto animLeftHandLaunch = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_leftHandPosition"));
  auto animRightHandLaunch = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_rightHandPosition"));

  float frameCount =  ((HAND_PERIOD) / 2) * (2 - (2 * DWELL_RATIO));

//    auto animHandDwell = new QPropertyAnimation(aJuggler, QByteArrayLiteral("m_handPosition"));
//    auto animHandLaunch = new QPropertyAnimation(aJuggler, QByteArrayLiteral("m_handPosition"));

//    if (aHand == rightHand) {
//      pos.setZ(1);
//      pos2.setZ(1);
//    }
//    QVector3D pos2 = pos-QVector3D(180,0,0);

//    animHandDwell->setDuration((int)(DWELL_TIME * S_TO_MS));
//    animHandDwell->setStartValue(pos);
//    animHandDwell->setEndValue(pos2);
//    animHandDwell->setLoopCount(1);
//    animGroup->addAnimation(animHandDwell);

//    qDebug() << "hand dwellTime : " << (int)(DWELL_TIME * S_TO_MS);

//    auto animHandLaunch = new QPropertyAnimation(aJuggler, QByteArrayLiteral("handPosition"));

//    animHandLaunch->setDuration((int)(frameCount * S_TO_MS)+1);
//    animHandLaunch->setStartValue(pos2);
//    animHandLaunch->setEndValue(pos2-QVector3D(180,0,0));
//    animHandLaunch->setLoopCount(1);
//    animGroup->addAnimation(animHandLaunch);

//    qDebug() << "hand launchTime : " << (int)(frameCount * S_TO_MS);

  if (t_hand == leftHand)
  {
    animLeftHandDwell->setDuration((int)(DWELL_TIME * S_TO_MS)); // WARNING: possible leak of memory ?
    animLeftHandDwell->setStartValue(pos);
    animLeftHandDwell->setEndValue(pos2);
    animLeftHandDwell->setLoopCount(1);
    animGroup->addAnimation(animLeftHandDwell);

    qDebug() << "hand dwellTime : " << (int)(DWELL_TIME * S_TO_MS);

    animLeftHandLaunch->setDuration((int)(frameCount * S_TO_MS)+1);
    animLeftHandLaunch->setStartValue(pos2);
    animLeftHandLaunch->setEndValue(pos2-QVector2D(240,0));
    animLeftHandLaunch->setLoopCount(1);
    animGroup->addAnimation(animLeftHandLaunch);

    qDebug() << "hand launchTime : " << (int)(frameCount * S_TO_MS);
  }
  else
  {
    animRightHandDwell->setDuration((int)(DWELL_TIME * S_TO_MS)); // WARNING: possible leak of memory ?
    animRightHandDwell->setStartValue(pos);
    animRightHandDwell->setEndValue(pos2);
    animRightHandDwell->setLoopCount(1);
    animGroup->addAnimation(animRightHandDwell);

    animRightHandLaunch->setDuration((int)(frameCount * S_TO_MS)+1);
    animRightHandLaunch->setStartValue(pos2);
    animRightHandLaunch->setEndValue(pos2-QVector2D(240,0));
    animRightHandLaunch->setLoopCount(1);
    animGroup->addAnimation(animRightHandLaunch);
  }
  return animGroup;
}

QSequentialAnimationGroup *Animation::parabolicAnim(Juggler *t_juggler, // TODO: several jugglers ?
                                                    int t_indexProp,
                                                    int t_launch,
                                                    hand t_hand)
{
  auto animGroup = new QSequentialAnimationGroup();
  QVector3D posProp; // pos where it starts
  QVector3D posFinal; // pos where it should finish

  // bool to know if we need to enlarge our juggling
  bool isExtPlusCatch = (m_propType == ring  && m_siteSwap->getLaunchType() == panCake) ||
      (m_propType == club && m_siteSwap->getLaunchType() == helicopter);

  // odd launches
  if (t_hand == leftHand && t_launch % 2 != 0)
  {
    posProp = t_juggler->getPositionLHint();
    if (isExtPlusCatch)
      posFinal = t_juggler->getPositionRHextPlus();
    else
      posFinal = t_juggler->getPositionRHext();
  }
  if (t_hand == rightHand && t_launch % 2 != 0)
  {
    posProp = t_juggler->getPositionRHint();
    if (isExtPlusCatch)
      posFinal = t_juggler->getPositionLHextPlus();
    else
      posFinal = t_juggler->getPositionLHext();
  }

  // even launches
  if (t_hand == leftHand && t_launch % 2 == 0)
  {
    posProp = t_juggler->getPositionLHint();
    if (isExtPlusCatch)
      posFinal = t_juggler->getPositionLHextPlus();
    else
      posFinal = t_juggler->getPositionLHext();
  }
  if (t_hand == rightHand && t_launch % 2 == 0)
  {
    posProp = t_juggler->getPositionRHint();
    if (isExtPlusCatch)
      posFinal = t_juggler->getPositionRHextPlus();
    else
      posFinal = t_juggler->getPositionRHext();
  }

  // Shannon theorem
  float arcTime;
  if (t_launch == 1) // For launch 1 Shannon doesn't work
  {
    arcTime = LAUNCH1_TIME;
  }
  else // thanks Claude Shannon
    arcTime = ((HAND_PERIOD) / 2) * (t_launch - (2 * DWELL_RATIO));

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
  auto animTempGroup = new QParallelAnimationGroup();
  QPropertyAnimation *animRotProp;
  auto animTranslationGroup = new QSequentialAnimationGroup();

  // declare them in case we need
  int launchType; // normal, flat, pancake ??
  int rotCount; // number of rotation
  float fRotCount; // needed for helicopter
  float rotY; // to handle different positions between normal, flat, helicopter

  qDebug() << "prop launchTime : " << (int)(frameCount * (int)(DELTA_TIME * S_TO_MS));
  switch(m_propType)
  {
  case ball:
    aBall = m_v_ball.at(t_indexProp); // WARNING: possible leak of memory ?
    // loop creates all our animations for translation
    for (int i = 0; i < frameCount; i++)
    {
      auto animBall = new QPropertyAnimation(aBall, QByteArrayLiteral("m_position"));
      animBall->setDuration((int)(DELTA_TIME * S_TO_MS));
      animBall->setStartValue(vParabolic.at(i));
      animBall->setEndValue(vParabolic.at(i + 1));
      animBall->setLoopCount(1);
      animGroup->addAnimation(animBall);
    }
    break;
  case ring:
    aRing = m_v_ring.at(t_indexProp);
    // set rotY to align with juggler
    rotY = t_juggler->getRotY();

    // switch between types
    launchType = aRing->getLaunchType();
    switch (launchType)
    {
    case normalRing:
      aRing->setRotY(rotY + RING_BASIC_ROTY);

      // loop creates all our animations for translation
      for (int i = 0; i < frameCount; i++)
      {
        auto animRing = new QPropertyAnimation(aRing, QByteArrayLiteral("m_position"));
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
        auto animRing = new QPropertyAnimation(aRing, QByteArrayLiteral("m_position"));
        animRing->setDuration((int)(DELTA_TIME * S_TO_MS));
        animRing->setStartValue(vParabolic.at(i));
        animRing->setEndValue(vParabolic.at(i + 1));
        animRing->setLoopCount(1);
        animTranslationGroup->addAnimation(animRing);
      }
      rotCount = (int)(t_launch / 2);
      if (rotCount)
      {
        animRotProp = new QPropertyAnimation(aRing, QByteArrayLiteral("m_rotX"));
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
    aClub = m_v_club.at(t_indexProp);
    // set rotY to align with juggler
    rotY = t_juggler->getRotY();

    // switch between types
    launchType = aClub->getLaunchType();
    switch (launchType)
    {
    case normalClub:
      aClub->setRotX(CLUB_BASIC_ROTX);
      aClub->setRotY(rotY);
      for (int i = 0; i < frameCount; i++)
      {
        auto animClub = new QPropertyAnimation(aClub, QByteArrayLiteral("m_position"));
        animClub->setDuration((int)(DELTA_TIME * S_TO_MS));
        animClub->setStartValue(vParabolic.at(i));
        animClub->setEndValue(vParabolic.at(i + 1));
        animClub->setLoopCount(1);
        animTranslationGroup->addAnimation(animClub);
      }
      rotCount = (int)(t_launch / 2);
      if (rotCount)
      {
        animRotProp = new QPropertyAnimation(aClub, QByteArrayLiteral("m_rotX"));
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
        auto animClub = new QPropertyAnimation(aClub, QByteArrayLiteral("m_position"));
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
      if (t_hand == leftHand)
        aClub->setRotY(rotY + 90);
      else
        aClub->setRotY(rotY - 90);
      for (int i = 0; i < frameCount; i++)
      {
        auto animClub = new QPropertyAnimation(aClub, QByteArrayLiteral("m_position"));
        animClub->setDuration((int)(DELTA_TIME * S_TO_MS));
        animClub->setStartValue(vParabolic.at(i));
        animClub->setEndValue(vParabolic.at(i + 1));
        animClub->setLoopCount(1);
        animTranslationGroup->addAnimation(animClub);
      }
      fRotCount = (float)((float)t_launch / 2.0f);
      animRotProp = new QPropertyAnimation(aClub, QByteArrayLiteral("m_rotY"));
      animRotProp->setDuration(((int)(DELTA_TIME * S_TO_MS)) * (frameCount));
      if (t_hand == leftHand)
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

QSequentialAnimationGroup *Animation::dwellAnim(Juggler *t_juggler,
                                                  int t_indexProp,
                                                  int t_nextLaunch,
                                                  hand t_hand)
{
  // set recieve pos
  QVector3D pos;
  // set next launch pos
  QVector3D pos2;
  // set center curve
  QVector3D centerCurve;
  // bool to know if we need to enlarge our juggling
  bool isExtPlusCatch = (m_propType == ring && m_siteSwap->getLaunchType() == panCake) ||
      (m_propType == club && m_siteSwap->getLaunchType() == helicopter);

  auto returnAnim = new QSequentialAnimationGroup();

  if (t_hand == leftHand)
  {
    if (isExtPlusCatch)
      pos = t_juggler->getPositionLHextPlus();
    else
      pos = t_juggler->getPositionLHext();
    pos2 = t_juggler->getPositionLHint();
  }
  else
  {
    if (isExtPlusCatch)
      pos = t_juggler->getPositionRHextPlus();
    else
      pos = t_juggler->getPositionRHext();
    pos2 = t_juggler->getPositionRHint();
  }
  centerCurve = (pos + pos2) / 2;

  // declare before switch cos I don't want to be insulted
  QPropertyAnimation *dwellAnimation;
  JugglingBall *aBall;
  JugglingRing *aRing;
  Pirouette *aClub;

  if (t_nextLaunch == 1) // that's special for launch 1
  {
    switch (m_propType)
    {
    default: case ball:
      aBall = m_v_ball.at(t_indexProp);
      dwellAnimation = new QPropertyAnimation(aBall, QByteArrayLiteral("m_position"));
      break;
    case ring:
      aRing = m_v_ring.at(t_indexProp);
      dwellAnimation = new QPropertyAnimation(aRing, QByteArrayLiteral("m_position"));
      break;
    case club:
      aClub = m_v_club.at(t_indexProp);
      dwellAnimation = new QPropertyAnimation(aClub, QByteArrayLiteral("m_position"));
      break;
    }
    dwellAnimation->setDuration((int)(DWELL_TIME_LAUNCH1 * S_TO_MS));
    dwellAnimation->setStartValue(pos);
    dwellAnimation->setEndValue(pos2);
    // NOTE: make an easing curve ?
    dwellAnimation->setLoopCount(1);
    returnAnim->addAnimation(dwellAnimation);

    return returnAnim;
  }

  // determine axis for rotation
  float rotY = t_juggler->getRotY();
  QVector3D axisCurve = QVector3D(0, 0, 1);
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(rotY, QVector3D(0, 1, 0));
  axisCurve = rotAxis * axisCurve;

  // determine number of frames
  int frameCount = (int)((DWELL_TIME / DELTA_TIME));
  // determine angles for each delta animation
  float deltaAngles = (float)(180 / frameCount);
  switch (m_propType)
  {
  default: case ball:
    aBall = m_v_ball.at(t_indexProp);
    break;
  case ring:
    aRing = m_v_ring.at(t_indexProp);
    break;
  case club:
    aClub = m_v_club.at(t_indexProp);
    break;
  }
  // loop creates all our animations for dwell time
  for (int i = 0; i < frameCount; i++)
  {
    switch (m_propType)
    {
    default: case ball:
      dwellAnimation = new QPropertyAnimation(aBall, QByteArrayLiteral("m_position"));
      break;
    case ring:
      dwellAnimation = new QPropertyAnimation(aRing, QByteArrayLiteral("m_position"));
      break;
    case club:
      dwellAnimation = new QPropertyAnimation(aClub, QByteArrayLiteral("m_position"));
      break;
    }
    dwellAnimation->setDuration((int)(DELTA_TIME * S_TO_MS));
    dwellAnimation->setStartValue(pos);
    // handle rotation stuff
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.translate(centerCurve);
    if (t_hand == leftHand)
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
