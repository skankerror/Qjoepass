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
  QBitArray state = m_siteSwap->getState();
  // we get all the loops for each prop
  auto v_v_propAnimEvents = m_siteSwap->getTotalPropAnimEvents();

  int propId = 0;

  for (int i = 0; i < state.size(); i++) // for each bit in state
  {
    if (state.testBit(i)) // if it's a launching site
    {
      // prendre la bonne boucle de v_v_propAnimEvents
      auto v_propAnimEvents = v_v_propAnimEvents.at(propId);
      // on crée un PropAnim
      auto propAnim = new PropAnim(m_v_juggler,
                                   m_v_prop.at(propId),
                                   propId,
                                   m_propType,
                                   m_launchType);

      // we had delay according to place in state

      // créer une seq pour décaler le début
      auto seqAnimForDelay = new QSequentialAnimationGroup();
      // NOTE: we assume that each juggler start juggling at the same time
      seqAnimForDelay->addPause((int)((i / m_v_juggler.size()) * (HAND_PERIOD / 2) * S_TO_MS));
      // on lui donne un vec d'propAnimEvent, il s'en occupe
      propAnim->setAnim(v_propAnimEvents);
      seqAnimForDelay->addAnimation(propAnim);
      addAnimation(seqAnimForDelay);

      qDebug() << "lenght of prop anim n°" << i << propAnim->duration();

      propId++;
    }
  }
}


