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

#ifndef SITESWAP_H
#define SITESWAP_H

#include <QObject>
#include <QBitArray>
#include "propanimevents.h"
#include "handanimevents.h"
#include "qjoepass.h"


// To handle atomic siteswap information
struct siteswapEvent
{
  int s_launch;
  bool s_isMulti; // if true, group with next one.
};

class SiteSwap : public QObject
{

  Q_OBJECT

public:

  explicit SiteSwap(QVector<siteswapEvent *> &t_v_event,
                    int t_jugCount,
                    bool t_synchron = false,
                    QObject *parent = nullptr);

  int at(int i) const { return m_v_event.at(i)->s_launch; }

  // getters
  bool isValid() const { return m_valid; };
  int getPropCount() const { return m_propCount; };
  int getPeriod() const { return m_period; };
  QBitArray getState() const { return m_state; };

  // For sending datas to animation
  QVector<PropAnimEvents *> getTotalPropAnimEvents() const { return m_v_propAnimEvents; };
  QVector<HandAnimEvents *> getTotalHandAnimEvents() const { return m_v_handAnimEvents; };

private:

  void setValidity();
  void setPropCount();
  void setState();
  void setCompleteSiteswap();
  void setRealistic();

  hand changeHand(hand t_hand){ return (t_hand == leftHand) ? rightHand : leftHand; };

  void setTotalAnimEvents();
  void setPropAnimEvents(int t_launchPos,
                         int t_jugglerLaunchId,
                         hand t_launchHand,
                         int t_idInState,
                         int t_propId);
  void setHandsAnimEvents();

  // needed to reorder m_v_handAnimEvents
  static bool wayToSort(handAnimEvent *t_firstHandAnimEvent,
                        handAnimEvent *t_secondHandAnimEvent)
  { return t_firstHandAnimEvent->s_startTime < t_secondHandAnimEvent->s_startTime; };

  void reorderHandsAnimEvents();

private:

  QVector<siteswapEvent *> m_v_event;
  int m_period;
  bool m_valid = false;
  bool m_realistic = false; // bool to know is this can be really animed irl
  bool m_synchron = false;
  QBitArray m_state;

  /* set complete form of siteswap
   * simplier for set propAnimEvent
   * for example a 3 siteswap will become 333 */
  QVector<int> m_v_completeSiteswap;
  int m_periodCompleteSiteswap;

  int m_propCount;
  int m_jugglerCount;

  // all informations to move props
  QVector<PropAnimEvents *> m_v_propAnimEvents;

  // all informations to move hands
  QVector<HandAnimEvents *> m_v_handAnimEvents;

};

#endif // SITESWAP_H
