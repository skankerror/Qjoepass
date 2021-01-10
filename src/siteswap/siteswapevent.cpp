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

#include "siteswapevent.h"

SiteswapEvent::SiteswapEvent(int t_launch,
                             int t_id1,
                             int t_id2,
                             bool t_multi,
                             QObject *parent)
  : QObject(parent),
    m_launch(t_launch),
    m_multiplex(t_multi),
//    m_passing(pass),
    m_passJugId(t_id1),
    m_receiveiJugId(t_id2)
{
//  if (m_passJugId != m_receiveiJugId)
//    setPassing(true);
//  else
//    setPassing(false);

  (m_passJugId != m_receiveiJugId) ? setPassing(true) : setPassing(false);
}
