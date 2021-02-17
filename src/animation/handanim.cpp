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

#include "handanim.h"
#include "handdwellanim.h"

HandAnim::HandAnim(QVector<Juggler *> t_v_juggler,
                   int t_jugglerId,
                   hand t_hand,
                   QVector<JugglingProp *> t_v_prop,
                   bool t_isExtCatchPlus,
                   QObject *parent)
  : QSequentialAnimationGroup(parent),
    m_v_juggler(t_v_juggler),
    m_juggler(m_v_juggler.at(t_jugglerId)), // FIXME: no check ?
    m_hand(t_hand),
    m_v_prop(t_v_prop),
    m_isExtPlusCatch(t_isExtCatchPlus)
{}

void HandAnim::setAnim(QVector<handAnimEvent *> *t_v_handAnimEvents)
{
  for (int i = 0; i < t_v_handAnimEvents->size(); i++)
  {
    // dwell vars
    auto myHandAnimEvent = t_v_handAnimEvents->at(i);
    int launch = myHandAnimEvent->s_launch;
    int startTime = myHandAnimEvent->s_startTime;
    int dwellDuration;

    // empty hand vars
    handAnimEvent *nextHandAnimEvent = nullptr;
    auto initialPos = getIntHandPos(m_juggler, m_hand);
    auto finalPos = getExtHandPos(m_juggler, m_hand);
    int duration;

    // déterminer quand ça va démarrer, si y a un décalage rajouter une pause
    if (i == 0)
    {
      int pauseTime;
      // right hand ?
      (m_hand == rightHand) ?
            pauseTime = startTime :
          pauseTime = startTime - (int)((HAND_PERIOD / 2) * S_TO_MS);
      addPause(pauseTime);
      qDebug() << "event n° : " << i << "Pause time begining : " << startTime;

    }

    auto myDwellAnim = dwellAnim(myHandAnimEvent->s_propId,
                                 myHandAnimEvent->s_launch,
                                 myHandAnimEvent->s_jugglerReceiveId,
                                 myHandAnimEvent->s_receiveHand);
    addAnimation(myDwellAnim);
    (launch == 1) ?
          dwellDuration = DWELL_TIME_LAUNCH1 * S_TO_MS :
        dwellDuration = DWELL_TIME * S_TO_MS;
    // if it's last event ?
    if (i == t_v_handAnimEvents->size() - 1)
    {
      nextHandAnimEvent = t_v_handAnimEvents->at(0);
      int nextHandAnimEventStartTime = nextHandAnimEvent->s_startTime;
      qDebug() << "start Time ?" << nextHandAnimEventStartTime;
      if (!nextHandAnimEventStartTime) // start time = 0 ?
      {
        // we can get loop duration
        duration = nextHandAnimEvent->s_loopDuration - startTime - dwellDuration;
      }
      else
      {
//        duration = nextHandAnimEvent->s_loopDuration - startTime - dwellDuration;
        duration = (int)((HAND_PERIOD * S_TO_MS) - dwellDuration);
      }
    }
    else
    {
      nextHandAnimEvent = t_v_handAnimEvents->at(i + 1); // else get next
      duration = nextHandAnimEvent->s_startTime - startTime - dwellDuration;
    }


    // FIXME: doesn't work for all case

    qDebug() << "event n° : " << i << "empty hand Duration : " << duration;
    // pass propId to
    auto myEmptyHandAnim = emptyHandAnim(initialPos,
                                         finalPos,
                                         duration);

    addAnimation(myEmptyHandAnim);
  }
  setLoopCount(INFINITE_LOOP);
}

QParallelAnimationGroup *HandAnim::dwellAnim(int t_propId,
                                               int t_launch,
                                               int t_jugglerReceiveId,
                                               hand t_receiveHand)
{
  auto jugglerReceive = m_v_juggler.at(t_jugglerReceiveId); // TODO: check out of range

  // pos where it starts
  QVector3D initialPos = getExtHandPos(m_juggler,
                                       m_hand);
  // pos where it should finish
  QVector3D finalPos = getIntHandPos(m_juggler,
                                     m_hand);

  // bool to know if it's a passing launch
  bool isPassing = (m_juggler != jugglerReceive);

  auto prop = m_v_prop.at(t_propId);

  HandDwellAnim *myDwellAnim = nullptr;

  if (isPassing)
  {
    // set medPos for passing
    auto medPos = setMedPosForPassingDwell(finalPos,
                                           jugglerReceive,
                                           t_receiveHand);

    // create our dwell anim
    myDwellAnim = new HandDwellAnim(m_juggler,
                                    m_hand,
                                    prop,
                                    initialPos,
                                    medPos,
                                    finalPos,
                                    t_launch);

  }
  else
  {
    myDwellAnim = new HandDwellAnim(m_juggler,
                                    m_hand,
                                    prop,
                                    initialPos,
                                    finalPos,
                                    t_launch);
  }
  return myDwellAnim;
}

QSequentialAnimationGroup *HandAnim::emptyHandAnim(QVector3D t_initialPos,
                                                   QVector3D t_finalPos,
                                                   int t_duration)
{
  auto retSeqAnimGroup = new QSequentialAnimationGroup();

  QPropertyAnimation *propertyAnim = nullptr;

  (m_hand == leftHand) ?
        propertyAnim = new QPropertyAnimation(m_juggler,
                                              QByteArrayLiteral("m_leftHandPosition")):
      propertyAnim = new QPropertyAnimation(m_juggler,
                                            QByteArrayLiteral("m_rightHandPosition"));

  propertyAnim->setDuration(t_duration);
  propertyAnim->setStartValue(t_initialPos);
  propertyAnim->setEndValue(t_finalPos);

  retSeqAnimGroup->addAnimation(propertyAnim);

  return retSeqAnimGroup;
}

QVector3D HandAnim::getExtHandPos(const Juggler *t_juggler,
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

QVector3D HandAnim::getIntHandPos(const Juggler *t_juggler,
                                  const hand t_side) const
{
  QVector3D retVector;
  t_side == leftHand ?
        retVector = t_juggler->getPositionLHint():
      retVector = t_juggler->getPositionRHint();
  return retVector;

}

QVector3D HandAnim::setMedPosForPassingDwell(QVector3D t_finalPos,
                                             Juggler *t_jugglerReceive,
                                             hand t_receiveHand)
{
  // get vec where launch will arrive
  auto handReceivePos = getExtHandPos(t_jugglerReceive,
                                      t_receiveHand);

  // transform in jugglerLaunch vec
  auto handReceivePosRelative = m_juggler->worldVecToJugglerVec(handReceivePos);
  auto posFinalRelative = m_juggler->worldVecToJugglerVec(t_finalPos);

  // determine vec where launching hand will get for preparing launch
  // find line coeff
  float lineCoeff = (handReceivePosRelative.z() - posFinalRelative.z())
      / (handReceivePosRelative.x() - posFinalRelative.x());
  // find origine ordinate
  float originOrdinate = handReceivePosRelative.z()
      - (lineCoeff * handReceivePosRelative.x());
  // find x pos
  float vecX = - (originOrdinate / lineCoeff);
  // create our vec
  QVector3D handBackVecRelative(vecX,
                                HAND_BACKEST_PASSING_Y,
                                HAND_BACKEST_PASSING_Z);

  // get back to world coordonate
  auto medPos = m_juggler->jugglerVecToWorldVec(handBackVecRelative);

  return medPos;

}
