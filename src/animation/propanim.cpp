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

#include "propanim.h"

PropAnim::PropAnim(QVector<Juggler *> t_v_juggler,
                   JugglingProp *t_prop,
                   int t_propId,
                   propType t_propType,
                   int t_launchType,
                   QObject *parent)

  : QSequentialAnimationGroup(parent),
    m_propId(t_propId),
    m_v_juggler(t_v_juggler),
    m_propType(t_propType),
    m_prop(t_prop)
{
  switch (m_propType) // TODO: check out of range
  {
  default:
  case  ball: m_launchTypeBall = launchTypeBall(t_launchType); break;
  case ring: m_launchTypeRing = launchTypeRing(t_launchType); break;
  case club: m_launchTypeClub = launchTypeClub(t_launchType); break;
  }

  // bool to know if we need to enlarge our juggling
  m_isExtPlusCatch = (m_propType == ring  && m_launchTypeRing == panCake) ||
      (m_propType == club && m_launchTypeClub == helicopter);

  // get rotations or our prop, that we will update after each rotation
  m_propRotX = m_prop->getRotX();
  m_propRotY = m_prop->getRotY();

  /************************testing zone ****************************/
}

void PropAnim::setAnim(QVector<animEvent *> t_v_animEvents)
{
  for (int i = 0; i < t_v_animEvents.size(); i++)
  {
    auto propAnimEvent = t_v_animEvents.at(i);

    /* this is a vanilla launch, so we must adjust it according to number of jugglers
     * we create an adjust int. */
    int jugglerLaunchId = propAnimEvent->s_jugglerLaunchId;
    int jugglerReceiveId = propAnimEvent->s_jugglerRecieveId;
    int adjustLaunch = jugglerLaunchId - jugglerReceiveId;
    int launch = propAnimEvent->s_launch;
//    qDebug() << "launch in propAnim" << launch;
    int realLaunch = (launch + adjustLaunch) / m_v_juggler.size();
//    qDebug() << "real launch in propAnim" << realLaunch;
    auto aDwellParabolicAnim = dwellParabolicAnim(jugglerLaunchId,
                                                  propAnimEvent->s_launchHand,
                                                  jugglerReceiveId,
                                                  propAnimEvent->s_receiveHand,
                                                  realLaunch);
    addAnimation(aDwellParabolicAnim);
    setLoopCount(INFINITE_LOOP);
  }
}

QParallelAnimationGroup *PropAnim::parabolicAnim(int t_jugglerIdLaunch,
                                                 hand t_handLaunch,
                                                 int t_jugglerIdRecieve,
                                                 hand t_handRecieve,
                                                 int t_launch)
{
  // create parallel animation group if we need to rotate during translation
  auto parallelAnimGroup = new QParallelAnimationGroup();

  // get our concerned jugglers
  auto jugglerLaunch = m_v_juggler.at(t_jugglerIdLaunch); // TODO: check out of range
  auto jugglerRecieve = m_v_juggler.at(t_jugglerIdRecieve); // TODO: check out of range

  // find our 2 positions
  auto posProp = getIntHandPos(jugglerLaunch, t_handLaunch); // pos where it starts
  auto posFinal = getExtHandPos(jugglerRecieve, t_handRecieve); // pos where it should finish

  // bool to know if it's a passing launch
  bool isPassing = (t_jugglerIdLaunch != t_jugglerIdRecieve);

  // calculate arc time
  float arcTime = getArcTime(t_launch);

  //
  int frameCount = (int)((arcTime / (DELTA_TIME)));

  // create translation seq animation
  auto translationAnimGroup = parabolicTranslataionAnimGroup(posProp,
                                                             posFinal,
                                                             arcTime);

  // add it to our parallel
  parallelAnimGroup->addAnimation(translationAnimGroup);

  // NOTE: see rebounds stuff when it will be implemented
  // if prop is a ball simply return translation
  if (m_propType == ball)
    return parallelAnimGroup;

  // else we must rotate prop
  else
  {
    // set numbers of rotations on (x)
    int rotXCount = (int)(t_launch / 2);

    // now set rotations
    float initialRotX = m_propRotX;

    float addRotX = 0;
    // if passing launch add some rot
    if (isPassing)
      addRotX = ((initialRotX - 90.0f) * 2.0f);

    // change rotX for normal club
    if (m_propType == club &&
        m_launchTypeClub == normalClub)
    {
      // if passing, club must be vertical in recieving hand
      if (isPassing)
        addRotX = (90.0f - (3 * initialRotX)) / 2.0f; // - initX - (initX - 90)/2
      else
      {
        addRotX = -addRotX; // maybe this can be correctly set in dwell
      }
    }

    if (m_propType == club &&
        m_launchTypeClub == flat)
    {
      rotXCount = 0; // no rot it's flat
      // but if passing, club must be vertical in recieving hand
      if (isPassing)
        addRotX = - initialRotX; // TODO: check this
    }

    // helicopter workaround
//    if (m_propType == club &&
//        m_launchTypeClub == helicopter)
//    {
//      // rotation (x) become (y)
//      int rotYCount = rotXCount;
//      float initialRotY = m_prop->getRotY();
//      float endValue = jugglerRecieve->getRotY();
//      t_handRecieve == leftHand ? // NOTE: doesn't seem totally good
//              endValue += 90 - (rotYCount * 360):
//          endValue -= 90 - (rotYCount * 360);
//      auto animRotY = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotY"));
//      animRotY->setDuration(((int)(DELTA_TIME * S_TO_MS)) * frameCount);
//      animRotY->setStartValue(initialRotY);
//      animRotY->setEndValue(endValue);
//      animRotY->setLoopCount(ONE_LOOP);
//      parallelAnimGroup->addAnimation(animRotY);

//      return parallelAnimGroup;
//    }

    // rotation X
    auto animRotX = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"));
    animRotX->setDuration(((int)(DELTA_TIME * S_TO_MS)) * frameCount);
    animRotX->setStartValue((360 * rotXCount) + initialRotX);
    animRotX->setEndValue(initialRotX + addRotX);
    animRotX->setLoopCount(ONE_LOOP);
    parallelAnimGroup->addAnimation(animRotX);

    // update our rotX to our endValue
    m_propRotX = initialRotX + addRotX;

    // finally return anim
    return parallelAnimGroup;
  }
}

QParallelAnimationGroup *PropAnim::dwellAnim(int t_jugglerIdLaunch,
                                             hand t_handLaunch,
                                             int t_jugglerIdRecieve,
                                             hand t_handRecieve,
                                             int t_launch)
{
  // create parallel animation group to translate and rotation if needed
  auto parallelAnimGroup = new QParallelAnimationGroup();

  // get our concerned jugglers
  auto jugglerLaunch = m_v_juggler.at(t_jugglerIdLaunch); // TODO: check out of range
  auto jugglerRecieve = m_v_juggler.at(t_jugglerIdRecieve); // TODO: check out of range

  // pos where it starts
  QVector3D posProp = getExtHandPos(jugglerLaunch,
                                    t_handLaunch);
  // pos where it should finish
  QVector3D posFinal = getIntHandPos(jugglerLaunch,
                                     t_handLaunch);

  // bool to know if it's a passing launch
  bool isPassing = (t_jugglerIdLaunch != t_jugglerIdRecieve);

  if (isPassing)
  {
    auto translationAnimGroup = dwellPassingTranslationAnim(jugglerLaunch,
                                                            jugglerRecieve,
                                                            posProp,
                                                            posFinal,
                                                            t_handRecieve);

    // add to return anim
    parallelAnimGroup->addAnimation(translationAnimGroup);

    // make x y rotations for club
    if (m_propType == club && (m_launchTypeClub == normalClub || m_launchTypeClub == flat))
    {
      // make x rot for club
      auto animRotX = dwellClubRotXAnim(t_launch, isPassing);

      // add sequential to return parallel anim group
      parallelAnimGroup->addAnimation(animRotX);

      // make y rotation for club
      auto animRotY = dwellClubPassingRotYAnim(jugglerRecieve,
                                               posFinal,
                                               t_handRecieve);

      // add to return parallel anim group
      parallelAnimGroup->addAnimation(animRotY);
    }

  }

  else // it's self launch
  {
    auto translationAnimGroup = dwellSelfTranslationAnim(posProp,
                                                         posFinal,
                                                         t_launch);

    parallelAnimGroup->addAnimation(translationAnimGroup);

    // let's rotate if necessary
    // give a little (x) rotation for normal and flat club.
    if (m_propType == club && (m_launchTypeClub == normalClub || m_launchTypeClub == flat))
    {
      auto animRotX = dwellClubRotXAnim(t_launch, isPassing);

      // add sequential to return parallel anim group
      parallelAnimGroup->addAnimation(animRotX);
    }

    // give a little (y) rotation for normal ring and normal and flat club
    if ((m_propType == ring && m_launchTypeRing == normalRing) ||
        (m_propType == club && (m_launchTypeClub == normalClub || m_launchTypeClub == flat)))
    {
      float jugglerRotY = jugglerLaunch->getRotY();

      auto animRotY = dwellClubSelfRotYAnim(jugglerRotY,
                                            t_launch,
                                            t_handLaunch);

      parallelAnimGroup->addAnimation(animRotY);
    }
  }

  // finally we return
  return parallelAnimGroup;
}

QSequentialAnimationGroup* PropAnim::dwellParabolicAnim(int t_jugglerIdLaunch,
                                                        hand t_handLaunch,
                                                        int t_jugglerIdRecieve,
                                                        hand t_handRecieve,
                                                        int t_launch)
{
  auto dwell = dwellAnim(t_jugglerIdLaunch,
                         t_handLaunch,
                         t_jugglerIdRecieve,
                         t_handRecieve,
                         t_launch);

  auto parabolic = parabolicAnim(t_jugglerIdLaunch,
                                 t_handLaunch,
                                 t_jugglerIdRecieve,
                                 t_handRecieve,
                                 t_launch);

  auto retAnim = new QSequentialAnimationGroup();

  retAnim->addAnimation(dwell);
  retAnim->addAnimation(parabolic);

  return retAnim;
}

QVector3D PropAnim::getExtHandPos(const Juggler *t_juggler,
                                  const hand t_side) const
{
  QVector3D retVector;
  if (t_side == leftHand)
  {
    m_isExtPlusCatch ?
          retVector = t_juggler->getPositionLHextPlus():
       retVector = t_juggler->getPositionLHext();
  }
  else
  {
    m_isExtPlusCatch ?
          retVector = t_juggler->getPositionRHextPlus():
       retVector = t_juggler->getPositionRHext();
  }
  return retVector;
}

QVector3D PropAnim::getIntHandPos(const Juggler *t_juggler,
                                  const hand t_side) const
{
  QVector3D retVector;
  t_side == leftHand ?
        retVector = t_juggler->getPositionLHint():
      retVector = t_juggler->getPositionRHint();
  return retVector;
}

float PropAnim::getArcTime(const int t_launch) const
{
  float arcTime;

  if (t_launch == 1) // For launch 1 Shannon doesn't work
  {
    arcTime = LAUNCH1_TIME;
  }
  else // Shannon Theorem
    arcTime = ((HAND_PERIOD) / 2) * (t_launch - (2 * DWELL_RATIO));

  return arcTime;
}

QSequentialAnimationGroup *PropAnim::parabolicTranslataionAnimGroup(const QVector3D t_startPos,
                                                                    const QVector3D t_endPos,
                                                                    const float t_arcTime)
{
  // we calculate velocity launch
  auto velBall = ((t_endPos - t_startPos) - 0.5 *
                  (GRAVITY * qPow(t_arcTime, 2))) / t_arcTime;

  int frameCount = (int)((t_arcTime / (DELTA_TIME)));

  // We create our curve
  auto v_parabolic = Curves::curveParabolic(velBall,
                                            t_startPos,
                                            frameCount);

  // create sequential for translation
  auto translationAnimGroup = new QSequentialAnimationGroup();

  // this loop creates all animations for translation
  for (int i = 0; i < frameCount; i++)
  {
    auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
    translationAnim->setDuration((int)(DELTA_TIME * S_TO_MS));
    translationAnim->setStartValue(v_parabolic.at(i));
    translationAnim->setEndValue(v_parabolic.at(i + 1));
    translationAnim->setLoopCount(ONE_LOOP);
    translationAnimGroup->addAnimation(translationAnim);
  }
  return translationAnimGroup;
}

QSequentialAnimationGroup *PropAnim::dwellSelfTranslationAnim(const QVector3D t_startPos,
                                                              const QVector3D t_endPos,
                                                              const int t_launch)
{
  auto translationAnimGroup = new QSequentialAnimationGroup();
  if (t_launch == 1)
  {
    auto translationAnim = dwellSelfLaunch1TranslationAnim(t_startPos, t_endPos);
    translationAnimGroup->addAnimation(translationAnim);
    return translationAnimGroup;
  }
  else
  {
    // determine number of frames
    int frameCount = (int)((DWELL_TIME / DELTA_TIME));
    // we create our curve
    QVector<QVector3D> v_semiCircular = Curves::curveSemiCircular(t_startPos,
                                                                  t_endPos,
                                                                  frameCount);
    // loop creating our tranlation anim
    for (int i = 0; i < frameCount; i++)
    {
      auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
      translationAnim->setDuration((int)(DELTA_TIME * S_TO_MS));
      translationAnim->setStartValue(v_semiCircular.at(i));
      translationAnim->setEndValue(v_semiCircular.at(i + 1));
      translationAnim->setLoopCount(ONE_LOOP);
      translationAnimGroup->addAnimation(translationAnim);
     }
  }
  return translationAnimGroup;
}

QSequentialAnimationGroup *PropAnim::dwellClubRotXAnim(const int t_launch,
                                                       const bool t_isPassing)
{
  // create a sequential anim group cos we have 2 property animations
  auto animRotX = new QSequentialAnimationGroup();

  // final rotX
  float finalRotX = CLUB_BASIC_ROTX; // we always send with this angle

  // mid angle
  float midAngleRotX;
  if (t_isPassing)
  {
    midAngleRotX = finalRotX + CLUB_DWELL_PASSING_X_DECAY;
  }
  else
  {
    if (t_launch == 1)
    {
      midAngleRotX = m_propRotX + ((m_propRotX - finalRotX) / 2);
    }
    else
    {
      midAngleRotX = finalRotX + CLUB_DWELL_SELF_X_DECAY; // NOTE: is it good for a passed club ?
    }
  }

  // create first anim to low down angle
  auto animRotX1 = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"));
  animRotX1->setDuration((int)(DWELL_TIME * S_TO_MS / 2));
  animRotX1->setStartValue(m_propRotX);
  animRotX1->setEndValue(midAngleRotX);
  animRotX1->setLoopCount(ONE_LOOP);
  // add to sequential anim group
  animRotX->addAnimation(animRotX1);

  // create second anim
  auto animRotX2 = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"));
  animRotX2->setDuration((int)(DWELL_TIME * S_TO_MS / 2));
  animRotX2->setStartValue(midAngleRotX);
  animRotX2->setEndValue(finalRotX);
  animRotX2->setLoopCount(ONE_LOOP);
  // add to sequential anim group
  animRotX->addAnimation(animRotX2);

  // update prop rotX
  m_propRotX = finalRotX;

  return animRotX;
}

QPropertyAnimation *PropAnim::dwellClubSelfRotYAnim(const float t_jugglerRotY,
                                                    const int t_launch,
                                                    const hand t_side)
{
  qDebug() << "rotY before dwell animation" << m_propRotY;

  // set final (y) rot
  float finalRotY;
  // angle will depend on side and oddity of launch
  if (t_side == leftHand)
  {
    t_launch % 2 ? // is it even ?
                   finalRotY = t_jugglerRotY - PROP_JUGGLING_DECAY_ROT_Y:
        finalRotY = t_jugglerRotY + PROP_JUGGLING_DECAY_ROT_Y;
  }
  else
  {
    t_launch % 2 ? // is it even ?
                   finalRotY = t_jugglerRotY + PROP_JUGGLING_DECAY_ROT_Y:
        finalRotY = t_jugglerRotY - PROP_JUGGLING_DECAY_ROT_Y;
  }

  // set anim rot (y)
  auto animRotY = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotY"));
  animRotY->setDuration((int)(DWELL_TIME * S_TO_MS));
  animRotY->setStartValue(m_propRotY); // NOTE: sure ?
  animRotY->setEndValue(finalRotY);
  animRotY->setLoopCount(ONE_LOOP);

  // upadate prop rotY
  m_propRotY = finalRotY;
  qDebug() << "rotY after dwell animation" << m_propRotY;

  return animRotY;
}

QSequentialAnimationGroup *PropAnim::dwellPassingTranslationAnim(const Juggler *t_jugglerLaunch,
                                                                 const Juggler *t_jugglerReceive,
                                                                 const QVector3D t_startPos,
                                                                 const QVector3D t_endPos,
                                                                 hand t_handReceive)
{
  /* si c'est un passing reculer la main, jusqu'à z = 0
  sur le point aligné entre la main du reciever
  et le point de départ (m_pos*Hint) */

  auto translationAnimGroup = new QSequentialAnimationGroup();

  // get vec where launch will arrive
  auto handRecievePos = getExtHandPos(t_jugglerReceive,
                                      t_handReceive);

  // transform in jugglerLaunch vec
  auto handRecievePosRelative = t_jugglerLaunch->worldVecToJugglerVec(handRecievePos);
  auto posFinalRelative = t_jugglerLaunch->worldVecToJugglerVec(t_endPos);

  // determine vec where launching hand will get for preparing launch
  // find line coeff
  float lineCoeff = (handRecievePosRelative.z() - posFinalRelative.z())
      / (handRecievePosRelative.x() - posFinalRelative.x());
  // find origine ordinate
  float originOrdinate = handRecievePosRelative.z()
      - (lineCoeff * handRecievePosRelative.x());
  // our vec will be at z = 0
  // find x pos
  float vecX = - (originOrdinate / lineCoeff);
  // create our vec
  QVector3D handBackVecRelative(vecX, HAND_BACKEST_PASSING_Y, 0);
  // get back to world coordonate
  auto handBackVec = t_jugglerLaunch->jugglerVecToWorldVec(handBackVecRelative);

  // determine number of frames
  int frameCount = (int)((DWELL_TIME / DELTA_TIME));

  // faire anim arc de cercle jusqu'à ce point
  // puis jusqu'au point de lancement
  auto v_twoQuarterCircular = Curves::curveTwoQuarterCircular(t_startPos,
                                                              handBackVec,
                                                              t_endPos,
                                                              frameCount);
  // loop creating our tranlation anim
  for (int i = 0; i < frameCount; i++)
  {
    auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
    translationAnim->setDuration((int)(DELTA_TIME * S_TO_MS));
    translationAnim->setStartValue(v_twoQuarterCircular.at(i));
    translationAnim->setEndValue(v_twoQuarterCircular.at(i + 1));
    translationAnim->setLoopCount(ONE_LOOP);
    translationAnimGroup->addAnimation(translationAnim);
  }
  return translationAnimGroup;
}

QPropertyAnimation *PropAnim::dwellSelfLaunch1TranslationAnim(const QVector3D t_startPos,
                                                              const QVector3D t_endPos)
{
  auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
  translationAnim->setDuration((int)(DWELL_TIME_LAUNCH1 * S_TO_MS));
  translationAnim->setStartValue(t_startPos);
  translationAnim->setEndValue(t_endPos);
  translationAnim->setLoopCount(ONE_LOOP);

  return translationAnim;
}

QPropertyAnimation *PropAnim::dwellClubPassingRotYAnim(const Juggler *t_jugglerReceive,
                                                       const QVector3D t_endPos,
                                                       hand t_handReceive)
{
  // get position where launch arrives
  auto dirVec = getExtHandPos(t_jugglerReceive,
                                 t_handReceive);

  // set final angle
  float finalRotY = -qRadiansToDegrees(qAtan2(dirVec.z() - t_endPos.z(),
                                              dirVec.x() - t_endPos.x()));

  // set anim rot (y)
  auto animRotY = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotY"));
  animRotY->setDuration((int)(DWELL_TIME * S_TO_MS));
  animRotY->setStartValue(m_propRotY); // NOTE: sure ?
  animRotY->setEndValue(finalRotY);
  animRotY->setLoopCount(ONE_LOOP);

  // upadate prop rotY
  m_propRotY = finalRotY;

  return animRotY;
}

