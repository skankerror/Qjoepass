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

#include "siteswap.h"

SiteSwap::SiteSwap(QVector<int> &aVInt,
                   bool aSynchron,
                   QObject *parent)
  : QObject(parent),
    v_event(aVInt),
    synchron(aSynchron)
{
  period = v_event.size();
  valid = isValid();
}

bool SiteSwap::isValid() const
{
  // create a vector to test our values
  QVector<int> vTest;
  // let's be optimistic !
  bool ret = true;

  for (int i = 0; i < vTest.size(); i++)
  {
    // we modulo each launch
    int modLaunch = v_event.at(i) % period;
    // and test to find collision
    if (vTest.indexOf(modLaunch) != -1) // collision
    {
      ret = false; // bad news
      break;
    }
    // this seems good, we're adding to our vector for next tests
    vTest.append(modLaunch);
  }
  return ret;
}

int SiteSwap::getNumProp() const
{
  if(!valid)
    return 0;

  int totalLaunch = 0;
  for (int i = 0; i < v_event.size(); i++)
    totalLaunch += v_event.at(i);
  return totalLaunch / period;
}
