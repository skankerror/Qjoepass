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

#ifndef SITESWAPEVENT_H
#define SITESWAPEVENT_H

#include <QObject>

class SiteswapEvent : public QObject
{

  Q_OBJECT

public:

  explicit SiteswapEvent(int t_launch = 0,
//                         bool t_pass = false,
                         int t_id1 = 0,
                         int t_id2 = 0,
                         bool t_multi = false,
                         QObject *parent = nullptr);


  // getters
  int getLaunch() const { return m_launch; };
  bool isMultiplex() const {return m_multiplex; };
  bool isPassing() const { return m_passing; };
  int getPassJugId() const { return m_passJugId; };
  int getReceiveJugId() const { return m_receiveiJugId; };

  // setters
  void setLaunch(const int t_launch) { m_launch = t_launch; };
  void setMultiplex(const bool t_bool) { m_multiplex = t_bool; };
  void setPassing(const bool t_bool) { m_passing = t_bool; };
  void setPassJugId(const int t_id) { m_passJugId = t_id; };
  void setReceiveJugId(const int t_id) { m_receiveiJugId = t_id; };

private:

  int m_launch;
  bool m_multiplex = false; // if it's true, group with the next one
  bool m_passing = false;
  int m_passJugId;
  int m_receiveiJugId;

};

#endif // SITESWAPEVENT_H
