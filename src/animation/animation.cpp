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
{
  // bool to know if we need to enlarge our juggling
  m_isExtPlusCatch = (m_propType == ring  && m_launchType == panCake) ||
      (m_propType == club && m_launchType == helicopter);

}

void Animation::setSiteSwap(SiteSwap *t_siteSwap)
{
  m_siteSwap = t_siteSwap;
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

  setPropAnim();
  setHandAnim();

}

void Animation::setPropAnim()
{
  // we calculate state.
  QBitArray state = m_siteSwap->getState();
  // we get all the loops for each prop
  auto v_propAnimEvents = m_siteSwap->getTotalPropAnimEvents();

  int propId = 0;

  for (int i = 0; i < state.size(); i++) // for each bit in state
  {
    if (state.testBit(i)) // if it's a launching site
    {
      // prendre la bonne boucle de v_v_propAnimEvents
      auto propAnimEvents = v_propAnimEvents.at(propId);
      // on crée un PropAnim
      auto propAnim = new PropAnim(m_v_juggler,
                                   m_v_prop.at(propId),
                                   propId,
                                   m_propType,
                                   m_launchType);


      // créer une seq pour décaler le début
      auto seqAnimForDelay = new QSequentialAnimationGroup();
      // NOTE: we assume that each juggler start juggling at the same time
      seqAnimForDelay->addPause((int)((i / m_v_juggler.size()) * (HAND_PERIOD / 2) * S_TO_MS));
      // on lui donne un vec d'propAnimEvent, il s'en occupe
      propAnim->setAnim(propAnimEvents);
      seqAnimForDelay->addAnimation(propAnim);
      addAnimation(seqAnimForDelay);

//      qDebug() << "lenght of prop anim n°" << i << propAnim->duration();

      propId++;
    }
  }

}

void Animation::setHandAnim()
{
  auto v_v_handAnimEvents = m_siteSwap->getTotalHandAnimEvents();

  for (int i = 0; i < m_v_juggler.size(); i++)
  {
    // right hand
    auto rightHandAnim = new HandAnim(m_v_juggler,
                                      i,
                                      hand(rightHand),
                                      m_v_prop,
                                      m_isExtPlusCatch);

    rightHandAnim->setAnim(v_v_handAnimEvents.at(2 * i));
    addAnimation(rightHandAnim);

    /***************** check *******************/
//      auto v_handAnimEvents = v_v_handAnimEvents.at(2 * i);
//      qDebug() << "HAND ANIM n° : " << 2 * i;
//      for (int j = 0; j < v_handAnimEvents->size(); j++)
//      {
//        auto myHandAnimEvents = v_handAnimEvents->at(j);
//        qDebug() << "EVENT n° : " << j;
//        qDebug() << "*start time" << myHandAnimEvents->s_startTime;
//        qDebug() << "*prop id" << myHandAnimEvents->s_propId;
//        qDebug() << "*rec hand" << myHandAnimEvents->s_receiveHand;
//      }

    // left hand
    // add delay
    auto seqForDelay = new QSequentialAnimationGroup();

    // TODO: change this for synchron
    seqForDelay->addPause((int)((HAND_PERIOD / 2) * S_TO_MS));

    auto leftHandAnim = new HandAnim(m_v_juggler,
                                     i,
                                     hand(leftHand),
                                     m_v_prop,
                                     m_isExtPlusCatch);

    leftHandAnim->setAnim(v_v_handAnimEvents.at((2 * i) + 1));
    seqForDelay->addAnimation(leftHandAnim);
    addAnimation(seqForDelay);

    /***************** check *******************/
//    v_handAnimEvents = v_v_handAnimEvents.at((2 * i) + 1);
//    qDebug() << "HAND ANIM n° : " << (2 * i) + 1;
//    for (int j = 0; j < v_handAnimEvents->size(); j++)
//    {
//      auto myHandAnimEvents = v_handAnimEvents->at(j);
//      qDebug() << "EVENT n° : " << j;
//      qDebug() << "*start time" << myHandAnimEvents->s_startTime;
//      qDebug() << "*prop id" << myHandAnimEvents->s_propId;
//      qDebug() << "*rec hand" << myHandAnimEvents->s_receiveHand;
//    }

  }
}


