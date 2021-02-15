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
#include <algorithm>

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
    setCompleteSiteswap();
    setTotalAnimEvents();
    setRealistic();
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
    qDebug() << "state : " << m_state;
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
  qDebug() << "state : " << m_state;
}

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
  qDebug() << "complete siteswap : " << m_v_completeSiteswap;
  m_periodCompleteSiteswap = m_v_completeSiteswap.size();
}

void SiteSwap::setTotalAnimEvents()
{
  // we set propId to pass to handAnimEvent
  int propId = 0;

  // create this to handle 0 launch in hand anim
  QVector<hand0AnimEvent *> v_hand0AnimEvents;

  // we use state
  for (int i = 0; i < m_state.size(); i++) // for each bit in state
  {
    // find which juggler is launching
    int jugglerLaunchId = i % m_jugglerCount;

    // find launching hand
    hand launchHand;
    // int to check wich hand is launching
    int checkLaunchHand = i % (2 * m_jugglerCount);
    (checkLaunchHand < m_jugglerCount) ?
          launchHand = rightHand: // NOTE: we always begin with right hands
        launchHand = leftHand;

    if (m_state.testBit(i)) // if it's a site launch
    {
      int launchPos = i % m_periodCompleteSiteswap; // i may be beyond period

      // set prop anim loop
      setPropAnimEvents(launchPos,
                        jugglerLaunchId,
                        launchHand,
                        i,
                        propId);

      // update propId
      propId++;
    }
    else // it's not a launch site
    {
      // pass to handAnim
      auto myHand0AnimEvent = new struct hand0AnimEvent;
      myHand0AnimEvent->s_startTime = (i / m_jugglerCount) * ((HAND_PERIOD  * S_TO_MS) / 2.0f);
      myHand0AnimEvent->s_jugglerId = jugglerLaunchId;
      myHand0AnimEvent->s_hand = launchHand;
    }
  }
  // now that we have all prop anim events, we can set hands anims events
  setHandsAnimEvents(v_hand0AnimEvents);
}

// TODO: rewrite with a do while
void SiteSwap::setPropAnimEvents(int t_launchPos,
                                 int t_jugglerLaunchId,
                                 hand t_launchHand,
                                 int t_idInState,
                                 int t_propId)
{
  QVector<propAnimEvent *> v_propAnimEvent;

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
        receiveHand = t_launchHand:      receiveHand = changeHand(t_launchHand);

  // to find starting time (in ms) to prepare hand anim
  // TODO: works only for asynchron siteswap
  // FIXME:
  int halfHandPeriod = (HAND_PERIOD  * S_TO_MS) / 2.0f;
//  qDebug() << "id in state" << t_idInState;
  int startTime = (t_idInState / m_jugglerCount) * halfHandPeriod;
//  qDebug() << "startTime" << startTime;
  // calculate next startTime
  int nextStartTime = ((launch / m_jugglerCount) * halfHandPeriod) + startTime; // TODO: check this
//  qDebug() << "next Start time" << nextStartTime;


  // we can set our first anim event
  auto firstAnimEvent = new struct propAnimEvent; // TODO: vérifier le delete
  firstAnimEvent->s_jugglerLaunchId = t_jugglerLaunchId;
  firstAnimEvent->s_launchHand = t_launchHand;
  firstAnimEvent->s_launch = launch;
  firstAnimEvent->s_jugglerReceiveId = jugglerReceiveId;
  firstAnimEvent->s_receiveHand = receiveHand;
  firstAnimEvent->s_startTime = startTime;
  firstAnimEvent->s_propId = t_propId;
  // append to our vec
  v_propAnimEvent.append(firstAnimEvent);

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
    auto nextAnimEvent = new struct propAnimEvent; // TODO: vérifier le delete
    nextAnimEvent->s_jugglerLaunchId = t_jugglerLaunchId;
    nextAnimEvent->s_launchHand = t_launchHand;
    nextAnimEvent->s_launch = launch;
    nextAnimEvent->s_jugglerReceiveId = jugglerReceiveId;
    nextAnimEvent->s_receiveHand = receiveHand;
    nextAnimEvent->s_startTime = nextStartTime;
    nextAnimEvent->s_propId = t_propId;
    // append to our vec
    v_propAnimEvent.append(nextAnimEvent);

    nextStartTime += (launch / m_jugglerCount) * halfHandPeriod; // TODO: check this
//    qDebug() << "next Start time" << nextStartTime;

  }
  // we have complete prop loop
  m_v_v_propAnimEvents.append(v_propAnimEvent);
}

void SiteSwap::setHandsAnimEvents(QVector<hand0AnimEvent *> t_v_hand0animEvents)
{
  // create a vec<handAnimEvents*> for each hand
  for (int i = 0; i < m_jugglerCount; i++)
  {
    // right hand vec
    auto v_rightHandAnimEvents = new QVector<handAnimEvent *>();
    m_v_v_handAnimEvents.append(v_rightHandAnimEvents);

    // left hand vec
    auto v_leftHandAnimEvents = new QVector<handAnimEvent *>();
    m_v_v_handAnimEvents.append(v_leftHandAnimEvents);
  }

  // get all informations from prop anim events
  for (int i = 0; i < m_v_v_propAnimEvents.size(); i++)
  {
    auto v_propAnimEvents = m_v_v_propAnimEvents.at(i);
    for (int j = 0; j < v_propAnimEvents.size(); j++)
    {
      auto myPropAnimEvents = v_propAnimEvents.at(j);
      auto myHandAnimEvents = new struct handAnimEvent; // TODO: make delete somewhere
      myHandAnimEvents->s_propId = myPropAnimEvents->s_propId;
      myHandAnimEvents->s_startTime = myPropAnimEvents->s_startTime;
      myHandAnimEvents->s_launch = myPropAnimEvents->s_launch;
      myHandAnimEvents->s_jugglerReceiveId = myPropAnimEvents->s_jugglerReceiveId;
      myHandAnimEvents->s_receiveHand = myPropAnimEvents->s_receiveHand;

//      qDebug() << "start time in hand anim" << myHandAnimEvents->s_startTime;

      // get the correct hand vec
      int id;
      (myPropAnimEvents->s_launchHand == rightHand) ?
            id = myPropAnimEvents->s_jugglerLaunchId * 2:
          id = (myPropAnimEvents->s_jugglerLaunchId * 2) + 1;

      m_v_v_handAnimEvents.at(id)->append(myHandAnimEvents);
    }
  }
  // add 0 anim events
  if (t_v_hand0animEvents.size())
  {
    for (int i = 0; i < t_v_hand0animEvents.size(); i++)
    {
      auto myHand0AnimEvents = t_v_hand0animEvents.at(i);
      auto myHandAnimEvents = new struct handAnimEvent; // TODO: make delete somewhere
      myHandAnimEvents->s_propId = NO_PROP;
      myHandAnimEvents->s_startTime = myHand0AnimEvents->s_startTime;
      myHandAnimEvents->s_launch = 0;

      // get the correct hand vec
      int id;
      (myHand0AnimEvents->s_hand == rightHand) ?
            id = myHand0AnimEvents->s_jugglerId * 2:
          id = (myHand0AnimEvents->s_jugglerId * 2) + 1;

      m_v_v_handAnimEvents.at(id)->append(myHandAnimEvents);

    }
  }

  // reorder m_v_v_handAnimEvents
  reorderHandsAnimEvents();

  /***************** check *******************/
//  qDebug() << "m_v_v_handAnimEvents size : " << m_v_v_handAnimEvents.size();
//  for (int i = 0; i < m_v_v_handAnimEvents.size(); i++)
//  {
//    auto v_handAnimEvents = m_v_v_handAnimEvents.at(i);
//    qDebug() << "HAND ANIM n° : " << i;
//    for (int j = 0; j < v_handAnimEvents->size(); j++)
//    {
//      auto myHandAnimEvents = v_handAnimEvents->at(j);
//      qDebug() << "EVENT n° : " << j;
//      qDebug() << "*start time" << myHandAnimEvents->s_startTime;
//      qDebug() << "*prop id" << myHandAnimEvents->s_propId;
//      qDebug() << "*rec hand" << myHandAnimEvents->s_receiveHand;
//    }
//  }

}

void SiteSwap::reorderHandsAnimEvents()
{
  for (int i = 0; i < m_v_v_handAnimEvents.size(); i++)
  {
    auto v_handAnimEvents = m_v_v_handAnimEvents.at(i);
    std::sort(v_handAnimEvents->begin(), v_handAnimEvents->end(), wayToSort);
    for (int j = 0; j < v_handAnimEvents->size(); j++)
    {
//      qDebug() << "start time in : " << i << " : " << v_handAnimEvents->at(j)->s_startTime;
    }
  }
}

// FIXME: this doesn't work for 5511 2 jugglers
void SiteSwap::setRealistic()
{
  for (int i = 0; i < m_v_v_propAnimEvents.size(); i++)
  {
    QVector<propAnimEvent *> v_propAnimEvents;
    for (int j = 0; j < v_propAnimEvents.size(); j++)
    {
      auto anAnimEvent = v_propAnimEvents.at(j);
      if (anAnimEvent->s_jugglerLaunchId != anAnimEvent->s_jugglerReceiveId
          && anAnimEvent->s_launch < m_jugglerCount)
      {
        qDebug() << "vanilla siteswap can not be realisticly animed,"
                << "because there's at least one launch arriving at the moment it will be launch";
        m_realistic = false;
        return;
      }
    }
  }
  m_realistic = true;
  return;
}
