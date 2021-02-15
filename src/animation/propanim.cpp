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

}

void PropAnim::setAnim(QVector<propAnimEvent *> t_v_propAnimEvents)
{
  for (int i = 0; i < t_v_propAnimEvents.size(); i++)
  {
    auto propAnimEvent = t_v_propAnimEvents.at(i);

    /* this is a vanilla launch, so we must adjust launch according to number of jugglers
     * we create an adjust int. */
    int jugglerLaunchId = propAnimEvent->s_jugglerLaunchId;
    int jugglerReceiveId = propAnimEvent->s_jugglerReceiveId;
    int adjustLaunch = jugglerLaunchId - jugglerReceiveId;
    int launch = propAnimEvent->s_launch;
    int realLaunch = (launch + adjustLaunch) / m_v_juggler.size();
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
                                                 int t_jugglerIdReceive,
                                                 hand t_handReceive,
                                                 int t_launch)
{
  // create our return anim
  PropParabolicAnim *parabolicAnimGroup = nullptr;

  // get our concerned jugglers
  auto jugglerLaunch = m_v_juggler.at(t_jugglerIdLaunch); // TODO: check out of range
  auto jugglerReceive = m_v_juggler.at(t_jugglerIdReceive); // TODO: check out of range

  // find our 2 positions
  auto initialPos = getIntHandPos(jugglerLaunch, t_handLaunch); // pos where it starts
  auto finalPos = getExtHandPos(jugglerReceive, t_handReceive); // pos where it should finish


  // bool to know if it's a passing launch
  bool isPassing = (t_jugglerIdLaunch != t_jugglerIdReceive);

  if (m_propType == ball)
  {
    parabolicAnimGroup = new PropParabolicAnim(m_prop,
                                               initialPos,
                                               finalPos,
                                               t_launch);
  }
  else // else we must rotate prop
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
//      float endValue = jugglerReceive->getRotY();
//      t_handReceive == leftHand ? // NOTE: doesn't seem totally good
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
    float startRotX = (360 * rotXCount) + initialRotX;
    float finalRotX = initialRotX + addRotX;

    parabolicAnimGroup = new PropParabolicAnim(m_prop,
                                               initialPos,
                                               finalPos,
                                               t_launch,
                                               startRotX,
                                               finalRotX);

    m_propRotX = initialRotX + addRotX;

  }
  return parabolicAnimGroup;
}

QParallelAnimationGroup *PropAnim::dwellAnim(int t_jugglerIdLaunch,
                                             hand t_handLaunch,
                                             int t_jugglerIdReceive,
                                             hand t_handReceive,
                                             int t_launch)
{
  // get our concerned jugglers
  auto jugglerLaunch = m_v_juggler.at(t_jugglerIdLaunch); // TODO: check out of range
  auto jugglerReceive = m_v_juggler.at(t_jugglerIdReceive); // TODO: check out of range

  // pos where it starts
  QVector3D initialPos = getExtHandPos(jugglerLaunch,
                                       t_handLaunch);
  // pos where it should finish
  QVector3D finalPos = getIntHandPos(jugglerLaunch,
                                     t_handLaunch);

  // bool to know if it's a passing launch
  bool isPassing = (t_jugglerIdLaunch != t_jugglerIdReceive);

  PropDwellAnim *dwellAnim = nullptr;

  if (m_propType == ball) // no rotation, only translation
  {
    if (isPassing)
    {
      // set medPos for passing
      auto medPos = setMedPosForPassingDwell(finalPos,
                                             jugglerLaunch,
                                             jugglerReceive,
                                             t_handReceive);

      // create our dwell anim
      dwellAnim = new PropDwellAnim(m_prop,
                                    initialPos,
                                    medPos,
                                    finalPos,
                                    t_launch);
    }
    else
    {
      // create our dwell anim
      dwellAnim = new PropDwellAnim(m_prop,
                                    initialPos,
                                    finalPos,
                                    t_launch);
    }
  }
  else // make translation and rotation
  {
    // make x y rotations for club
    if (m_propType == club &&
        (m_launchTypeClub == normalClub ||
         m_launchTypeClub == flat))
    {
      // initial rotX
      float initialRotX = m_propRotX;

      // final rotX
      float finalRotX = CLUB_BASIC_ROTX; // we always send with this angle

      // initial rotY
      float initialRotY = m_propRotY;

      // final rotY
      float finalRotY;

      if (isPassing)
      {
        // set medPos for passing
        auto medPos = setMedPosForPassingDwell(finalPos,
                                               jugglerLaunch,
                                               jugglerReceive,
                                               t_handReceive);

        // get position where launch arrives
        auto dirVec = getExtHandPos(jugglerReceive,
                                       t_handReceive);

        // set final angle
        finalRotY = -qRadiansToDegrees(qAtan2(dirVec.z() - finalPos.z(),
                                                    dirVec.x() - finalPos.x()));


        dwellAnim = new PropDwellAnim(m_prop,
                                      initialPos,
                                      medPos,
                                      finalPos,
                                      t_launch,
                                      initialRotX,
                                      finalRotX,
                                      initialRotY,
                                      finalRotY);
      }
      else
      {
        // angle will depend on side and oddity of launch
        if (t_handLaunch == leftHand)
        {
          t_launch % 2 ? // is it even ?
                         finalRotY = jugglerLaunch->getRotY() - PROP_JUGGLING_DECAY_ROT_Y:
              finalRotY = jugglerLaunch->getRotY() + PROP_JUGGLING_DECAY_ROT_Y;
        }
        else
        {
          t_launch % 2 ? // is it even ?
                         finalRotY = jugglerLaunch->getRotY() + PROP_JUGGLING_DECAY_ROT_Y:
              finalRotY = jugglerLaunch->getRotY() - PROP_JUGGLING_DECAY_ROT_Y;
        }

        dwellAnim = new PropDwellAnim(m_prop,
                                      initialPos,
                                      finalPos,
                                      t_launch,
                                      initialRotX,
                                      finalRotX,
                                      initialRotY,
                                      finalRotY);

      }
      // we update prop rotation
      m_propRotX = finalRotX;
      m_propRotY = finalRotY;
    }
  }
  return dwellAnim;
}

QSequentialAnimationGroup* PropAnim::dwellParabolicAnim(int t_jugglerIdLaunch,
                                                        hand t_handLaunch,
                                                        int t_jugglerIdReceive,
                                                        hand t_handReceive,
                                                        int t_launch)
{
  auto dwell = dwellAnim(t_jugglerIdLaunch,
                         t_handLaunch,
                         t_jugglerIdReceive,
                         t_handReceive,
                         t_launch);

  auto parabolic = parabolicAnim(t_jugglerIdLaunch,
                                 t_handLaunch,
                                 t_jugglerIdReceive,
                                 t_handReceive,
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

QVector3D PropAnim::setMedPosForPassingDwell(QVector3D t_finalPos,
                                             Juggler *t_jugglerLaunch,
                                             Juggler *t_jugglerReceive,
                                             hand t_handReceive)
{
  // get vec where launch will arrive
  auto handReceivePos = getExtHandPos(t_jugglerReceive,
                                      t_handReceive);

  // transform in jugglerLaunch vec
  auto handReceivePosRelative = t_jugglerLaunch->worldVecToJugglerVec(handReceivePos);
  auto posFinalRelative = t_jugglerLaunch->worldVecToJugglerVec(t_finalPos);

  // determine vec where launching hand will get for preparing launch
  // find line coeff
  float lineCoeff = (handReceivePosRelative.z() - posFinalRelative.z())
      / (handReceivePosRelative.x() - posFinalRelative.x());
  // find origine ordinate
  float originOrdinate = handReceivePosRelative.z()
      - (lineCoeff * handReceivePosRelative.x());
  // our vec will be at z = 0
  // find x pos
  float vecX = - (originOrdinate / lineCoeff);
  // create our vec
  QVector3D handBackVecRelative(vecX, HAND_BACKEST_PASSING_Y, 0);
  // get back to world coordonate
  auto medPos = t_jugglerLaunch->jugglerVecToWorldVec(handBackVecRelative);

  return medPos;
}
