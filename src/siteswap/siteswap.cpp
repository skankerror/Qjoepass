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

SiteSwap::SiteSwap(QVector<siteswapEvent *> &t_v_event,
                   int t_jugCount,
                   bool t_synchron,
                   QObject *parent)
  : QObject(parent),
    m_v_event(t_v_event),
    m_synchron(t_synchron),
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
  // TODO: what about passing siteswap ?
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

QVector<animEvent *> SiteSwap::getAnimEvents(int t_launchPos,
                                             hand t_handLaunch,
                                             int t_jugLaunchId)
{
  QVector<animEvent*> v_returnVec;

  hand initialHandLaunch = t_handLaunch; // to keep it
  int initialLaunchPos = t_launchPos; // idem
  int initialJugLaunchId = t_jugLaunchId;

  int myLaunch = at(t_launchPos);
  hand newLaunchHand;
  (myLaunch % 2 == 1) ?
        newLaunchHand = changeHand(t_handLaunch) :
      newLaunchHand = t_handLaunch;
  int newLaunchPos = (myLaunch + t_launchPos) % m_period;
  int myNewLaunch = at(newLaunchPos);
  int newJugId = m_v_event.at(t_launchPos)->jugRecieveId;
  auto myAnimEvent = new struct animEvent; // TODO: vérifier le delete
  myAnimEvent->launch = myLaunch;
  myAnimEvent->handLaunch = t_handLaunch;
  myAnimEvent->handRecieve = newLaunchHand;
  myAnimEvent->jugLaunchId = t_jugLaunchId;
  myAnimEvent->jugRecieveId = newJugId;
  myAnimEvent->newLaunch = myNewLaunch;
  v_returnVec.append(myAnimEvent);

  while (newLaunchPos != initialLaunchPos ||
         newLaunchHand != initialHandLaunch ||
         t_jugLaunchId != initialJugLaunchId)
  {
    t_handLaunch = newLaunchHand;
    t_launchPos = newLaunchPos;
    myLaunch = at(t_launchPos);
    (myLaunch % 2 == 1) ?
          newLaunchHand = changeHand(t_handLaunch) :
        newLaunchHand = t_handLaunch;
    newLaunchPos = (myNewLaunch + t_launchPos) % m_period;
    myNewLaunch = at(newLaunchPos);
    if (t_jugLaunchId != initialJugLaunchId && newJugId == t_jugLaunchId)
      t_jugLaunchId = 1; // TODO: Ugly !
    else
      t_jugLaunchId = newJugId;
    if (t_jugLaunchId != initialJugLaunchId) // if it's the other juggler
      newJugId = 0; // get back to first
    else
      newJugId = m_v_event.at(t_launchPos)->jugRecieveId;

    struct animEvent *newAnimEvent = new struct animEvent; // TODO: le delete
    newAnimEvent->launch = myLaunch;
    newAnimEvent->handLaunch = t_handLaunch;
    newAnimEvent->handRecieve = newLaunchHand;
    newAnimEvent->jugLaunchId = t_jugLaunchId;
    newAnimEvent->jugRecieveId = newJugId;
    newAnimEvent->newLaunch = myNewLaunch;
    v_returnVec.append(newAnimEvent);
  }
  return v_returnVec;
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
