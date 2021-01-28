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

  // TODO: if 0 launch don't create anything, just pause
  // create particular first dwell with good rot

  // là on met tout le merdier du 1er lancement ? donné par siteswap ?
  // 1er lancer, bien orienter les objets, les placer dans les bonnes mains
  // créer les 1ers dwells avec plusieurs objets dans les mains
  // ou on fait tout ça dans animation ?

  // pour les mains, principe de la timeline, chaque mouvement de main doit avoir
  // un temps de début et un temps de fin, voir dans handanim. Connecter les objets ?

  /************************testing zone ****************************/
  auto testAnim = dwellAnim(0,
                            hand(rightHand),
                            1,
                            hand(leftHand),
                            5);

  auto testAnim1 = parabolicAnim(0,
                                 hand(rightHand),
                                 1,
                                 hand(leftHand),
                                 5);

  auto testAnim2 = dwellAnim(1,
                             hand(leftHand),
                             1,
                             hand(rightHand),
                             3);

  auto testAnim3 = parabolicAnim(1,
                                 hand(leftHand),
                                 1,
                                 hand(rightHand),
                                 3);

  auto testAnim4 = dwellAnim(1,
                             hand(rightHand),
                             0,
                             hand(leftHand),
                             5);

  auto testAnim5 = parabolicAnim(1,
                                 hand(rightHand),
                                 0,
                                 hand(leftHand),
                                 5);

  auto testAnim6 = dwellAnim(0,
                             hand(leftHand),
                             0,
                             hand(rightHand),
                             3);

  auto testAnim7 = parabolicAnim(0,
                                 hand(leftHand),
                                 0,
                                 hand(rightHand),
                                 3);

  addAnimation(testAnim);
  addAnimation(testAnim1);
  addAnimation(testAnim2);
  addAnimation(testAnim3);
  addAnimation(testAnim4);
  addAnimation(testAnim5);
  addAnimation(testAnim6);
  addAnimation(testAnim7);
  setLoopCount(INFINITE_LOOP);

}

QParallelAnimationGroup *PropAnim::parabolicAnim(int t_jugglerIdLaunch,
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

  // declare our 2 positions
  QVector3D posProp; // pos where it starts
  QVector3D posFinal; // pos where it should finish

  // bool to know if it's a passing launch
  bool isPassing = (t_jugglerIdLaunch != t_jugglerIdRecieve);

  // set position were parabolic curve starts
  // we assume we always send from hand int position
  t_handLaunch == leftHand ?
        posProp = jugglerLaunch->getPositionLHint():
      posProp = jugglerLaunch->getPositionRHint();

  // set position where it ends
  if (t_handRecieve == leftHand)
  {
    m_isExtPlusCatch ? // helico, pancakes ?
                       posFinal = jugglerRecieve->getPositionLHextPlus():
        posFinal = jugglerRecieve->getPositionLHext();
  }
  else
  {
    m_isExtPlusCatch ?
          posFinal = jugglerRecieve->getPositionRHextPlus():
        posFinal = jugglerRecieve->getPositionRHext();
  }

  // calculate arc time
  float arcTime;
  if (t_launch == 1) // For launch 1 Shannon doesn't work
  {
    arcTime = LAUNCH1_TIME;
  }
  else // Shannon Theorem
    arcTime = ((HAND_PERIOD) / 2) * (t_launch - (2 * DWELL_RATIO));

  // we calculate velocity launch
  auto velBall = ((posFinal - posProp) - 0.5 *
                  (GRAVITY * qPow(arcTime, 2))) / arcTime;

  int frameCount = (int)((arcTime / (DELTA_TIME)));

  // We create our curve
  auto v_parabolic = Curves::curveParabolic(velBall, posProp, frameCount);

  // create sequential for translation
  auto translationAnimGroup = new QSequentialAnimationGroup(this);

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
    float initialRotX = m_prop->getRotX(); // NOTE: not sure of that
    // maybe we have to store the rotX in an argument...

    float addRotX = 0;
    // if passing launch add some rot
    if (isPassing)
      addRotX = ((initialRotX - 90.0f) * 2.0f);

    // adjust values if necessary
    // no rotX for normal ring
    if (m_propType == ring &&
        m_launchTypeRing == normalRing)
    {
      rotXCount = 0;
      addRotX = 0;
    }

    // change rotX for normal club
    if (m_propType == club &&
        m_launchTypeClub == normalClub)
    {
      // if passing, club must be vertical in recieving hand
      if (isPassing)
        addRotX = (90.0f - (3 * initialRotX)) / 2.0f; // - initX - (initX - 90)/2
      else
      {
        initialRotX = m_prop->getRotX(); // NOTE: not sure of that
        addRotX = -addRotX; // maybe this can be correctly set in dwell
      }
    }

    if (m_propType == club &&
        m_launchTypeClub == flat)
    {
      rotXCount = 0; // no rot it's flat
      // but if passing, club must be vertical in recieving hand
      if (isPassing)
        addRotX = - initialRotX;
    }

    // helicopter workaround
    if (m_propType == club &&
        m_launchTypeClub == helicopter)
    {
      // rotation (x) become (y)
      int rotYCount = rotXCount;
      float initialRotY = m_prop->getRotY();
      float endValue = jugglerRecieve->getRotY();
      t_handRecieve == leftHand ? // NOTE: doesn't seem totally good
                                  endValue += 90 - (rotYCount * 360):
          endValue -= 90 - (rotYCount * 360);
      auto animRotY = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotY"));
      animRotY->setDuration(((int)(DELTA_TIME * S_TO_MS)) * frameCount);
      animRotY->setStartValue(initialRotY);
      animRotY->setEndValue(endValue);
      animRotY->setLoopCount(ONE_LOOP);
      parallelAnimGroup->addAnimation(animRotY);

      return parallelAnimGroup;
    }

    // rotation X
    auto animRotX = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"));
    animRotX->setDuration(((int)(DELTA_TIME * S_TO_MS)) * frameCount);
    animRotX->setStartValue((360 * rotXCount) + initialRotX);
    animRotX->setEndValue(initialRotX + addRotX);
    animRotX->setLoopCount(ONE_LOOP);
    parallelAnimGroup->addAnimation(animRotX);

//    qDebug() << "anim rotx start" << animRotX->startValue() << "anim rotx end" << animRotX->endValue();
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
  // TODO: make private methods
//  qDebug() << "rotY au début" << m_prop->getRotY();
  // create parallel animation group to translate and rotation if needed
  auto parallelAnimGroup = new QParallelAnimationGroup();

  // get our concerned jugglers
  auto jugglerLaunch = m_v_juggler.at(t_jugglerIdLaunch); // TODO: check out of range
  auto jugglerRecieve = m_v_juggler.at(t_jugglerIdRecieve); // TODO: check out of range

  // declare our 2 positions
  QVector3D posProp; // pos where it starts
  QVector3D posFinal; // pos where it should finish

  // bool to know if it's a passing launch
  bool isPassing = (t_jugglerIdLaunch != t_jugglerIdRecieve);

  // set position were circular curve starts
  if (t_handLaunch == leftHand)
  {
    m_isExtPlusCatch ?
          posProp = jugglerLaunch->getPositionLHextPlus():
        posProp = jugglerLaunch->getPositionLHext();
  }
  else
  {
    m_isExtPlusCatch ?
          posProp = jugglerLaunch->getPositionRHextPlus():
        posProp = jugglerLaunch->getPositionRHext();
  }

  // set position where it ends
  t_handLaunch == leftHand ?
        posFinal = jugglerLaunch->getPositionLHint():
      posFinal = jugglerLaunch->getPositionRHint();

  // create sequential for translation
  auto translationAnimGroup = new QSequentialAnimationGroup(this);

  // determine number of frames
  int frameCount = (int)((DWELL_TIME / DELTA_TIME));

  if (isPassing)
  {
    // si c'est un passing reculer la main, jusqu'à z = 0
    // sur le point aligné entre la main du reciever
    // et le point de départ (m_pos*Hint)

    // get vec where launch will arrive
    QVector3D handRecievePos;
    if (t_handRecieve == leftHand)
    {
      m_isExtPlusCatch ?
            handRecievePos = jugglerRecieve->getPositionLHextPlus():
          handRecievePos = jugglerRecieve->getPositionLHext();
    }
    else
    {
      m_isExtPlusCatch ?
            handRecievePos = jugglerRecieve->getPositionRHextPlus():
          handRecievePos = jugglerRecieve->getPositionRHext();
    }

    // transform in jugglerLaunch vec
    auto handRecievePosRelative = jugglerLaunch->worldVecToJugglerVec(handRecievePos);
    auto posFinalRelative = jugglerLaunch->worldVecToJugglerVec(posFinal);

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
    auto handBackVec = jugglerLaunch->jugglerVecToWorldVec(handBackVecRelative);

    // faire anim arc de cercle jusqu'à ce point
    // puis jusqu'au point de lancement
    auto v_twoQuarterCircular = Curves::curveTwoQuarterCircular(posProp,
                                                                handBackVec,
                                                                posFinal,
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
    // add to return anim
    parallelAnimGroup->addAnimation(translationAnimGroup);

    // make all rotation

    // return
    return parallelAnimGroup;
  }

  else // it's self launch
  {
    // determine axis for curve, on the plane in front of juggler
    float jugglerRotY = jugglerLaunch->getRotY();

    // translate for 1 launch
    if (t_launch == 1) // that's special for launch 1
    {
      auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
      translationAnim->setDuration((int)(DWELL_TIME_LAUNCH1 * S_TO_MS));
      translationAnim->setStartValue(posProp);
      translationAnim->setEndValue(posFinal);
      translationAnim->setLoopCount(ONE_LOOP);
      translationAnimGroup->addAnimation(translationAnim); // in that case it's useless
                                                           // but to be coherent with other cases
                                                           // add to return anim
      // add to return anim
      parallelAnimGroup->addAnimation(translationAnimGroup);
    }
    else // translate for launch > 1
    {

      // we create our curve
      QVector<QVector3D> v_semiCircular = Curves::curveSemiCircular(posProp,
                                                                    posFinal,
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
      // add to return anim
      parallelAnimGroup->addAnimation(translationAnimGroup);

//       make all the rotation

//      // return anim
//      return parallelAnimGroup;
    }

    // let's rotate if necessary
    // give a little (x) rotation for normal and flat club.
    if (m_propType == club && (m_launchTypeClub == normalClub || m_launchTypeClub == flat))
    {
      // create a sequential anim group cos we have 2 property animations
      auto animRotX = new QSequentialAnimationGroup();

      // it's a club we have to know it's rot (x), because it may have been passed
      float propRotX = m_prop->getRotX();

      // final rotX
      float finalRotX = CLUB_BASIC_ROTX; // we always send with this angle

      // mid angle
      float midAngleRotX;
      // if prop's just been passed
      if (propRotX != finalRotX)
      {
        midAngleRotX = propRotX + ((propRotX - finalRotX) / 2);
      }
      else
      {
        (t_launch == 1) ?
          midAngleRotX = propRotX: // the X rot will be neutralized
          midAngleRotX = propRotX + CLUB_DWELL_X_DECAY;
      }

      // create first anim to low down angle
      auto animRotX1 = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"));
      animRotX1->setDuration((int)(DWELL_TIME * S_TO_MS / 2));
      animRotX1->setStartValue(propRotX);
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

      // add sequential to return parallel anim group
      parallelAnimGroup->addAnimation(animRotX);
    }

    // give a little (y) rotation for normal ring and normal and flat club
    if ((m_propType == ring && m_launchTypeRing == normalRing) ||
        (m_propType == club && (m_launchTypeClub == normalClub || m_launchTypeClub == flat)))
    {
      // get (y) rot prop
      float propRotY = m_prop->getRotY();

      // set final (y) rot
      float finalRotY;
      // angle will depend on side and oddity of launch
      if (t_handLaunch == leftHand)
      {
        t_launch % 2 ? // is it even ?
                       finalRotY = jugglerRotY - PROP_JUGGLING_DECAY_ROT_Y:
            finalRotY = jugglerRotY + PROP_JUGGLING_DECAY_ROT_Y;
      }
      else
      {
        t_launch % 2 ? // is it even ?
                       finalRotY = jugglerRotY + PROP_JUGGLING_DECAY_ROT_Y:
            finalRotY = jugglerRotY - PROP_JUGGLING_DECAY_ROT_Y;
      }
//      qDebug() << "propRotY" << propRotY << "finalRotY" << finalRotY;

      // set anim rot (y)
      auto animRotY = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotY"));
      animRotY->setDuration((int)(DWELL_TIME * S_TO_MS));
      animRotY->setStartValue(propRotY);
      animRotY->setEndValue(finalRotY);
      animRotY->setLoopCount(ONE_LOOP);
      parallelAnimGroup->addAnimation(animRotY);

      // we can return
      return parallelAnimGroup;
    }

    // NOTE: check
    return parallelAnimGroup;
  }
}

