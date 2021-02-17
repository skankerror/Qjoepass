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

#include "propanimevents.h"

PropAnimEvents::PropAnimEvents(int t_propId,
                               QObject *parent)
  : QObject(parent),
    m_propId(t_propId)
{
  connect(this, SIGNAL(sizeChanged()), this, SLOT(setLoopDuration()));
}

propAnimEvent *PropAnimEvents::getPropAnimEventAt(int t_index) const
{
  if (t_index < 0 ||
      t_index < getSize())
  {
    qDebug() << "out of range in PropAnimEvents";
    return nullptr;
  }
  return m_v_propAnimEvent.at(t_index);
}

void PropAnimEvents::appendPropAnimEvent(propAnimEvent *t_propAnimEvent)
{
  m_v_propAnimEvent.append(t_propAnimEvent);
  emit sizeChanged();
}

void PropAnimEvents::setLoopDuration()
{
  // restart the count
  m_loopDuration = 0;

  for (int i = 0; i < getSize(); i++)
  {
    int launch = getPropAnimEventAt(i)->s_launch;
    int duration = launch * (int)((HAND_PERIOD * S_TO_MS) / 2);
    m_loopDuration += duration;
  }

  for (int i = 0; i < getSize(); i++)
  {
    auto myPropAnimEvent = getPropAnimEventAt(i);
    myPropAnimEvent->s_loopDuration = m_loopDuration;
  }
}
