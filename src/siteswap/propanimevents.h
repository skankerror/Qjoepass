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

#ifndef PROPANIMEVENTS_H
#define PROPANIMEVENTS_H

#include <QObject>
#include <QVector>
#include "qjoepass.h"

struct propAnimEvent
{
  int s_jugglerLaunchId;
  hand s_launchHand;
  int s_launch;
  int s_jugglerReceiveId;
  hand s_receiveHand;
  int s_startTime; // needed to pass to hand anim
  int s_propId; // needed to pass to hand anim
  int s_loopDuration; // set this for the first one, needed by hand anim
};


class PropAnimEvents: public QObject
{

  Q_OBJECT

public:

  PropAnimEvents(int t_propId,
                 QObject *parent = nullptr);

  int getPropId() const {return m_propId; }
  int getLoopDuration() const { return m_loopDuration; }
  int getSize() const { return m_v_propAnimEvent.size(); }
  propAnimEvent *getPropAnimEventAt(int t_index) const;

  void appendPropAnimEvent(propAnimEvent * t_propAnimEvent);

signals:

  void sizeChanged();

private slots:

  void setLoopDuration();

private:

  int m_propId;
  QVector<propAnimEvent *> m_v_propAnimEvent;
  int m_loopDuration = 0; // in ms

};

#endif // PROPANIMEVENTS_H
