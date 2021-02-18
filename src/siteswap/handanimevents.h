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

#ifndef HANDANIMEVENTS_H
#define HANDANIMEVENTS_H

#include <QObject>
#include "qjoepass.h"

struct handAnimEvent
{
  int s_propId;
  int s_startTime;
  int s_launch;
  int s_jugglerReceiveId;
  hand s_receiveHand;
  int s_loopPropDuration;
};

class HandAnimEvents : public QObject
{

  Q_OBJECT

public:

  explicit HandAnimEvents(int t_jugglerId,
                          hand t_hand,
                          QObject *parent = nullptr);

  void appendHandAnimEvent(handAnimEvent *t_handAnimEvent);
  handAnimEvent *getHandAnimEventAt(int t_index);

  int getDuration() const { return m_loopDuration; }
  int getJugglerId() const {return m_jugglerId; }
  hand getHand() const { return m_hand; }
  int getSize() const { return m_v_handAnimEvent.size(); }

  void reorderEvents();
  void printDebug();

signals:

  void sizeChanged();

private slots:

  void setLoopDuration();

private:

  void setLastAnimDuration();

  // needed to reorder m_v_handAnimEvents
  static bool wayToSort(handAnimEvent *t_firstHandAnimEvent,
                        handAnimEvent *t_secondHandAnimEvent)
  { return t_firstHandAnimEvent->s_startTime < t_secondHandAnimEvent->s_startTime; };

private:

  int m_jugglerId;
  hand m_hand;
  QVector<handAnimEvent *> m_v_handAnimEvent;
  int m_loopDuration = 0;
  int m_lastAnimDuration;
};

#endif // HANDANIMEVENTS_H
