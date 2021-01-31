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
#include <QVector>
#include <QBitArray>
#include "qjoepass.h"


// To handle atomic siteswap information
struct siteswapEvent
{
  int s_launch;
  int s_jugLaunchId;
  int s_jugRecieveId;
  bool s_isMulti; // if true, group with next one.
};

// To send informations to animation
struct animEvent
{
  int s_jugLaunchId;
  hand s_handLaunch;
  int s_launch;
  int s_jugRecieveId;
  hand s_handRecieve;
  int s_newLaunch;
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
  bool isValid() const;
  int getNumProp() const;
  int getPeriod() const { return m_period; };
  QBitArray getState() const { return m_state; };
  int getJugglerCount() const { return m_jugglerCount; };

  // For sending datas to animation
  QVector<animEvent *> getAnimEvents(int t_launchPos,
                                    hand t_handLaunch,
                                    int t_jugLaunchId);

private:

  void setState();
  hand changeHand(hand t_hand){ return (t_hand == leftHand) ? rightHand : leftHand; };

private:

  QVector<siteswapEvent *> m_v_event;
  int m_period;
  bool m_valid = false;
  bool m_synchron = false;
//  propType m_prop;
//  int m_launchType;
  QBitArray m_state;
  int m_propCount;
  int m_jugglerCount;

};

#endif // SITESWAP_H
