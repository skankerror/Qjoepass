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

#include "siteswap.h"
#include <QDebug>

SiteSwap::SiteSwap(QVector<SiteswapEvent*> &t_v_Int,
                   int t_jugCount,
                   jugglingProp t_prop,
                   bool t_synchron,
                   QObject *parent)
  : QObject(parent),
    m_v_event(t_v_Int),
    m_synchron(t_synchron),
    m_prop(t_prop),
    m_jugglerCount(t_jugCount)
{
  m_period = m_v_event.size();
  m_valid = isValid();
  m_propCount = getNumProp();
  if (m_valid)
    setState();
}

bool SiteSwap::isValid() const
{
  // create a vector to test our values
  QVector<int> v_test;
  // let's be optimistic !
  bool ret = true;

  for (int i = 0; i < m_period; i++)
  {
    // we modulo each launch in order to find where this launch will be in next loop
    int modLaunch = (at(i) + i) % m_period;
    // and test to find collision
    if (v_test.indexOf(modLaunch) != -1) // collision
    {
      ret = false; // bad news
      break;
    }
    // this seems good, we're adding to our vector for next tests
    v_test.append(modLaunch);
  }
  return ret;
}

int SiteSwap::getNumProp() const
{
  if(!m_valid)
    return 0;

  int totalLaunch = 0;
  for (int i = 0; i < m_v_event.size(); i++)
    totalLaunch += at(i);
  return totalLaunch / m_period;
  // TODO: multiply by numbers of jugglers
}

void SiteSwap::setPropType(jugglingProp t_prop)
{
  m_prop = t_prop;
}

QVector<AnimEvent*> SiteSwap::getAnimEvents(int launchPos, hand handLaunch, int jugLaunchId)
{
  QVector<AnimEvent*> returnVec;

  hand initialHandLaunch = handLaunch; // to keep it
  int initialLaunchPos = launchPos; // idem
  int initialJugLaunchId = jugLaunchId;

  int launch = at(launchPos);
  hand newLaunchHand;
  (launch % 2 == 1) ? newLaunchHand = changeHand(handLaunch) : newLaunchHand = handLaunch;
  int newLaunchPos = (launch + launchPos) % m_period;
  int newLaunch = at(newLaunchPos);
  int newJugId = m_v_event.at(launchPos)->getReceiveJugId();
  auto animEvent = new AnimEvent();
  animEvent->setLaunch(launch);
  animEvent->setHandLaunch(handLaunch);
  animEvent->setHandRecieve(newLaunchHand);
  animEvent->setJugLaunchId(jugLaunchId);
  animEvent->setJugRecieveId(newJugId);
  animEvent->setNewLaunch(newLaunch);
  returnVec.append(animEvent);

  while (newLaunchPos != initialLaunchPos || newLaunchHand != initialHandLaunch || jugLaunchId != initialJugLaunchId)
  {
    handLaunch = newLaunchHand;
    launchPos = newLaunchPos;
    launch = at(launchPos);
    (launch % 2 == 1) ? newLaunchHand = changeHand(handLaunch) : newLaunchHand = handLaunch;
    newLaunchPos = (launch + launchPos) % m_period;
    newLaunch = at(newLaunchPos);
    if (jugLaunchId != initialJugLaunchId && newJugId == jugLaunchId)
      jugLaunchId = 1; // FIXME: Ugly !
    else
      jugLaunchId = newJugId;
    if (jugLaunchId != initialJugLaunchId) // if it's the other juggler
      newJugId = 0; // get back to first
    else
      newJugId = m_v_event.at(launchPos)->getReceiveJugId();

    auto animEvent = new AnimEvent();
    animEvent->setLaunch(launch);
    animEvent->setHandLaunch(handLaunch);
    animEvent->setHandRecieve(newLaunchHand);
    animEvent->setJugLaunchId(jugLaunchId);
    animEvent->setJugRecieveId(newJugId);
    animEvent->setNewLaunch(newLaunch);
    returnVec.append(animEvent);
  }
  return returnVec;
}

void SiteSwap::setState()
{
  if (!m_period) // this shoud not happen
    return;

  // simple case
  if (m_period == 1)
  {
    m_state.resize(m_propCount);
    for (int i = 0; i < m_propCount; i++)
    {
      m_state.setBit(i, true);
    }
    return;
  }

  // a bit more complex
  int propFoundCount = 0; // count of true sites found
  int index = 0;
  QVector<int> indexAlreadySet; // store false forward sites

  while (propFoundCount < m_propCount) // while we haven't found all true sites
  {
    if (index + 1 > m_state.size())
      m_state.resize(index + 1); // we must resize
    int launch = at(index % m_period);
    if (launch) // if launch is not 0
    {
      // verify if site has not already been set
      if (indexAlreadySet.indexOf(index) == -1)
      {
        m_state.setBit(index, true); // it's a true site
        propFoundCount++; // we found one more
      }
      if ((index + launch + 1) > m_state.size())
        m_state.resize(index + launch + 1); // we must resize
      m_state.setBit(index + launch, false); // it's surely a false
      indexAlreadySet.append(index + launch); // we declare it as already set
    }
    index++;
  }
  // now we truncate finishing 0s
  bool test = m_state.testBit(m_state.size() - 1);
  while (test == false)
  {
    m_state.resize(m_state.size() - 1); // erase last bit
    test = m_state.testBit(m_state.size() - 1);
  }
}
