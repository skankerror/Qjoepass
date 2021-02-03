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
  setValidity();
  setPropCount();
  if (m_valid)
  {
    setState();
    qDebug() << m_state;
    setCompleteSiteswap();
    qDebug() << m_v_completeSiteswap;
    setTotalAnimEvents();
  }
}


void SiteSwap::setValidity()
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
  m_valid = ret;
}

void SiteSwap::setPropCount()
{
  if(!m_valid)
    m_propCount = 0;

  int totalLaunch = 0;
  for (int i = 0; i < m_v_event.size(); i++)
    totalLaunch += at(i);
  m_propCount =  totalLaunch / m_period;
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

// TODO: make a do while ?
void SiteSwap::setCompleteSiteswap()
{
  // transform siteswap so his lenght >= prop Count

  // simple case, we copy launches in our vector and return
//  if (m_propCount <= m_v_event.size())
  if (m_state.size() <= m_v_event.size())
  {
    for (int i = 0; i < m_v_event.size(); i++)
    {
      int launch = at(i);
      m_v_completeSiteswap.append(launch);
    }
  }
  else
  {
//    while (m_propCount > m_v_completeSiteswap.size())
    while (m_state.size() > m_v_completeSiteswap.size())
    {
      for (int i = 0; i < m_v_event.size(); i++)
      {
        int launch = at(i);
        m_v_completeSiteswap.append(launch);
      }
    }
  }

  m_periodCompleteSiteswap = m_v_completeSiteswap.size();
}

void SiteSwap::setTotalAnimEvents()
{

  // we use state
  for (int i = 0; i < m_state.size(); i++) // for each bit in state
  {
    if (m_state.testBit(i)) // if it's a site launch
    {
      int launchPos = i % m_periodCompleteSiteswap; // i may be beyond period

      // find which juggler is launching
      int jugglerLaunchId = i % m_jugglerCount;

      // find launching hand
      hand launchHand;
      // int to check wich hand is launching
      int checkLaunchHand = i % (2 * m_jugglerCount);
      (checkLaunchHand < m_jugglerCount) ?
            launchHand = rightHand: // NOTE: we always begin with right hands
          launchHand = leftHand;


      // get prop anim loop
      auto propAnimLoop = getPropAnimEvents(launchPos,
                                            jugglerLaunchId,
                                            launchHand);
      // add to our argument
      m_v_v_propAnimEvents.append(propAnimLoop);
    }
  }
}

// TODO: rewrite with a do while
QVector<animEvent *> SiteSwap::getPropAnimEvents(int t_launchPos,
                                                 int t_jugglerLaunchId,
                                                 hand t_launchHand)
{
  QVector<animEvent *> v_returnVec;

  // to keep nitial values
  hand initialHandLaunch = t_launchHand;
  int initialLaunchPos = t_launchPos;
  int initialJugglerLaunchId = t_jugglerLaunchId;

  // get launch
  int launch = m_v_completeSiteswap.at(t_launchPos);

  // find next launch pos
  int nextLaunchPos = (launch + t_launchPos) % m_periodCompleteSiteswap;

  // find id of receiving juggler
  int jugglerReceiveId = nextLaunchPos % m_jugglerCount;

  // find hand of receiving juggler
  hand receiveHand;
  // int to check wich hand is receiving
  int checkReceiveHand = (launch + (t_launchPos % m_jugglerCount)) % (2 * m_jugglerCount);
  (checkReceiveHand < m_jugglerCount) ?
        receiveHand = t_launchHand:
      receiveHand = changeHand(t_launchHand);

  // we can set our first anim event
  auto firstAnimEvent = new struct animEvent; // TODO: vérifier le delete
  firstAnimEvent->s_jugglerLaunchId = t_jugglerLaunchId;
  firstAnimEvent->s_launchHand = t_launchHand;
  firstAnimEvent->s_launch = launch;
  firstAnimEvent->s_jugglerRecieveId = jugglerReceiveId;
  firstAnimEvent->s_receiveHand = receiveHand;
  // append to our vec
  v_returnVec.append(firstAnimEvent);

  // to prepare the rest
  // set next hand
  hand nextLaunchHand = receiveHand;
  // set next juggler launch id
  int nextJugglerLaunchId = jugglerReceiveId;

  // while prop hasn't finished his total loop
  while (nextLaunchPos != initialLaunchPos ||
         nextLaunchHand != initialHandLaunch ||
         nextJugglerLaunchId != initialJugglerLaunchId)
  {
    // we're in the next throw
    t_launchHand = nextLaunchHand;
    t_launchPos = nextLaunchPos;
    t_jugglerLaunchId = nextJugglerLaunchId;

    // get launch
    launch = m_v_completeSiteswap.at(t_launchPos);

    // find next launch pos
    nextLaunchPos = (launch + t_launchPos) % m_periodCompleteSiteswap;

    // find id of receiving juggler
    jugglerReceiveId = nextLaunchPos % m_jugglerCount;

    // for while loop check
    nextJugglerLaunchId = jugglerReceiveId;

    // find hand of receiving juggler
    checkReceiveHand = (launch + (t_launchPos % m_jugglerCount)) % (2 * m_jugglerCount);
      (checkReceiveHand < m_jugglerCount) ?
            receiveHand = t_launchHand:
          receiveHand = changeHand(t_launchHand);

    // for while loop check
    nextLaunchHand = receiveHand;

    // set next anim event
    auto nextAnimEvent = new struct animEvent; // TODO: vérifier le delete
    nextAnimEvent->s_jugglerLaunchId = t_jugglerLaunchId;
    nextAnimEvent->s_launchHand = t_launchHand;
    nextAnimEvent->s_launch = launch;
    nextAnimEvent->s_jugglerRecieveId = jugglerReceiveId;
    nextAnimEvent->s_receiveHand = receiveHand;
    // append to our vec
    v_returnVec.append(nextAnimEvent);
  }
  // we have complete prop loop
  return v_returnVec;
}
