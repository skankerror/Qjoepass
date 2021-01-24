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

// NOTE: faut-il calculer le nombre de jongleur ici ?

// To handle atomic siteswap information
struct siteswapEvent
{
  int launch;
  int jugLaunchId;
  int jugRecieveId;
  bool isMulti; // if true, group with next one.
};

// To send informations to animation
struct animEvent
{
  int jugLaunchId;
  hand handLaunch;
  int launch;
  int jugRecieveId;
  hand handRecieve;
  int newLaunch;
  // int launchType;
};


class SiteSwap : public QObject
{

  Q_OBJECT

public:

  explicit SiteSwap(QVector<siteswapEvent *> &t_v_event,
                    int t_jugCount,
                    jugglingProp t_prop = ball,
                    bool t_synchron = false,
                    QObject *parent = nullptr);

  int at(int i) const { return m_v_event.at(i)->launch; }

  // getters
  bool isValid() const;
  int getNumProp() const;
  int getPeriod() const { return m_period; };
  jugglingProp getPropType() const { return m_prop; };
  int getLaunchType() const { return m_launchType ;};
  QBitArray getState() const { return m_state; };
  int getJugglerCount() const { return m_jugglerCount; };

  // For sending datas to animation
  QVector<animEvent *> getAnimEvents(int t_launchPos,
                                    hand t_handLaunch,
                                    int t_jugLaunchId);

  // setters
  void setPropType(jugglingProp t_prop);
  void setLaunchType(int t_launchType) { m_launchType = t_launchType; };

private:

  void setState();
  hand changeHand(hand t_hand){ return (t_hand == leftHand) ? rightHand : leftHand; };

private:

  QVector<siteswapEvent *> m_v_event;
  int m_period;
  bool m_valid = false;
  bool m_synchron = false;
  jugglingProp m_prop;
  int m_launchType;
  QBitArray m_state;
  int m_propCount;
  int m_jugglerCount;

};

#endif // SITESWAP_H
