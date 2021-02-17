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

#include "handanimevents.h"

HandAnimEvents::HandAnimEvents(int t_jugglerId,
                               hand t_hand,
                               QObject *parent)
  : QObject(parent),
    m_jugglerId(t_jugglerId),
    m_hand(t_hand)
{
  connect(this, SIGNAL(sizeChanged()), this, SLOT(setLoopDuration()));
}

void HandAnimEvents::appendHandAnimEvent(handAnimEvent *t_handAnimEvent)
{
  m_v_handAnimEvent.append(t_handAnimEvent);
  emit sizeChanged();
}

handAnimEvent *HandAnimEvents::getHandAnimEventAt(int t_index)
{
  if (t_index < 0 ||
      t_index < m_v_handAnimEvent.size())
  {
    qDebug() << "out of range in PropAnimEvents";
    return nullptr;
  }
  return m_v_handAnimEvent.at(t_index);
}

void HandAnimEvents::setLoopDuration()
{
  // prendre la plus grande duration des props qui passent par la main
  // je peux pas le prouver mais j'ai l'intuition que ça marche
  for (int i = 0; m_v_handAnimEvent.size(); i++)
  {
    int loopPropDuration = m_v_handAnimEvent.at(i)->s_loopPropDuration;
    if (loopPropDuration > m_loopDuration)
      m_loopDuration = loopPropDuration;
  }

}
