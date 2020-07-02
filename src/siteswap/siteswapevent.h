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
  explicit SiteswapEvent(int aLaunch = 0,
//                         bool pass = false,
                         int id1 = 0,
                         int id2 = 0,
                         bool multi = false,
                         QObject *parent = nullptr);


  // getters
  int getLaunch() const { return launch; };
  bool isMultiplex() const {return multiplex; };
  bool isPassing() const { return passing; };
  int getPassJugId() const { return passJugId; };
  int getReceiveJugId() const { return receiveiJugId; };

  // setters
  void setLaunch(const int aLaunch) { launch = aLaunch; };
  void setMultiplex(const bool aBool) { multiplex = aBool; };
  void setPassing(const bool aBool) { passing = aBool; };
  void setPassJugId(const int id) { passJugId = id; };
  void setReceiveJugId(const int id) { receiveiJugId = id; };

signals:

private:
  int launch;
  bool multiplex = false; // if it's true, group with the next one
  bool passing = false;
  int passJugId;
  int receiveiJugId;

};

#endif // SITESWAPEVENT_H
