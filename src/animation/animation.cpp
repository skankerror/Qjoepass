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
  m_period = m_siteSwap->getPeriod();
}

void Animation::setAnim()
{
  // Normally it's already tested but to be sure we test
  if (!(m_siteSwap->isValid()))
  {
    qDebug() << "Siteswap isn't valid";
    return;
  }

  // we calculate state.
  // TODO: what about passing siteswap ?
  QBitArray state = m_siteSwap->getState();
  int propNum = 0;

  // NOTE: this approch must be changed to handle 0 launch, hand must stay quiet
  // rightHandAnim is a seq anim that will handle all right hand anim
  auto rightHandAnimation = handAnim(m_v_juggler.at(0), propNum, 3, rightHand);
  // infinite Loop
  rightHandAnimation->setLoopCount(INFINITE_LOOP);
  // add right hand to the main paral anim
  addAnimation(rightHandAnimation);

  // set initial pause for left hand
  // we assume beginning with right one
  // so we must create seq handPauseAnim to handle this
  auto handPauseAnim = new QSequentialAnimationGroup();
  handPauseAnim->addPause((HAND_PERIOD / 2) * S_TO_MS);
  // leftHandAnim is a seq anim that will handle all left hand anim
  auto leftHandAnimation = handAnim(m_v_juggler.at(0), propNum, 3, leftHand);
  // add hand mouvement to anim with initial pause
  handPauseAnim->addAnimation(leftHandAnimation);
  // inifinite loop
  leftHandAnimation->setLoopCount(INFINITE_LOOP);
  // add the whole left hand to main paral anim
  addAnimation(handPauseAnim);

  // TODO: we must have any number of jugglers

  for (int i = 0; i < state.size(); i++) // for each bit in state
  {
    if (state.testBit(i)) // if it's a site launch
    {
      int launchPos = i % m_period; // i may be beyond period
      int launch = m_siteSwap->at(launchPos);
      auto propMoveAnim = new QSequentialAnimationGroup(); // will handle the whole moving
      auto propGlobAnim = new QSequentialAnimationGroup();// needed to add pause at the beginning

      // we had delay for each prop
      // we set a delay even for first launch so that we can anim the hand before launch
      float delay = (HAND_PERIOD / 2) * i;
      if (launch == 1) // if launch is 1 time is shorter
        propGlobAnim->addPause((DWELL_TIME_LAUNCH1 + delay) * S_TO_MS);
      else
        propGlobAnim->addPause((DWELL_TIME + delay) * S_TO_MS);

      // single juggler workaround, this has to be changed
      int jugId = 0;
      auto juggler = m_v_juggler.at(0);
      // let's go
      hand launchHand;
      (i % 2 == 0) ? launchHand = rightHand : launchHand = leftHand; // i is odd or even

      QVector<animEvent *> v_animEvent = m_siteSwap->getAnimEvents(launchPos, launchHand, jugId);

      for (int j = 0; j < v_animEvent.size(); j++)
      {
        auto myAnimEvent = v_animEvent.at(j);
        auto launchAnim = parabolicAnim(juggler, propNum, myAnimEvent->launch, myAnimEvent->handLaunch);
        launchAnim->setLoopCount(ONE_LOOP);
        propMoveAnim->addAnimation(launchAnim);
        // dwell
        auto dwellAnimation = dwellAnim(juggler, propNum, myAnimEvent->newLaunch, myAnimEvent->handRecieve);
        dwellAnimation->setLoopCount(ONE_LOOP);
        propMoveAnim->addAnimation(dwellAnimation);
      }
      // we add to the anim containing starting pause
      propGlobAnim->addAnimation(propMoveAnim);
      propMoveAnim->setLoopCount(-1);
      addAnimation(propGlobAnim); // and we add to the main parallel anim

      propNum++;
    }
  }
}

QSequentialAnimationGroup *Animation::handAnim(Juggler *t_juggler,
                                               int t_indexProp,
                                               int t_launch,
                                               hand t_hand)
{

  // now we don't animate hands according to the launch except for 1 launch
  // but we may implement differences between a 3 and a 5 ?
  // is indexProp relevant ?
  Q_UNUSED(t_indexProp)

  // set receive pos
  QVector3D pos;
  // set next launch pos
  QVector3D pos2;
  // bool to know if we need to enlarge our juggling
  bool isExtPlusCatch = (m_propType == ring && m_siteSwap->getLaunchType() == panCake) ||
      (m_propType == club && m_siteSwap->getLaunchType() == helicopter);

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

  // create seq anim for whole mouvement
  auto animGroup = new QSequentialAnimationGroup();
  //define anim with prop
  QPropertyAnimation *dwellAnimation;
  // define empty anim hand
  QPropertyAnimation *emptyHandAnimation;

  // calculate number of frames
  int frameCount = (int)((DWELL_TIME / DELTA_TIME));

  // special for launch 1 with linear mouvement
  if (t_launch == 1)
  {
    if (t_hand == leftHand)
    {
      dwellAnimation = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_leftHandPosition"));
      emptyHandAnimation = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_leftHandPosition"));
    }
    else
    {
      dwellAnimation = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_rightHandPosition"));
      emptyHandAnimation = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_rightHandPosition"));
    }
    dwellAnimation->setDuration((int)(DWELL_TIME_LAUNCH1 * S_TO_MS));
    dwellAnimation->setStartValue(pos);
    dwellAnimation->setEndValue(pos2);
    // NOTE: make an easing curve ?
    dwellAnimation->setLoopCount(ONE_LOOP);
    animGroup->addAnimation(dwellAnimation);

    emptyHandAnimation->setDuration((int)((HAND_PERIOD - DWELL_TIME_LAUNCH1) * S_TO_MS));
    emptyHandAnimation->setStartValue(pos2);
    emptyHandAnimation->setEndValue(pos);
    // NOTE: make an easing curve ?
    emptyHandAnimation->setLoopCount(ONE_LOOP);
    animGroup->addAnimation(emptyHandAnimation);

    return animGroup;
  }

  // determine axis for rotation
  float rotY = t_juggler->getRotY();

  QVector3D axisCurve = QVector3D(0, 0, 1);
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(rotY, QVector3D(0, 1, 0));
  axisCurve = rotAxis * axisCurve;

  // we create our curve
  QVector<QVector3D> v_semiCircular = Curves::curveSemiCircular(pos, pos2, rotY, t_hand, frameCount);

  // loop creates all our animations for dwell time
  for (int i = 0; i < frameCount; i++)
  {
    if (t_hand == leftHand)
    {
      dwellAnimation = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_leftHandPosition"));
    }
    else
    {
      dwellAnimation = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_rightHandPosition"));
    }
    dwellAnimation->setDuration((int)(DELTA_TIME * S_TO_MS));
    dwellAnimation->setStartValue(v_semiCircular.at(i));
    dwellAnimation->setEndValue(v_semiCircular.at(i + 1));
    dwellAnimation->setLoopCount(ONE_LOOP);
    animGroup->addAnimation(dwellAnimation);
  }
  // time adjustments
  float duration = DELTA_TIME * frameCount;
  float decay = DWELL_TIME - duration;
  int intDecay = (int)(decay * S_TO_MS);
  if (intDecay)
    animGroup->addPause(intDecay);

  // empty hand animation
  if (t_hand == leftHand)
  {
    emptyHandAnimation = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_leftHandPosition"));
  }
  else
  {
    emptyHandAnimation = new QPropertyAnimation(t_juggler, QByteArrayLiteral("m_rightHandPosition"));
  }
  // calculate empty hand time
  int emptyHandTime = (HAND_PERIOD - DWELL_TIME) * S_TO_MS + 1; // without 1 there's a decay...
  emptyHandAnimation->setDuration(emptyHandTime);
  emptyHandAnimation->setStartValue(pos2);
  emptyHandAnimation->setEndValue(pos);
  // NOTE: make an easing curve ?
  emptyHandAnimation->setLoopCount(ONE_LOOP);
  animGroup->addAnimation(emptyHandAnimation);

  // time adjustments ?

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
  QVector<QVector3D> v_parabolic = Curves::curveParabolic(velBall, posProp, frameCount);

  // declare one object for each prop in case we need it in switch routines
  JugglingBall *aBall = nullptr;
  JugglingRing *aRing = nullptr;
  Pirouette *aClub = nullptr;

  // for ring or club rotation we have to create 3 more anims
  QParallelAnimationGroup *animTempGroup = nullptr;
  QPropertyAnimation *animRotProp;
  QSequentialAnimationGroup *animTranslationGroup = nullptr;

  // declare them in case we need
  int launchType; // normal, flat, pancake ??
  int rotCount; // number of rotation
  float fRotCount; // needed for helicopter
  float rotY; // to handle different positions between normal, flat, helicopter

  switch(m_propType)
  {
  case ball:
    aBall = m_v_ball.at(t_indexProp);
    // loop creates all our animations for translation
    for (int i = 0; i < frameCount; i++)
    {
      auto animBall = new QPropertyAnimation(aBall, QByteArrayLiteral("m_position"));
      animBall->setDuration((int)(DELTA_TIME * S_TO_MS));
      animBall->setStartValue(v_parabolic.at(i));
      animBall->setEndValue(v_parabolic.at(i + 1));
      animBall->setLoopCount(ONE_LOOP);
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
        animRing->setStartValue(v_parabolic.at(i));
        animRing->setEndValue(v_parabolic.at(i + 1));
        animRing->setLoopCount(1);
        animGroup->addAnimation(animRing);
      }
      break;
    case panCake:
      aRing->setRotY(rotY);
      aRing->setRotX(RING_PANCAKE_ROTX);
      animTranslationGroup = new QSequentialAnimationGroup();
      for (int i = 0; i < frameCount; i++)
      {
        auto animRing = new QPropertyAnimation(aRing, QByteArrayLiteral("m_position"));
        animRing->setDuration((int)(DELTA_TIME * S_TO_MS));
        animRing->setStartValue(v_parabolic.at(i));
        animRing->setEndValue(v_parabolic.at(i + 1));
        animRing->setLoopCount(1);
        animTranslationGroup->addAnimation(animRing);
      }
      rotCount = (int)(t_launch / 2);
      animTempGroup = new QParallelAnimationGroup();
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
      animTranslationGroup = new QSequentialAnimationGroup();
      for (int i = 0; i < frameCount; i++)
      {
        auto animClub = new QPropertyAnimation(aClub, QByteArrayLiteral("m_position"));
        animClub->setDuration((int)(DELTA_TIME * S_TO_MS));
        animClub->setStartValue(v_parabolic.at(i));
        animClub->setEndValue(v_parabolic.at(i + 1));
        animClub->setLoopCount(1);
        animTranslationGroup->addAnimation(animClub);
      }
      rotCount = (int)(t_launch / 2);
      animTempGroup = new QParallelAnimationGroup();
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
        animClub->setStartValue(v_parabolic.at(i));
        animClub->setEndValue(v_parabolic.at(i + 1));
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
      animTranslationGroup = new QSequentialAnimationGroup();
      for (int i = 0; i < frameCount; i++)
      {
        auto animClub = new QPropertyAnimation(aClub, QByteArrayLiteral("m_position"));
        animClub->setDuration((int)(DELTA_TIME * S_TO_MS));
        animClub->setStartValue(v_parabolic.at(i));
        animClub->setEndValue(v_parabolic.at(i + 1));
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
      animTempGroup = new QParallelAnimationGroup();
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

  // declare before switch cos I don't want to be insulted by gcc
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
    dwellAnimation->setLoopCount(ONE_LOOP);
    returnAnim->addAnimation(dwellAnimation);

    return returnAnim;
  }

  // determine axis for rotation
  float rotY = t_juggler->getRotY();
  // determine number of frames
  int frameCount = (int)((DWELL_TIME / DELTA_TIME));

  QVector3D axisCurve = QVector3D(0, 0, 1);
  QMatrix4x4 rotAxis;
  rotAxis.setToIdentity();
  rotAxis.rotate(rotY, QVector3D(0, 1, 0));
  axisCurve = rotAxis * axisCurve;

  // we create our curve
  QVector<QVector3D> v_semiCircular = Curves::curveSemiCircular(pos, pos2, rotY, t_hand, frameCount);

  // loop creates all our animations for dwell time
  for (int i = 0; i < frameCount; i++)
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
    dwellAnimation->setDuration((int)(DELTA_TIME * S_TO_MS));
    dwellAnimation->setStartValue(v_semiCircular.at(i));
    dwellAnimation->setEndValue(v_semiCircular.at(i + 1));
    dwellAnimation->setLoopCount(ONE_LOOP);
    returnAnim->addAnimation(dwellAnimation);
  }

  // time adjustments
  float duration = DELTA_TIME * frameCount;
  float decay = DWELL_TIME - duration;
  int intDecay = (int)(decay * S_TO_MS);
  if (intDecay)
    returnAnim->addPause(intDecay);
  return returnAnim;
}
