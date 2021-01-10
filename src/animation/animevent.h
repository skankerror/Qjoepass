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

#ifndef ANIMEVENT_H
#define ANIMEVENT_H

#include <QObject>
#include "qjoepass.h"

// class wich stores datas for each launch
// TODO: make a struct instead ?

class AnimEvent : public QObject
{
  Q_OBJECT

public:

  explicit AnimEvent(QObject *parent = nullptr);

  // getters
  int getJugLaunchId() { return jugLaunchId; };
  hand getHandLaunch() { return handLaunch; };
  int getLaunch() { return launch; };
  int getJugRecieveId() { return jugRecieveId; };
  hand getHandRecieve() { return handRecieve; };
  int getNewLaunch() { return newLaunch; };

  // setters
  void setJugLaunchId(int id) { jugLaunchId = id; };
  void setHandLaunch(hand aHand) { handLaunch = aHand; };
  void setLaunch(int aLaunch) { launch = aLaunch; };
  void setJugRecieveId(int id) { jugRecieveId = id; };
  void setHandRecieve(hand aHand) { handRecieve = aHand; };
  void setNewLaunch(int aLaunch) { newLaunch = aLaunch; };

private:

  int jugLaunchId;
  hand handLaunch;
  int launch;
  int jugRecieveId;
  hand handRecieve;
  int newLaunch;
  // int launchType;
};

#endif // ANIMEVENT_H
